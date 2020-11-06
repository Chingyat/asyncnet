//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_IMPL_POST_HPP
#define ASYNCNET_IMPL_POST_HPP

#include <asyncnet/post.hpp>
#include <asyncnet/associated_executor.hpp>
#include <asyncnet/associated_allocator.hpp>
#include <asyncnet/async_completion.hpp>
#include <asyncnet/executor_work_guard.hpp>

namespace asyncnet {
    template<typename CompletionToken>
    typename async_result<CompletionToken, void()>::result_type
    post(CompletionToken &&token) {
        async_completion<CompletionToken, void()> init(token);
        auto ex = get_associated_executor(init.completion_handler);
        auto alloc = get_associated_allocator(init.completion_handler);

        ex.post(std::move(init.completion_handler), alloc);

        return init.result.get();
    }

    template<typename Executor, typename CompletionToken, std::enable_if_t<is_executor<Executor>::value, int>>
    typename async_result<CompletionToken, void()>::result_type
    post(const Executor &ex, CompletionToken &&token) {
        async_completion<CompletionToken, void()> init(token);
        auto ex1 = get_associated_executor(init.completion_handler, ex);
        auto work = make_work(ex1);
        auto alloc = get_associated_allocator(init.completion_handler);

        auto f = [w = std::move(work),
                h = std::move(init.completion_handler),
                ex1 = std::move(ex1),
                alloc = std::move(alloc)]()  mutable {
            ex1.dispatch(std::move(h), alloc);
            w.reset();
        };
        Executor(ex).post(std::move(f), alloc);
        return init.result.get();
    }

    template<typename ExecutionContext,
            typename CompletionToken, std::enable_if_t<std::is_base_of<execution_context, ExecutionContext>::value, int>>
    typename async_result<CompletionToken, void()>::result_type
    post(ExecutionContext &context, CompletionToken &&token) {
        return post(context.get_executor(), std::forward<CompletionToken>(token));
    }
}

#endif //ASYNCNET_IMPL_POST_HPP
