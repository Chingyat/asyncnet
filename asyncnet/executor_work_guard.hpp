//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_EXECUTOR_WORK_GUARD_HPP
#define ASYNCNET_EXECUTOR_WORK_GUARD_HPP

#include <asyncnet/executor.hpp>

#include <utility>
#include <cassert>

namespace asyncnet {
    template<typename Executor>
    class executor_work_guard {
    public:
        static_assert(is_executor<Executor>(), "executor requirement not met");

        using executor_type = Executor;

        explicit executor_work_guard(const Executor &ex) : _ex(ex) { start_work(); }

        executor_work_guard(const executor_work_guard &other) : _ex(other.ex) {
            if (other.owns_work()) start_work();
        }

        executor_work_guard &operator=(const executor_work_guard &other) {
            reset();
            _ex = other.get_executor();
            if (other.owns_work())
                start_work();
            return *this;
        }

        executor_work_guard(executor_work_guard &&other) noexcept
                : _ex(std::move(other._ex)), _owns_work(std::exchange(other._owns_work, false)) {

        }

        executor_work_guard &operator=(executor_work_guard &&other) noexcept {
            using std::swap;
            swap(_ex, other._ex);
            swap(_owns_work, other._owns_work);
            return *this;
        }

        ~executor_work_guard() noexcept { reset(); }

        void reset() {
            if (_owns_work)
                _ex.on_work_finished();
            _owns_work = false;
        }

        bool owns_work() const { return _owns_work; }

        executor_type get_executor() const { return _ex; }

    private:
        void start_work() {
            assert(!owns_work() && "work already started");
            _ex.on_work_started();
            _owns_work = true;
        }

        Executor _ex;
        bool _owns_work{false};
    };

    template<typename Executor, std::enable_if_t<is_executor<Executor>::value, int> = 0>
    executor_work_guard<Executor> make_work(const Executor &ex) {
        return executor_work_guard<Executor>(ex);
    }

    template<typename ExecutionContext, std::enable_if_t<std::is_base_of<execution_context, ExecutionContext>::value, int> = 0>
    executor_work_guard<typename ExecutionContext::executor_type> make_work(const ExecutionContext &context) {
        return executor_work_guard<typename ExecutionContext::executor_type>(context.get_executor());
    }
}

#endif //ASYNCNET_EXECUTOR_WORK_GUARD_HPP
