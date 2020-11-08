//
// Created by lince on 11/8/20.
//

#ifndef ASYNCNET_DEFER_HPP
#define ASYNCNET_DEFER_HPP

#include <asyncnet/async_completion.hpp>
#include <asyncnet/executor.hpp>

namespace asyncnet {

template <
    typename CompletionToken>
typename async_result<std::decay_t<CompletionToken>, void()>::result_type
defer(
    CompletionToken &&token);

template <
    typename Executor,
    typename CompletionToken,
    std::enable_if_t<is_executor<Executor>::value, int> = 0>
typename async_result<std::decay_t<CompletionToken>, void()>::result_type
defer(
    const Executor &ex,
    CompletionToken &&token);

template <
    typename ExecutionContext,
    typename CompletionToken,
    std::enable_if_t<std::is_convertible<ExecutionContext &, execution_context &>::value, int> = 0>
typename async_result<std::decay_t<CompletionToken>, void()>::result_type
defer(
    ExecutionContext &ctx,
    CompletionToken &&token);

}// namespace asyncnet

#include <asyncnet/impl/defer.hpp>

#endif//ASYNCNET_DEFER_HPP
