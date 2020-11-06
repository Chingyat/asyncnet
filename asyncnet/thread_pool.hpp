//
// Created by tsing on 2020/11/5.
//

#ifndef ASYNCNET_THREAD_POOL_HPP
#define ASYNCNET_THREAD_POOL_HPP

#include <asyncnet/execution_context.hpp>
#include <asyncnet/async_result.hpp>

#include <atomic>

namespace asyncnet {

    class thread_pool {
    public:
        class executor_type;

        thread_pool();

        explicit thread_pool(std::size_t nthreads);

        thread_pool(const thread_pool &) = delete;

        thread_pool &operator=(const thread_pool &) = delete;

        ~thread_pool() noexcept;

        executor_type get_executor();

        void stop();

        void join();


    private:
        std::atomic_bool _stop_called;
    };

}

#endif //ASYNCNET_THREAD_POOL_HPP
