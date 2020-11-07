//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_ASSOCIATED_EXECUTOR_HPP
#define ASYNCNET_ASSOCIATED_EXECUTOR_HPP

#include <asyncnet/system_executor.hpp>

#include <type_traits>

namespace asyncnet {

template <typename T, typename Executor = system_executor, typename = std::void_t<>>
struct associated_executor
{
  using type = Executor;

  static type get(const T &t, const Executor &ex = Executor())
  {
    (void) t;
    return ex;
  }
};

template <typename T, typename Executor>
struct associated_executor<T, Executor, std::void_t<typename T::executor_type, decltype(std::declval<const T &>().get_executor())>>
{
  using type = typename T::executor_type;

  static type get(const T &x, const Executor & = Executor())
  {
    return x.get_executor();
  }
};

template <typename T, typename Executor = system_executor>
using associated_executor_t = typename associated_executor<T, Executor>::type;

template <typename T, typename Executor = system_executor>
associated_executor_t<T, Executor> get_associated_executor(const T &t, const Executor &ex = Executor())
{
  return associated_executor<T, Executor>::get(t, ex);
}

} // namespace asyncnet

#endif //ASYNCNET_ASSOCIATED_EXECUTOR_HPP
