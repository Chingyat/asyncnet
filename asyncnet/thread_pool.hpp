//
// Created by tsing on 2020/11/5.
//

#ifndef ASYNCNET_THREAD_POOL_HPP
#define ASYNCNET_THREAD_POOL_HPP

#include <asyncnet/execution_context.hpp>
#include <asyncnet/async_result.hpp>

namespace asyncnet {

    class thread_pool {
    public:
        class executor_type;

        thread_pool();
        explicit thread_pool(int nthreads);
        thread_pool(const thread_pool&) = delete;
        thread_pool &operator=(const thread_pool&) = delete;
        ~thread_pool() noexcept;

        executor_type get_executor();

        template <typename CompletionToken, typename ProtoAllocator>
        typename async_result<CompletionToken, void()>::result_type
        post(CompletionToken &&token, const ProtoAllocator& a);


    };

}

#endif //ASYNCNET_THREAD_POOL_HPP
