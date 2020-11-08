//
// Created by lince on 11/8/20.
//

#ifndef ASYNCNET_EXECUTOR_BINDER_HPP
#define ASYNCNET_EXECUTOR_BINDER_HPP

#include <asyncnet/detail/config.hpp>
#include <asyncnet/executor.hpp>
#include <asyncnet/execution_context.hpp>


namespace asyncnet {

template <
    typename T,
    typename Executor>
class executor_binder {
public:
  using executor_type = Executor;
  using target_type = T;

  template <typename U>
  executor_binder(
      executor_arg_t,
      const executor_type &e, U&& u)
      : e_(e),
        t_(std::forward<U>(u))
  {
  }

  executor_binder(
      const executor_binder &other)
      = default;

  executor_binder(
      executor_arg_t,
      const executor_type &e,
      const executor_binder &other)
      : e_(e),
        t_(other.t_)
  {
  }

  template<
      typename U,
      typename OtherExecutor>
  executor_binder(
      const executor_binder< U, OtherExecutor > & other)
      : e_(other.get_executor()),
        t_(other.t_)
  {
  }

  template<
      typename U,
      typename OtherExecutor>
  executor_binder(
      executor_arg_t ,
      const executor_type & e,
      const executor_binder< U, OtherExecutor > & other)
      : e_(e),
        t_(other.t_)
  {
  }

  executor_binder(
      executor_binder && other)
      = default;

  executor_binder(
      executor_arg_t ,
      const executor_type & e,
      executor_binder && other)
      : e_(e),
        t_(std::move(other.t_))
  {
  }

  template<
      typename U,
      typename OtherExecutor>
  executor_binder(
      executor_binder< U, OtherExecutor > && other)
      : e_(other.e_),
        t_(std::move(other.t_))
  {
  }

  template<
      typename U,
      typename OtherExecutor>
  executor_binder(
      executor_arg_t ,
      const executor_type & e,
      executor_binder< U, OtherExecutor > && other)
      : e_(e),
        t_(std::move(other.t_))
  {
  }

  ~executor_binder() = default;

  target_type &get() { return t_; }

  const target_type& get() const {return t_;}

  executor_type get_executor() const { return e_;}

  template <
      typename ...Args>
  auto operator()(
      Args&& ...args) -> decltype(auto)
  {
    return t_(std::forward<Args>(args)...);
  }

  template <
      typename ...Args>
  auto operator()(
      Args&& ...args) const -> decltype(auto)
  {
    return t_(std::forward<Args>(args)...);
  }

private:
  executor_type e_;
  target_type t_;
};


template<
    typename Executor,
    typename T>
executor_binder< typename std::decay< T >::type, Executor > bind_executor(
    const Executor & ex,
    T && t,
    typename std::enable_if< is_executor< Executor >::value>::type *  = 0)
{
  return { executor_arg, ex, std::forward<T>(t) };
}

template<
    typename ExecutionContext,
    typename T>
executor_binder< typename std::decay< T >::type, typename ExecutionContext::executor_type > bind_executor(
    ExecutionContext & ctx,
    T && t,
    typename std::enable_if< std::is_convertible< ExecutionContext &, execution_context & >::value >::type *  = 0)
{
  return { executor_arg, ctx.get_executor(), std::forward<T>(t) };
}

}

#endif // ASYNCNET_EXECUTOR_BINDER_HPP
