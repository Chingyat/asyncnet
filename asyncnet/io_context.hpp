//
// Created by tsing on 2020/11/5.
//

#ifndef ASYNCNET_IO_CONTEXT_HPP
#define ASYNCNET_IO_CONTEXT_HPP

#include <asyncnet/execution_context.hpp>
#include <asyncnet/async_result.hpp>
#include <asyncnet/detail/wrapped_handler.hpp>

#include <mutex>
#include <condition_variable>
#include <list>
#include <atomic>
#include <thread>
#include <vector>

namespace asyncnet {

    /// models ExecutionContext
    class io_context : public execution_context {
    public:
        class executor_type;

        using count_type = unsigned int;

        io_context();

        explicit io_context(int concurrency_hint);

        ~io_context() noexcept;

        executor_type get_executor();

        void notify_fork(fork_event);

        /// Run the io_context event processing loop to execute ready handlers.
        count_type poll();

        /// Run the io_context event processing loop to execute one ready handler.
        count_type poll_one();

        /// Restart the io_context in preparation for a subsequent run() invocation.
        /// This function is not thread-safe.
        void restart();

        /// Run the io_context object's event processing loop.
        count_type run();

        /// Run the io_context object's event processing loop to execute at most one handler.
        count_type run_one();

        /// Stop the io_context objects' event processing loop.
        void stop();

        /// Determine whether the io_context object has been stopped.
        bool stopped() const { return _stopped; }

    private:
        mutable std::mutex _mutex;
        std::condition_variable _cond;
        std::list<detail::wrapped_handler<void()>> _compq;

        std::atomic_bool _stopped{false};

        std::atomic<count_type> _work_count{0};

        static std::vector<executor_type> &_thread_run_stack();

        struct run_stack_guard;
    };

    class io_context::executor_type {
        friend io_context;

    public:
        executor_type(const executor_type &) = default;

        executor_type &operator=(const executor_type &) = default;

        io_context &context() const { assert(_context); return *_context; }

        bool running_in_this_thread() const;

        template<typename Function, typename ProtoAllocator>
        void post(Function &&f, const ProtoAllocator &a) const;

        template<typename Function, typename ProtoAllocator>
        void dispatch(Function &&f, const ProtoAllocator &a) const;


        void on_work_finished() const noexcept;

        void on_work_started() const noexcept;

        friend bool operator==(const executor_type &x, const executor_type &y) noexcept {
            return std::addressof(x.context()) == std::addressof(y.context());
        }

        friend bool operator!=(const executor_type &x, const executor_type &y) noexcept {
            return !(x == y);
        }

    private:
        explicit executor_type(io_context &context) noexcept;

        io_context *_context;
    };


}

#include <asyncnet/impl/io_context.hpp>

#endif //ASYNCNET_IO_CONTEXT_HPP
