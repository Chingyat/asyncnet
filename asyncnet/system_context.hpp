//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_SYSTEM_CONTEXT_HPP
#define ASYNCNET_SYSTEM_CONTEXT_HPP

#include <asyncnet/execution_context.hpp>
#include <asyncnet/io_context.hpp>
#include <asyncnet/executor_work_guard.hpp>

#include <atomic>
#include <thread>
#include <vector>

namespace asyncnet {
    class system_executor;

    class system_context : public execution_context {
        friend system_executor;
    public:
        using executor_type = system_executor;

        executor_type get_executor();

        void join();

        void stop();

        bool stopped() const;

        ~system_context() noexcept;

    private:
        system_context();

        static system_context &get_system_context();

        static const std::size_t num_threads = 4;

        io_context _work_io_context;
        std::vector<std::thread> _threads;

        executor_work_guard<io_context::executor_type> _work;
    };

}

#endif //ASYNCNET_SYSTEM_CONTEXT_HPP
