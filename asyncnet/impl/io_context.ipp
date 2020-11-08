//
// Created by tsing on 2020/11/5.
//

#ifndef ASYNCNET_IMPL_IO_CONTEXT_IPP
#define ASYNCNET_IMPL_IO_CONTEXT_IPP

#include <asyncnet/io_context.hpp>

#include <algorithm>

namespace asyncnet {

io_context::io_context(int concurrency_hint) noexcept
{
  (void) concurrency_hint;
}

io_context::io_context() noexcept : io_context(1) {}

io_context::~io_context() noexcept = default;

detail::intrusive_list<io_context::executor_stack_entry> &io_context::thread_local_executor_stack()
{
  static thread_local detail::intrusive_list<io_context::executor_stack_entry> stack;
  return stack;
}


struct io_context::run_stack_guard
{
  executor_stack_entry entry_;

  explicit run_stack_guard(io_context &context) : entry_{context}
  {
    thread_local_executor_stack().push_back(entry_);
  }

  ~run_stack_guard() noexcept
  {
    assert(&entry_ == &thread_local_executor_stack().back());
    thread_local_executor_stack().erase(entry_);
  }
};

io_context::count_type io_context::poll_one()
{
  run_stack_guard run_guard(*this);

  std::unique_lock<std::mutex> lock(mutex_);
  if (comp_queue_.empty())
    return 0;

  auto &h = comp_queue_.front();
  comp_queue_.erase(h);
  lock.unlock();

  h.invoke();
  return 1;
}

io_context::count_type io_context::poll()
{
  count_type n{0};
  while (poll_one())
    ++n;
  return n;
}

io_context::count_type io_context::run_one()
{
  run_stack_guard run_guard(*this);

  std::unique_lock<std::mutex> lock(mutex_);
  while (comp_queue_.empty() && !stopped() && !no_work_)
  {
    cond_.wait(lock);
  }

  if (comp_queue_.empty())
    return 0;

  auto &h = comp_queue_.front();
  comp_queue_.erase(h);
  lock.unlock();

  h.invoke();
  return 1;
}


io_context::count_type io_context::run()
{
  count_type n{0};

  while (run_one())
    ++n;

  return n;
}

void io_context::stop()
{
  std::lock_guard<std::mutex> lock(mutex_);
  stopped_ = true;
  cond_.notify_all();
}

void io_context::restart()
{
  this->~io_context();
  new (this) io_context();
}

io_context::executor_type io_context::get_executor()
{
  return executor_type(*this);
}


}// namespace asyncnet

#endif//ASYNCNET_IMPL_IO_CONTEXT_IPP
