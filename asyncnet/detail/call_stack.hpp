//
// Created by lince on 11/8/20.
//

#ifndef ASYNCNET_DETAIL_CALL_STACK_HPP
#define ASYNCNET_DETAIL_CALL_STACK_HPP

#include <asyncnet/detail/intrusive_list.hpp>
#include <asyncnet/detail/tss_ptr.hpp>

#include <algorithm>

namespace asyncnet {
namespace detail {

template <typename Key>
class call_stack
{
public:
  static_assert(std::is_copy_constructible<Key>::value && std::is_copy_assignable<Key>::value,
                "key must be copyable");

  class frame
  {
  public:
    explicit frame(Key key)
        : key_(key),
          next_(call_stack<Key>::top())
    {
      top_ = this;
    }

    ~frame() noexcept
    {
      top_ = next_;
    }

    Key key() const
    {
      return key_;
    }

    frame *next() const
    {
      return next_;
    }

  private:
    Key key_;
    frame *next_;
  };

  static frame *top()
  {
    return top_;
  }

  static bool contains(Key key)
  {
    frame *elem = top_;

    while (elem)
    {
      if (elem->key() == key)
        return true;

      elem = elem->next();
    }

    return false;
  }

private:
  static tss_ptr<frame, call_stack> top_;
};

template <typename Key>
tss_ptr<typename call_stack<Key>::frame, call_stack<Key>> call_stack<Key>::top_;

}// namespace detail
}// namespace asyncnet

#endif//ASYNCNET_DETAIL_CALL_STACK_HPP
