//
// Created by tsing on 2020/11/7.
//

#ifndef ASYNCNET_DETAIL_INTRUSIVE_LIST_NODE_HPP
#define ASYNCNET_DETAIL_INTRUSIVE_LIST_NODE_HPP

#include <utility>
#include <iterator>

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
  intrusive_list_node() noexcept
  {
    reinit_list_node();
  }

  void reinit_list_node() noexcept
  {
    prev = static_cast<T *>(this);
    next = static_cast<T *>(this);
  }


  T *prev;
  T *next;
};

}
}

#endif //ASYNCNET_DETAIL_INTRUSIVE_LIST_NODE_HPP
