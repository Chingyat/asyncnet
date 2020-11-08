//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_IMPL_IO_CONTEXT_HPP
#define ASYNCNET_IMPL_IO_CONTEXT_HPP

#include <asyncnet/async_completion.hpp>
#include <asyncnet/io_context.hpp>

namespace asyncnet {

template <typename Allocator, unsigned int Bits>
template <typename Function, typename ProtoAllocator>
void io_context::basic_executor_type<Allocator, Bits>::post(
    Function &&f, const ProtoAllocator &a) const
{
  {
    std::lock_guard<std::mutex> lock(context().mutex_);
    context().comp_queue_.push_back(*completion_handler_base::create(a, std::forward<Function>(f)));
  }

  context().cond_.notify_one();
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
io_context::basic_executor_type<Allocator, Bits>::basic_executor_type(io_context &context) noexcept
    : context_(&context)
{
}

template <typename Allocator, unsigned int Bits>
void io_context::basic_executor_type<Allocator, Bits>::on_work_finished() const noexcept
{
  if (--context().outstanding_work_count_ == 0)
  {
    {
      std::lock_guard<std::mutex> lock(context().mutex_);
      context().no_work_ = true;
    }

    context().cond_.notify_all();
  }
}

template <typename Allocator, unsigned int Bits>
void io_context::basic_executor_type<Allocator, Bits>::on_work_started() const noexcept
{
  ++context().outstanding_work_count_;
  context().no_work_ = false;
}


struct io_context::executor_stack_entry
    : detail::intrusive_list_node<io_context::executor_stack_entry>
{
  explicit executor_stack_entry(const executor_type &ex)
      : executor_(ex) {}

  io_context::executor_type executor_;
};

template <typename Allocator, unsigned int Bits>
bool io_context::basic_executor_type<Allocator, Bits>::running_in_this_thread() const
{
  auto &stk = thread_local_executor_stack();

  return std::any_of(stk.begin(), stk.end(), [this](const auto &x) {
    return x.executor_ == *this;
  });
}

} // namespace asyncnet

#endif //ASYNCNET_IMPL_IO_CONTEXT_HPP
