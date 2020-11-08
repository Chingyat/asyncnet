//
// Created by lince on 11/8/20.
//

#ifndef ASYNCNET_IMPL_DEFER_HPP
#define ASYNCNET_IMPL_DEFER_HPP

#include <asyncnet/defer.hpp>
#include <asyncnet/async_completion.hpp>
#include <asyncnet/associated_allocator.hpp>
#include <asyncnet/associated_executor.hpp>


namespace asyncnet {

template <
    typename CompletionToken>
typename async_result<std::decay_t<CompletionToken>, void()>::result_type
defer(
    CompletionToken &&token)
{
  async_completion<CompletionToken, void()> init(token);
  auto ex = asyncnet::get_associated_executor(init.completion_handler);
  auto alloc = asyncnet::get_associated_allocator(init.completion_handler);
  ex.defer(std::move(init.completion_handler), alloc);
  return init.result.get();
}

template <
    typename Executor,
    typename CompletionToken,
    std::enable_if_t<is_executor<Executor>::value, int>>
typename async_result<std::decay_t<CompletionToken>, void()>::result_type
defer(
    const Executor &ex,
    CompletionToken &&token)
{
  async_completion<CompletionToken, void()> init(token);
  auto ex1 = asyncnet::get_associated_executor(init.completion_handler);
  auto work = asyncnet::make_work(ex1);
  auto alloc = asyncnet::get_associated_allocator(init.completion_handler);
  auto f = [ex1,
            w = std::move(work),
            alloc,
            h = std::move(init.completion_handler)]
      () mutable
  {
    ex1.dispatch(std::move(h), alloc);
  };
  Executor(ex).defer(std::move(f), alloc);
  return init.result.get();
}

template <
    typename ExecutionContext,
    typename CompletionToken,
    std::enable_if_t<std::is_convertible<ExecutionContext &, execution_context &>::value, int>>
typename async_result<std::decay_t<CompletionToken>, void()>::result_type
defer(
    ExecutionContext &ctx,
    CompletionToken &&token)
{
  return defer(ctx.get_executor(), std::forward<CompletionToken>(token));
}

}

#endif//ASYNCNET_IMPL_DEFER_HPP
