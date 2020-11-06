//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_EXECUTOR_HPP
#define ASYNCNET_EXECUTOR_HPP

#include <asyncnet/execution_context.hpp>

#include <type_traits>

namespace asyncnet {
    namespace detail {
        template<typename T, typename = void>
        struct is_executor_impl : std::false_type {
        };

        template<typename T>
        struct is_executor_impl<T,
                std::void_t<decltype(std::declval<T>().context())>
        > : std::true_type {

        };
    }

    template<typename T>
    struct is_executor : detail::is_executor_impl<T> {
    };
}

#endif //ASYNCNET_EXECUTOR_HPP
