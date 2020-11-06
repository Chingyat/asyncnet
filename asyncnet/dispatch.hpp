//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_DISPATCH_HPP
#define ASYNCNET_DISPATCH_HPP

#include <asyncnet/async_result.hpp>
#include <asyncnet/executor.hpp>
#include <asyncnet/execution_context.hpp>

namespace asyncnet {

    template<typename CompletionToken>
    typename async_result<CompletionToken, void()>::result_type
    dispatch(CompletionToken &&token);

    template<typename Executor, typename CompletionToken, std::enable_if_t<is_executor<Executor>::value, int> = 0>
    typename async_result<CompletionToken, void()>::result_type
    dispatch(const Executor &ex, CompletionToken &&token);

    template<typename ExecutionContext, typename CompletionToken, std::enable_if_t<std::is_base_of<execution_context, ExecutionContext>::value, int> = 0>
    typename async_result<CompletionToken, void()>::result_type
    dispatch(ExecutionContext &context, CompletionToken &&token);
}

#include <asyncnet/impl/dispatch.hpp>

#endif //ASYNCNET_DISPATCH_HPP
