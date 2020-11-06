//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_POST_HPP
#define ASYNCNET_POST_HPP

#include <asyncnet/async_result.hpp>
#include <asyncnet/executor.hpp>
#include <asyncnet/execution_context.hpp>

namespace asyncnet {

template <typename CompletionToken>
typename async_result<CompletionToken, void()>::result_type
post(CompletionToken &&token);

template <typename Executor, typename CompletionToken, std::enable_if_t<is_executor<Executor>::value, int> = 0>
typename async_result<CompletionToken, void()>::result_type
post(const Executor &ex, CompletionToken &&token);

template <typename ExecutionContext, typename CompletionToken, std::enable_if_t<std::is_base_of<execution_context, ExecutionContext>::value, int> = 0>
typename async_result<CompletionToken, void()>::result_type
post(ExecutionContext &context, CompletionToken &&token);

}

#include <asyncnet/impl/post.hpp>

#endif //ASYNCNET_POST_HPP
