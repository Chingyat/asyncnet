//
// Created by tsing on 2020/11/6.
//

#ifndef ASYNCNET_IMPL_EXECUTION_CONTEXT_IPP
#define ASYNCNET_IMPL_EXECUTION_CONTEXT_IPP

#include <asyncnet/execution_context.hpp>
#include <asyncnet/service.hpp>

#include <algorithm>

namespace asyncnet {
    execution_context::~execution_context() noexcept {

    }

    void execution_context::shutdown() noexcept {
        std::vector<service *> services;

        {
            std::lock_guard<std::recursive_mutex> lock(_mutex);
            std::transform(_services.crbegin(), _services.crend(),
                           std::back_inserter(services),
                           [] (const std::pair<std::unique_ptr<service>, service_index_type > &svc) {
                return svc.first.get();
            });
        }

        std::for_each(services.cbegin(), services.cend(), [](service *svc) {
            svc->shutdown();
        });
    }


    void execution_context::destroy() noexcept {
        std::lock_guard<std::recursive_mutex> lock(_mutex);
        while (!_services.empty())
            _services.pop_back();
    }
}

#endif //ASYNCNET_IMPL_EXECUTION_CONTEXT_IPP
