//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_IMPL_SYSTEM_EXECUTOR_IPP
#define ASYNCNET_IMPL_SYSTEM_EXECUTOR_IPP

#include <asyncnet/system_context.hpp>
#include <asyncnet/system_executor.hpp>
#include <asyncnet/detail/global.hpp>

namespace asyncnet {

system_context &system_executor::context() const
{
  return detail::global<system_context>();
}

void system_executor::on_work_finished() const noexcept
{
  context().work_io_context_.get_executor().on_work_finished();
}

void system_executor::on_work_started() const noexcept
{
  context().work_io_context_.get_executor().on_work_started();
}

} // namespace asyncnet

#endif //ASYNCNET_IMPL_SYSTEM_EXECUTOR_IPP
