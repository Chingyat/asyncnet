//
// Created by lince on 11/8/20.
//

#ifndef ASYNCNET_DETAIL_TSS_PTR_HPP
#define ASYNCNET_DETAIL_TSS_PTR_HPP

#include <asyncnet/detail/config.hpp>

namespace asyncnet {
namespace detail {

template <typename T, typename Tag>
class tss_ptr
{
public:
  tss_ptr() = default;

  ~tss_ptr() = default;

  T &operator*() const
  {
    return *ptr_;
  }

  T *operator->() const
  {
    return ptr_;
  }

  operator T *() const
  {
    return ptr_;
  }

  tss_ptr &operator=(T *value)
  {
    ptr_ = value;
    return *this;
  }

private:
#ifdef __GNUC__
  static __thread T *ptr_;
#else
  static thread_local T *ptr_;
#endif
};

#ifdef __GNUC__
template <typename T, typename Tag>
__thread T *tss_ptr<T, Tag>::ptr_;
#else
thread_local T *tss_ptr<T, Tag>::ptr_;
#endif

}// namespace detail
}// namespace asyncnet

#endif//ASYNCNET_DETAIL_TSS_PTR_HPP
