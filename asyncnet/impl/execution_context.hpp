//
// Created by tsing on 2020/11/6.
//

#ifndef ASYNCNET_IMPL_EXECUTION_CONTEXT_HPP
#define ASYNCNET_IMPL_EXECUTION_CONTEXT_HPP

#include <asyncnet/execution_context.hpp>
#include <asyncnet/detail/key_index.hpp>

#include <algorithm>

namespace asyncnet {
    template <typename Service, typename ...Args>
    Service& make_service(execution_context& context, Args &&...args) {
        std::unique_lock<std::recursive_mutex> lock(context._mutex);

        auto const iter = std::find_if(context._services.cbegin(), context._services.cend(),
                     [](const std::pair<std::unique_ptr<execution_context::service>, execution_context::service_index_type> &pair) {
            return pair.second == detail::key_index<typename Service::key>();
        });

        if (iter != context._services.cend())
            throw service_already_exist();

        auto svc = std::make_unique<Service>(context, std::forward<Args>(args)...);
        auto &ret = *svc;

        context._services.emplace_back(std::move(svc), detail::key_index<typename Service::key>());
        return ret;
    }

    template <typename Service>
    bool has_service(const execution_context &context) {
        std::lock_guard<std::recursive_mutex> lock(context._mutex);

        auto const iter = std::find_if(context._services.cbegin(), context._services.cend(),
                                       [](const std::pair<std::unique_ptr<execution_context::service>, execution_context::service_index_type> &pair) {
                                           return pair.second == detail::key_index<typename Service::key>();
                                       });

        return iter != context._services.cend();
    }

    template<typename Service>
    Service &use_service(execution_context &context) {
        std::unique_lock<std::recursive_mutex> lock(context._mutex);

        auto const iter = std::find_if(context._services.cbegin(), context._services.cend(),
                                       [](const std::pair<std::unique_ptr<execution_context::service>, execution_context::service_index_type> &pair) {
                                           return pair.second == detail::key_index<typename Service::key>();
                                       });

        if (iter != context._services.cend())
            return *iter->first;

        auto svc = std::make_unique<Service>(context);
        auto &ret = *svc;

        context._services.emplace_back(std::move(svc), detail::key_index<typename Service::key>());
        return ret;
    }
}

#endif //ASYNCNET_IMPL_EXECUTION_CONTEXT_HPP
