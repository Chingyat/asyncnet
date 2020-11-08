//
// Created by tsing on 2020/11/5.
//

#ifndef ASYNCNET_IMPL_IO_CONTEXT_IPP
#define ASYNCNET_IMPL_IO_CONTEXT_IPP

#include <asyncnet/detail/call_stack.hpp>
#include <asyncnet/io_context.hpp>

#include <algorithm>

namespace asyncnet {

io_context::io_context(int concurrency_hint) noexcept
{
  (void) concurrency_hint;
}

io_context::io_context() noexcept : io_context(1) {}

io_context::~io_context() noexcept
{
  shutdown();

  while (!thread_local_completion_queue().empty()) {
    auto &h = thread_local_completion_queue().front();
    thread_local_completion_queue().erase(h);
    h.release();
  }

  while (!comp_queue_.empty()) {
    auto &h = comp_queue_.front();
    comp_queue_.erase(h);
    h.release();
  }

  destroy();
}


io_context::count_type io_context::poll_one()
{
  detail::call_stack<io_context *>::frame f(this);

  if (!thread_local_completion_queue().empty()) {
    auto &h = comp_queue_.front();
    comp_queue_.erase(h);
    h.invoke();
    return 1;
  }

  std::unique_lock<std::mutex> lock(mutex_);
  if (comp_queue_.empty())
    return 0;

  auto &h = comp_queue_.front();
  comp_queue_.erase(h);
  event_.reset(lock);
  lock.unlock();

  h.invoke();
  return 1;
}

io_context::count_type io_context::poll()
{
  count_type n{0};
  while (auto i = poll_one())
    n += i;
  return n;
}

io_context::count_type io_context::run_one()
{
  detail::call_stack<io_context *>::frame f(this);

  if (!thread_local_completion_queue().empty()) {
    auto &h = comp_queue_.front();
    comp_queue_.erase(h);
    h.invoke();
    return 1;
  }

  std::unique_lock<std::mutex> lock(mutex_);
  if (comp_queue_.empty() && !no_work_ && !stopped_)
  {
    event_.wait(lock);
  }

  if (stopped_)
    return 0;

  if (comp_queue_.empty())
    return 0;

  auto &h = comp_queue_.front();
  comp_queue_.erase(h);

  event_.reset(lock);
  lock.unlock();

  h.invoke();
  return 1;
}


io_context::count_type io_context::run()
{
  count_type n{0};

  while (auto i = run_one())
    n += i;

  return n;
}

void io_context::stop()
{
  std::lock_guard<std::mutex> lock(mutex_);
  stopped_ = true;
  event_.notify_all(lock);
}

void io_context::restart()
{
  std::unique_lock<std::mutex> lock(mutex_);
  this->stopped_ = false;
  this->event_.reset(lock);
}

io_context::executor_type io_context::get_executor()
{
  return executor_type(*this);
}

io_context::completion_handler_queue &io_context::thread_local_completion_queue()
{
  static thread_local completion_handler_queue instance;
  return instance;
}


}// namespace asyncnet

#endif//ASYNCNET_IMPL_IO_CONTEXT_IPP
