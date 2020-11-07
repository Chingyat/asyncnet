//
// Created by tsing on 2020/11/5.
//

#ifndef ASYNCNET_IO_CONTEXT_HPP
#define ASYNCNET_IO_CONTEXT_HPP

#include <asyncnet/detail/config.hpp>
#include <asyncnet/execution_context.hpp>
#include <asyncnet/async_result.hpp>
#include <asyncnet/detail/wrapped_handler.hpp>
#include <asyncnet/detail/intrusive_list.hpp>

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>

namespace asyncnet {

/// models ExecutionContext
class io_context : public execution_context
{
public:
  class executor_type;

  using count_type = unsigned int;

  /// Default constructor.
  ASYNCNET_DECL io_context() noexcept;

  /// Constructor.
  ASYNCNET_DECL  explicit io_context(int concurrency_hint) noexcept;

  /// Destructor.
  ASYNCNET_DECL ~io_context() noexcept;

  /// Returns an executor of the io_context.
  ASYNCNET_DECL executor_type get_executor();

  /// Notify to io_context that the process is forked.
  ASYNCNET_DECL void notify_fork(fork_event e);

  /// Run the io_context event processing loop to execute ready handlers.
  ASYNCNET_DECL count_type poll();

  /// Run the io_context event processing loop to execute one ready handler.
  ASYNCNET_DECL count_type poll_one();

  /// Restart the io_context in preparation for a subsequent run() invocation.
  /// This function is not thread-safe.
  ASYNCNET_DECL void restart();

  /// Run the io_context object's event processing loop.
  ASYNCNET_DECL count_type run();

  /// Run the io_context object's event processing loop to execute at most one handler.
  ASYNCNET_DECL count_type run_one();

  /// Stop the io_context objects' event processing loop.
  ASYNCNET_DECL void stop();

  /// Determine whether the io_context object has been stopped.
  ASYNCNET_DECL bool stopped() const { return stopped_; }

private:

  /// Mutex that guards the completion handler queue.
  mutable std::mutex mutex_;

  /// Used to notify threads that running this io_context.
  std::condition_variable cond_;

  /// Completion handler queue.
  detail::intrusive_list<detail::handler_base<void()>> comp_queue_;

  /// Indicates whether the io_context has been stopped.
  std::atomic_bool stopped_{false};

  /// Number of works in operation.
  std::atomic<count_type> outstanding_work_count_{0};

  struct executor_stack_entry;

  /// Thread local stack of running io_context executors.
  /// This can be used to determine if the current thread is running the io_context.
  ASYNCNET_DECL static detail::intrusive_list<executor_stack_entry> &thread_local_executor_stack();

  /// RAII guard that pushes and pops the thread local executor stack.
  struct run_stack_guard;
};

class io_context::executor_type
{
  friend class io_context;

public:
  /// Copy constructor.
  executor_type(const executor_type &) = default;

  /// Copy assignment.
  executor_type &operator=(const executor_type &) = default;

  /// Returns the io_context.
  io_context &context() const
  {
    assert(context_);
    return *context_;
  }

  /// Indicates whether the calling thread is running the io_context.
  ASYNCNET_DECL bool running_in_this_thread() const;

  /// Submit a function object to the completion queue.
  template <typename Function, typename ProtoAllocator>
  void post(Function &&f, const ProtoAllocator &a) const;

  /// If running_in_this_thread() returns true, then f will be invoked immediately.
  /// Otherwise the function object will be submit to the completion queue.
  template <typename Function, typename ProtoAllocator>
  void dispatch(Function &&f, const ProtoAllocator &a) const;

  /// Notifies the io_context that an asynchronous operation has completed.
  ASYNCNET_DECL void on_work_finished() const noexcept;

  /// Notifies the io_context that an asynchronous operation has started.
  ASYNCNET_DECL void on_work_started() const noexcept;

  /// Equality operator.
  friend bool operator==(const executor_type &x, const executor_type &y) noexcept
  {
    return std::addressof(x.context()) == std::addressof(y.context());
  }

  friend bool operator!=(const executor_type &x, const executor_type &y) noexcept
  {
    return !(x == y);
  }

private:
  /// Creates an executor from the execution context.
  ASYNCNET_DECL explicit executor_type(io_context &context) noexcept;

  /// The io_context that the executor belongs to.
  io_context *context_;
};


}

#include <asyncnet/impl/io_context.hpp>

#ifdef ASYNCNET_HEADER_ONLY

# include <asyncnet/impl/io_context.ipp>

#endif

#endif //ASYNCNET_IO_CONTEXT_HPP
