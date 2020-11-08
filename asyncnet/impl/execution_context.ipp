//
// Created by tsing on 2020/11/6.
//

#ifndef ASYNCNET_IMPL_EXECUTION_CONTEXT_IPP
#define ASYNCNET_IMPL_EXECUTION_CONTEXT_IPP

#include <asyncnet/execution_context.hpp>
#include <asyncnet/service.hpp>

#include <algorithm>

namespace asyncnet {

execution_context::~execution_context() noexcept
{
    shutdown();
    destroy();
}

void execution_context::shutdown() noexcept
{
  std::for_each(services_.rbegin(), services_.rend(), [](service &svc) noexcept
  {
    svc.shutdown();
  });
}


void execution_context::destroy() noexcept
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  while (!services_.empty())
  {
    auto &svc = services_.back();
    services_.erase(svc);
    delete std::addressof(svc);
  }
}


void execution_context::notify_fork(execution_context::fork_event e)
{
  // todo
}


void execution_context::set_service_index(execution_context::service &svc, unsigned long index)
{
  assert(svc.index_ == 0);
  svc.index_ = index;
}

unsigned long execution_context::get_service_index(const execution_context::service &svc)
{
  return svc.index_;
}


}

#endif //ASYNCNET_IMPL_EXECUTION_CONTEXT_IPP
