//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_IMPL_SYSTEM_CONTEXT_IPP
#define ASYNCNET_IMPL_SYSTEM_CONTEXT_IPP

#include <asyncnet/system_context.hpp>
#include <asyncnet/system_executor.hpp>
#include <asyncnet/executor_work_guard.hpp>

#include <algorithm>

namespace asyncnet {

    system_context::system_context() : _work(_work_io_context.get_executor()) {
        std::generate_n(std::back_inserter(_threads), num_threads, [this] {
            return std::thread([this] { _work_io_context.run(); });
        });
    }

    system_context::~system_context() noexcept {
        stop();
        join();
    }

    void system_context::stop() {
        _work.reset();
        _work_io_context.stop();
    }

    void system_context::join() {
        for (auto &t : _threads)
            t.join();
    }

    bool system_context::stopped() const {
        return _work_io_context.stopped();
    }

    system_context::executor_type system_context::get_executor() {
        return {};
    }

    system_context &system_context::get_system_context() {
        static system_context i;
        return i;
    }
}

#endif //ASYNCNET_IMPL_SYSTEM_CONTEXT_IPP
