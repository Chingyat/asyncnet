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

/// Base class for all execution contexts.
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

  /// Constructor.
  execution_context() = default;

  /// Disable copy.
  execution_context(const execution_context &) = delete;
  execution_context &operator=(const execution_context) = delete;


  enum fork_event
  {

  };

  /// Notify the services that the process has been forked.
  ASYNCNET_DECL void notify_fork(fork_event e);

protected:
  /// Invokes shutdown() followed by destroy().
  ASYNCNET_DECL ~execution_context() noexcept;

  /// Invokes svc->shutdown() for all services in reverse order of addition.
  ASYNCNET_DECL void shutdown() noexcept;

  /// Deletes all services in reverse order of addition.
  ASYNCNET_DECL void destroy() noexcept;

private:
  /// Sets service index.
  ASYNCNET_DECL static void set_service_index(service &svc, unsigned long index);

  /// Returns service indices.
  ASYNCNET_DECL static unsigned long get_service_index(const service &svc);

  /// Use recursive_mutex to allow service constructors to add other services to the execution context.
  mutable std::recursive_mutex mutex_;

  /// List of services.
  detail::intrusive_list<service> services_;
};

}

#include <asyncnet/impl/execution_context.hpp>
#ifdef ASYNCNET_HEADER_ONLY
# include <asyncnet/impl/execution_context.ipp>
#endif

#endif
