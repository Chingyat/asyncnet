//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_SYSTEM_CONTEXT_HPP
#define ASYNCNET_SYSTEM_CONTEXT_HPP

#include <asyncnet/execution_context.hpp>
#include <asyncnet/io_context.hpp>
#include <asyncnet/executor_work_guard.hpp>

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
  executor_type get_executor();

  /// Waits for all threads in the system thread pool to join.
  void join();

  /// Stops the system thread pool.
  void stop();

  /// Indicates whether stop() has been called.
  bool stopped() const;

  /// Destructor.
  ~system_context() noexcept;

private:
  /// Constructor.
  system_context();

  /// Returns the system_context singleton.
  static system_context &get_system_context();

  /// Number of threads in the system thread pool.
  static const std::size_t num_threads = 4;

  /// io_context used for dispatching works.
  io_context work_io_context_;

  /// Threads.
  std::vector<std::thread> threads_;

  /// Prevents work_io_context_ from exiting.
  executor_work_guard<io_context::executor_type> work_;
};

}

#endif //ASYNCNET_SYSTEM_CONTEXT_HPP
