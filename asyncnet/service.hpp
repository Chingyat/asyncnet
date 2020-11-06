//
// Created by tsing on 2020/11/5.
//

#ifndef ASYNCNET_SERVICE_HPP
#define ASYNCNET_SERVICE_HPP

#include <asyncnet/execution_context.hpp>

namespace asyncnet {

    class execution_context::service {
        friend class execution_context;
    public:
        explicit service(execution_context &context);

        virtual ~service() = default;

    private:
        virtual void shutdown() noexcept = 0;
        virtual void notify_fork() {}
    };


}

#endif //ASYNCNET_SERVICE_HPP
