//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_SYSTEM_CONTEXT_HPP
#define ASYNCNET_SYSTEM_CONTEXT_HPP

#include <asyncnet/detail/config.hpp>
#include <asyncnet/execution_context.hpp>
#include <asyncnet/executor_work_guard.hpp>
#include <asyncnet/io_context.hpp>

#include <atomic>
#include <thread>
#include <vector>

namespace asyncnet {

class system_executor;

class system_context : public execution_context
{
  friend system_executor;

public:
  using executor_type = system_executor;

  /// Returns an system_executor.
  ASYNCNET_DECL executor_type get_executor();

  /// Waits for all threads in the system thread pool to join.
  ASYNCNET_DECL void join();

  /// Stops the system thread pool.
  ASYNCNET_DECL void stop();

  /// Indicates whether stop() has been called.
  ASYNCNET_DECL bool stopped() const;

  /// Destructor.
  ASYNCNET_DECL ~system_context() noexcept;

  /// Constructor.
  ASYNCNET_DECL system_context();

private:
  /// Number of threads in the system thread pool.
  static const std::size_t num_threads = 1;

  /// io_context used for dispatching works.
  io_context work_io_context_;

  /// Threads.
  std::vector<std::thread> work_threads_;

  /// Prevents work_io_context_ from exiting.
  executor_work_guard<io_context::executor_type> work_;
};

} // namespace asyncnet

#ifdef ASYNCNET_HEADER_ONLY
#  include <asyncnet/impl/system_context.ipp>
#endif

#endif //ASYNCNET_SYSTEM_CONTEXT_HPP
