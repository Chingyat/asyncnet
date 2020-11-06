//
// Created by tsing on 2020/11/5.
//

#ifndef ASYNCNET_IMPL_IO_CONTEXT_IPP
#define ASYNCNET_IMPL_IO_CONTEXT_IPP

#include <asyncnet/io_context.hpp>

#include <algorithm>

namespace asyncnet {

io_context::io_context(int concurrency_hint)
{
  (void)concurrency_hint;
}

io_context::io_context() : io_context(3) {}

io_context::~io_context() noexcept = default;

struct io_context::executor_stack_entry
    : detail::intrusive_list_node<io_context::executor_stack_entry>
{

  explicit executor_stack_entry(const executor_type &ex)
      : executor_(ex) {}

  io_context::executor_type executor_;
};

detail::intrusive_list <io_context::executor_stack_entry> &io_context::thread_local_executor_stack()
{
  static thread_local detail::intrusive_list<io_context::executor_stack_entry> stack;
  return stack;
}


struct io_context::run_stack_guard
{
  executor_stack_entry entry_;

  explicit run_stack_guard(io_context &context) : entry_{context.get_executor()}
  {
    thread_local_executor_stack().push_back(&entry_);
  }

  ~run_stack_guard() noexcept
  {
    assert(&entry_ == thread_local_executor_stack().back());
    thread_local_executor_stack().remove(&entry_);
  }
};

io_context::count_type io_context::poll_one()
{
  run_stack_guard run_guard(*this);

  std::unique_lock<std::mutex> lock(mutex_);
  if (comp_queue_.empty())
    return 0;

  auto h = comp_queue_.front();
  comp_queue_.remove(h);
  lock.unlock();

  h->invoke();
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
  cond_.wait(lock, [&]
  {
    return !comp_queue_.empty() || stopped_ || outstanding_work_count_ == 0;
  });

  if (comp_queue_.empty())
    return 0;

  auto h = comp_queue_.front();
  comp_queue_.remove(h);
  lock.unlock();

  h->invoke();
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
  stopped_ = true;
  cond_.notify_all();
}

void io_context::restart()
{
  this->~io_context();
  new(this) io_context();
}

io_context::executor_type io_context::get_executor()
{
  return executor_type(*this);
}


io_context::executor_type::executor_type(io_context &context) noexcept
    : context_(&context)
{

}

void io_context::executor_type::on_work_finished() const noexcept
{
  if (--context().outstanding_work_count_ == 0)
  {
    context().cond_.notify_all();
  }
}

void io_context::executor_type::on_work_started() const noexcept
{
  ++context().outstanding_work_count_;
}

bool io_context::executor_type::running_in_this_thread() const
{
  auto &stk = thread_local_executor_stack();

  return std::any_of(stk.begin(), stk.end(), [this](const auto &x)
  {
    return x.executor_ == *this;
  });
}

}

#endif //ASYNCNET_IMPL_IO_CONTEXT_IPP
