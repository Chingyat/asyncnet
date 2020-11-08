//
// Created by lince on 11/8/20.
//

#ifndef ASYNCNET_DETAIL_GLOBAL_HPP
#define ASYNCNET_DETAIL_GLOBAL_HPP

#include <condition_variable>
#include <mutex>
#include <thread>

namespace asyncnet {
namespace detail {

template <typename T>
class global_impl
{
public:
  static void do_init()
  {
    instance_.ptr_ = new T();
  }

  ~global_impl()
  {
    delete ptr_;
  }

  static std::once_flag once_flag_;
  static global_impl instance_;
  T *ptr_;
};

template <typename T>
std::once_flag global_impl<T>::once_flag_;

template <typename T>
global_impl<T> global_impl<T>::instance_;

template <typename T>
T &global()
{
  std::call_once(
      global_impl<T>::once_flag_,
      &global_impl<T>::do_init);
  return *global_impl<T>::instance_.ptr_;
}

} // namespace detail
} // namespace asyncnet

#endif //ASYNCNET_DETAIL_GLOBAL_HPP
