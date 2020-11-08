//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_IMPL_IO_CONTEXT_HPP
#define ASYNCNET_IMPL_IO_CONTEXT_HPP

#include <asyncnet/async_completion.hpp>
#include <asyncnet/detail/call_stack.hpp>
#include <asyncnet/io_context.hpp>

namespace asyncnet {

template <typename Allocator, unsigned int Bits>
template <typename Function, typename ProtoAllocator>
void io_context::basic_executor_type<Allocator, Bits>::post(
    Function &&f, const ProtoAllocator &a) const
{
  std::unique_lock<std::mutex> lock(context().mutex_);
  if (context().stopped())
    return;

  context().comp_queue_.push_back(*completion_handler_base::create(a, std::forward<Function>(f)));
  context().event_.unlock_and_notify_one(lock);
}

template <typename Allocator, unsigned int Bits>
template <typename Function, typename ProtoAllocator>
void io_context::basic_executor_type<Allocator, Bits>::dispatch(Function &&f, const ProtoAllocator &a) const
{
  if (running_in_this_thread())
  {
    std::forward<Function>(f)();
  }
  else
  {
    post(std::forward<Function>(f), a);
  }
}

template <typename Allocator, unsigned int Bits>
template <typename Function, typename ProtoAllocator>
void io_context::basic_executor_type<Allocator, Bits>::defer(Function &&f, const ProtoAllocator &a) const
{
  if (running_in_this_thread())
  {
    context().thread_local_completion_queue().push_back(*completion_handler_base::create(a, std::forward<Function>(f)));
  }
  else
  {
    post(std::forward<Function>(f), a);
  }
}


template <typename Allocator, unsigned int Bits>
io_context::basic_executor_type<Allocator, Bits>::basic_executor_type(io_context &context) noexcept
    : context_(&context)
{
}

template <typename Allocator, unsigned int Bits>
void io_context::basic_executor_type<Allocator, Bits>::on_work_finished() const noexcept
{
  if (--context().outstanding_work_count_ == 0)
  {
    std::lock_guard<std::mutex> lock(context().mutex_);
    context().no_work_ = true;
    context().event_.notify_all(lock);
  }
}

template <typename Allocator, unsigned int Bits>
void io_context::basic_executor_type<Allocator, Bits>::on_work_started() const noexcept
{
  ++context().outstanding_work_count_;
  context().no_work_ = false;
}

template <typename Allocator, unsigned int Bits>
bool io_context::basic_executor_type<Allocator, Bits>::running_in_this_thread() const
{
  return detail::call_stack<io_context *>::contains(context_);
}

} // namespace asyncnet

#endif //ASYNCNET_IMPL_IO_CONTEXT_HPP
