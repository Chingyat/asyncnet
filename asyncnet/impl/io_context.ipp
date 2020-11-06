//
// Created by tsing on 2020/11/5.
//

#ifndef ASYNCNET_IMPL_IO_CONTEXT_IPP
#define ASYNCNET_IMPL_IO_CONTEXT_IPP

#include <asyncnet/io_context.hpp>

namespace asyncnet {

    io_context::io_context(int concurrency_hint) {
        (void)concurrency_hint;
    }

    io_context::io_context() : io_context(3) {}

    io_context::~io_context() noexcept = default;


    io_context::count_type io_context::poll_one() {
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
        count_type  n{0};

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
        new (this) io_context();
    }

    io_context::executor_type io_context::get_executor() {
        return executor_type(*this);
    }


    io_context::executor_type::executor_type(io_context &context) noexcept
        : _context(&context) {

    }

}

#endif //ASYNCNET_IMPL_IO_CONTEXT_IPP
