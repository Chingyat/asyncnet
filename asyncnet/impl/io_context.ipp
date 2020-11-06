//
// Created by tsing on 2020/11/5.
//

#ifndef ASYNCNET_IMPL_IO_CONTEXT_IPP
#define ASYNCNET_IMPL_IO_CONTEXT_IPP

#include <asyncnet/io_context.hpp>

namespace asyncnet {

    io_context::io_context(int concurrency_hint) {
        (void) concurrency_hint;
    }

    io_context::io_context() : io_context(3) {}

    io_context::~io_context() noexcept = default;

    std::vector<io_context::executor_type> &io_context::_thread_run_stack() {
        static thread_local std::vector<io_context::executor_type> stack;
        return stack;
    }


    struct io_context::run_stack_guard {
        explicit run_stack_guard(io_context &context) {
            _thread_run_stack().push_back(context.get_executor());
        }

        ~run_stack_guard() noexcept {
            _thread_run_stack().pop_back();
        }
    };

    io_context::count_type io_context::poll_one() {
        run_stack_guard run_guard(*this);

        std::unique_lock<std::mutex> lock(_mutex);
        if (_compq.empty())
            return 0;

        auto h = std::move(_compq.front());
        _compq.pop_front();
        lock.unlock();

        h.invoke();
        return 1;
    }

    io_context::count_type io_context::poll() {
        count_type n{0};
        while (poll_one())
            ++n;
        return n;
    }

    io_context::count_type io_context::run_one() {
        run_stack_guard run_guard(*this);

        std::unique_lock<std::mutex> lock(_mutex);
        _cond.wait(lock, [&] {
            return !_compq.empty() || _stopped || _work_count == 0;
        });

        if (_compq.empty())
            return 0;

        auto h = std::move(_compq.front());
        _compq.pop_front();
        lock.unlock();

        h.invoke();
        return 1;
    }


    io_context::count_type io_context::run() {
        count_type n{0};

        while (run_one())
            ++n;

        return n;
    }

    void io_context::stop() {
        _stopped = true;
        _cond.notify_all();
    }

    void io_context::restart() {
        this->~io_context();
        new(this) io_context();
    }

    io_context::executor_type io_context::get_executor() {
        return executor_type(*this);
    }


    io_context::executor_type::executor_type(io_context &context) noexcept
            : _context(&context) {

    }

    void io_context::executor_type::on_work_finished() const noexcept {
        if (--context()._work_count == 0) {
            context()._cond.notify_all();
        }
    }

    void io_context::executor_type::on_work_started() const noexcept {
        ++context()._work_count;
    }

    bool io_context::executor_type::running_in_this_thread() const {
        auto &stk = _thread_run_stack();
        return std::find(stk.crbegin(), stk.crend(), *this) != stk.crend();
    }

}

#endif //ASYNCNET_IMPL_IO_CONTEXT_IPP
