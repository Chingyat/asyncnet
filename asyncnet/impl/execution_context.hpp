//
// Created by tsing on 2020/11/6.
//

#ifndef ASYNCNET_IMPL_EXECUTION_CONTEXT_HPP
#define ASYNCNET_IMPL_EXECUTION_CONTEXT_HPP

#include <asyncnet/execution_context.hpp>
#include <asyncnet/detail/key_index.hpp>

#include <algorithm>

namespace asyncnet {

template <typename Service, typename ...Args>
Service &make_service(execution_context &context, Args &&...args)
{
  std::unique_lock<std::recursive_mutex> lock(context.mutex_);

  auto const iter = std::find_if(context.services_.cbegin(), context.services_.cend(),
                                 [](const execution_context::service &svc)
                                 {
                                   return execution_context::get_service_index(svc) ==
                                          detail::key_index<typename Service::key_type>();
                                 });

  if (iter != context.services_.cend())
    throw service_already_exist();

  auto svc = new Service(context, std::forward<Args>(args)...);
  auto &ret = *svc;
  context.services_.push_back(ret);

  return ret;
}

template <typename Service>
bool has_service(const execution_context &context)
{
  std::lock_guard<std::recursive_mutex> lock(context.mutex_);

  auto const iter = std::find_if(context.services_.cbegin(), context.services_.cend(),
                                 [](const execution_context::service &svc)
                                 {
                                   return execution_context::get_service_index(svc) ==
                                          detail::key_index<typename Service::key_type>();
                                 });

  return iter != context.services_.cend();
}

template <typename Service>
Service &use_service(execution_context &context)
{
  std::unique_lock<std::recursive_mutex> lock(context.mutex_);

  auto const iter = std::find_if(context.services_.begin(), context.services_.end(),
                                 [](const execution_context::service &svc)
                                 {
                                   return execution_context::get_service_index(svc) ==
                                          detail::key_index<typename Service::key_type>();
                                 });
  if (iter != context.services_.end())
    return static_cast<Service &>( *iter );

  auto svc = new Service(context);
  auto &ret = *svc;

  context.services_.push_back(ret);
  return ret;
}


}

#endif //ASYNCNET_IMPL_EXECUTION_CONTEXT_HPP
