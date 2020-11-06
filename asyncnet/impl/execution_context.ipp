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

}

void execution_context::shutdown() noexcept
{
  std::vector<service *> services;

  {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    std::transform(services_.crbegin(), services_.crend(),
                   std::back_inserter(services),
                   [](const std::pair<std::unique_ptr<service>, service_index_type> &svc)
                   {
                     return svc.first.get();
                   });
  }

  std::for_each(services.cbegin(), services.cend(), [](service *svc)
  {
    svc->shutdown();
  });
}


void execution_context::destroy() noexcept
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  while (!services_.empty())
    services_.pop_back();
}


void execution_context::notify_fork(execution_context::fork_event e)
{
  // todo
}

}

#endif //ASYNCNET_IMPL_EXECUTION_CONTEXT_IPP
