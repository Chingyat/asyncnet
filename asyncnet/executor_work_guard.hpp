//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_EXECUTOR_WORK_GUARD_HPP
#define ASYNCNET_EXECUTOR_WORK_GUARD_HPP

#include <asyncnet/executor.hpp>

#include <utility>
#include <cassert>

namespace asyncnet {

template <typename Executor>
class executor_work_guard
{
public:
  static_assert(is_executor<Executor>(), "executor requirement not met");

  using executor_type = Executor;

  explicit executor_work_guard(const Executor &ex) : ex_(ex) { start_work(); }

  executor_work_guard(const executor_work_guard &other) : ex_(other.ex)
  {
    if (other.owns_work()) start_work();
  }

  executor_work_guard &operator=(const executor_work_guard &other)
  {
    reset();
    ex_ = other.get_executor();
    if (other.owns_work())
      start_work();
    return *this;
  }

  executor_work_guard(executor_work_guard &&other) noexcept
      : ex_(std::move(other.ex_)), owns_work_(std::exchange(other.owns_work_, false))
  {
  }

  executor_work_guard &operator=(executor_work_guard &&other) noexcept
  {
    using std::swap;
    swap(ex_, other.ex_);
    swap(owns_work_, other.owns_work_);
    return *this;
  }

  ~executor_work_guard() noexcept { reset(); }

  void reset()
  {
    if (owns_work_)
      ex_.on_work_finished();
    owns_work_ = false;
  }

  bool owns_work() const { return owns_work_; }

  executor_type get_executor() const { return ex_; }

private:
  void start_work()
  {
    assert(!owns_work() && "work already started");
    ex_.on_work_started();
    owns_work_ = true;
  }

  Executor ex_;
  bool owns_work_{false};
};

template <typename Executor, std::enable_if_t<is_executor<Executor>::value, int> = 0>
executor_work_guard<Executor> make_work(const Executor &ex)
{
  return executor_work_guard<Executor>(ex);
}

template <typename ExecutionContext, std::enable_if_t<std::is_base_of<execution_context, ExecutionContext>::value, int> = 0>
executor_work_guard<typename ExecutionContext::executor_type> make_work(ExecutionContext &context)
{
  return executor_work_guard<typename ExecutionContext::executor_type>(context.get_executor());
}

}

#endif //ASYNCNET_EXECUTOR_WORK_GUARD_HPP
