#ifndef ASYNCNET_EXECUTION_CONTEXT_HPP
#define ASYNCNET_EXECUTION_CONTEXT_HPP

#include <asyncnet/detail/config.hpp>
#include <asyncnet/detail/intrusive_list.hpp>
#include <memory>
#include <vector>
#include <mutex>
#include <exception>

namespace asyncnet {

class execution_context;

class service_already_exist : public std::logic_error
{
public:
  service_already_exist()
      : std::logic_error("service_already_exist") {}
};

template <typename Service>
Service &use_service(execution_context &context);

template <typename Service, typename ...Args>
Service &make_service(execution_context &context, Args &&...args);

template <typename Service>
bool has_service(const execution_context &context);


class execution_context
{
  template <typename Service>
  friend Service &use_service(execution_context &context);

  template <typename Service, typename ...Args>
  friend Service &make_service(execution_context &context, Args &&...args);

  template <typename Service>
  friend bool has_service(const execution_context &context);

public:
  class service;

  execution_context() = default;

  execution_context(const execution_context &) = delete;

  execution_context &operator=(const execution_context) = delete;


  enum fork_event
  {

  };

  ASYNCNET_DECL void notify_fork(fork_event e);

protected:
  ~execution_context() noexcept;

  ASYNCNET_DECL void shutdown() noexcept;

  ASYNCNET_DECL void destroy() noexcept;

private:
  ASYNCNET_DECL static void set_service_index(service &svc, unsigned long index);
  ASYNCNET_DECL static unsigned long get_service_index(const service &svc);

  mutable std::recursive_mutex mutex_;
  detail::intrusive_list<service> services_;
};

}

#include <asyncnet/impl/execution_context.hpp>

#endif
