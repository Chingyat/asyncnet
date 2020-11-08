//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_IMPL_SYSTEM_CONTEXT_IPP
#define ASYNCNET_IMPL_SYSTEM_CONTEXT_IPP

#include <asyncnet/executor_work_guard.hpp>
#include <asyncnet/system_context.hpp>
#include <asyncnet/system_executor.hpp>

#include <algorithm>

namespace asyncnet {

system_context::system_context() : work_(work_io_context_.get_executor())
{
  std::generate_n(std::back_inserter(work_threads_), num_threads,
                  [this] { return std::thread([this] { work_io_context_.run(); }); });
}

system_context::~system_context() noexcept
{
  stop();
  join();
}

void system_context::stop()
{
  work_.reset();
  work_io_context_.stop();
}

void system_context::join()
{
  for (auto &t : work_threads_)
    t.join();
}

bool system_context::stopped() const
{
  return work_io_context_.stopped();
}

system_context::executor_type system_context::get_executor()
{
  return {};
}

system_context &system_context::get_system_context()
{
  static system_context i;
  return i;
}

} // namespace asyncnet

#endif //ASYNCNET_IMPL_SYSTEM_CONTEXT_IPP
