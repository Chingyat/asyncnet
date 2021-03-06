//
// Created by tsing on 2020/11/7.
//

#ifndef ASYNCNET_DETAIL_INTRUSIVE_LIST_NODE_HPP
#define ASYNCNET_DETAIL_INTRUSIVE_LIST_NODE_HPP

#include <iterator>
#include <utility>
#include <cassert>

namespace asyncnet {
namespace detail {

template <typename T>
class intrusive_list;

template <typename T>
class intrusive_list_iterator;

template <typename T>
class intrusive_list_node
{
  friend class intrusive_list<T>;

  friend class intrusive_list_iterator<T>;

public:
  intrusive_list_node(const intrusive_list_node &) = delete;

  intrusive_list_node operator=(const intrusive_list_node &) = delete;


protected:
  constexpr intrusive_list_node() noexcept
      : prev(this),
        next(this)
  {
  }

  ~intrusive_list_node() noexcept
  {
    assert(prev == this && next == this);
  }

  void init() noexcept
  {
    prev = this;
    next = this;
  }

  intrusive_list_node *prev;
  intrusive_list_node *next;
};

}// namespace detail
}// namespace asyncnet

#endif//ASYNCNET_DETAIL_INTRUSIVE_LIST_NODE_HPP
