//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_ASSOCIATED_EXECUTOR_HPP
#define ASYNCNET_ASSOCIATED_EXECUTOR_HPP

#include <asyncnet/system_executor.hpp>

namespace asyncnet {

    template<typename T, typename Executor = system_executor>
    struct associated_executor {
        using type = Executor;

        static type get(const T &t, const type &ex = Executor()) {
            (void) t;
            return ex;
        }
    };

    template<typename T, typename Executor = system_executor>
    using associated_executor_t = typename associated_executor<T, Executor>::type;

    template<typename T, typename Executor = system_executor>
    associated_executor_t<T, Executor> get_associated_executor(const T &t, const Executor &ex = Executor()) {
        return associated_executor<T, Executor>::get(t, ex);
    }
}

#endif //ASYNCNET_ASSOCIATED_EXECUTOR_HPP
