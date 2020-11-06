//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_IMPL_DISPATCH_HPP
#define ASYNCNET_IMPL_DISPATCH_HPP

#include <asyncnet/dispatch.hpp>
#include <asyncnet/async_completion.hpp>
#include <asyncnet/associated_executor.hpp>
#include <asyncnet/associated_allocator.hpp>
#include <asyncnet/executor_work_guard.hpp>

namespace asyncnet {

template<typename CompletionToken>
typename async_result<CompletionToken, void()>::result_type
dispatch(CompletionToken &&token)
{
  async_completion<CompletionToken, void()> init(token);
  auto ex = get_associated_executor(init.completion_handler);
  auto alloc = get_associated_allocator(init.completion_handler);

  ex.dispatch(std::move(init.completion_handler), alloc);

  return init.result.get();
}

template<typename Executor, typename CompletionToken, std::enable_if_t<is_executor<Executor>::value, int>>
typename async_result<CompletionToken, void()>::result_type
dispatch(const Executor &ex, CompletionToken &&token)
{
  async_completion<CompletionToken, void()> init(token);
  auto ex1 = get_associated_executor(init.completion_handler, ex);
  auto work = make_work(ex1);
  auto alloc = get_associated_allocator(init.completion_handler);

  auto f = [
      ex1 = std::move(ex1),
      h = std::move(init.completion_handler),
      alloc = std::move(alloc),
      w = std::move(work)
  ]() mutable
  {
    ex1.dispatch(std::move(h), alloc);
    w.reset();
  };

  Executor(ex).dispatch(std::move(f), alloc);
  return init.result.get();
}

template<typename ExecutionContext, typename CompletionToken, std::enable_if_t<std::is_base_of<execution_context, ExecutionContext>::value, int>>
typename async_result<CompletionToken, void()>::result_type
dispatch(ExecutionContext &context, CompletionToken &&token)
{
  return dispatch(context.get_executor(), std::forward<CompletionToken>(token));
}

}

#endif //ASYNCNET_IMPL_DISPATCH_HPP
