//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_ASSOCIATED_ALLOCATOR_HPP
#define ASYNCNET_ASSOCIATED_ALLOCATOR_HPP

#include <type_traits>

namespace asyncnet {

template <typename T, typename Allocator = std::allocator<void>, typename = std::void_t<>>
struct associated_allocator
{
  using type = Allocator;

  static type get(const T &t, const Allocator &a = Allocator())
  {
    (void) t;
    return a;
  };
};

template <typename T, typename Allocator>
struct associated_allocator<T,
                            Allocator,
                            std::void_t<typename T::allocator_type, decltype(std::declval<const T &>().get_allocator())>>
{
  using type = typename T::allocator_type;

  static type get(const T &t, const Allocator &a = Allocator())
  {
    (void) a;
    return t.get_allocator();
  }
};


template <typename T, typename Allocator = std::allocator<void>>
using associated_allocator_t = typename associated_allocator<T, Allocator>::type;

template <typename T, typename Allocator = std::allocator<void>>
associated_allocator_t<T, Allocator> get_associated_allocator(const T &t, const Allocator &a = Allocator())
{
  return associated_allocator<T, Allocator>::get(t, a);
}

} // namespace asyncnet

#endif //ASYNCNET_ASSOCIATED_ALLOCATOR_HPP
