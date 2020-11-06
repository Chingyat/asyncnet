//
// Created by tsing on 2020/11/5.
//

#ifndef ASYNCNET_ASYNC_COMPLETION_HPP
#define ASYNCNET_ASYNC_COMPLETION_HPP

#include <asyncnet/async_result.hpp>
#include <utility>

namespace asyncnet {

    template <typename CompletionToken, typename Signature>
    struct async_completion {
        using completion_handler_type = typename async_result<std::decay_t<CompletionToken>, Signature>::completion_handler_type;

        completion_handler_type completion_handler;
        async_result<std::decay_t<CompletionToken>, Signature> result;

        template<typename U>
        explicit async_completion(U &&u)
            : completion_handler(std::forward<CompletionToken>(u))
            , result(completion_handler)
        {}
    };
}

#endif //ASYNCNET_ASYNC_COMPLETION_HPP
