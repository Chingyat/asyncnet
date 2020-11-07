//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_IMPL_IO_CONTEXT_HPP
#define ASYNCNET_IMPL_IO_CONTEXT_HPP

#include <asyncnet/io_context.hpp>
#include <asyncnet/async_completion.hpp>

namespace asyncnet {

template <typename Function, typename ProtoAllocator>
void io_context::executor_type::post(
    Function &&f, const ProtoAllocator &a) const
{
  {
    std::lock_guard<std::mutex> lock(context().mutex_);
    context().comp_queue_.push_back(*detail::allocate_handler<void()>(std::forward<Function>(f), a));
  }

  context().cond_.notify_one();
}

template <typename Function, typename ProtoAllocator>
void io_context::executor_type::dispatch(Function &&f, const ProtoAllocator &a) const
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
}

#endif //ASYNCNET_IMPL_IO_CONTEXT_HPP
