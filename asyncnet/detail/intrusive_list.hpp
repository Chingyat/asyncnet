//
// Created by tsing on 2020/11/7.
//

#ifndef ASYNCNET_DETAIL_INTRUSIVE_LIST_HPP
#define ASYNCNET_DETAIL_INTRUSIVE_LIST_HPP

#include <asyncnet/detail/intrusive_list_node.hpp>

#include <type_traits>
#include <cassert>

namespace asyncnet {
namespace detail {

template <typename T>
class intrusive_list
{
public:
  using node_type = T;
  using pointer = T *;
  using node_ptr = intrusive_list_node <T> *;
  using iterator = intrusive_list_iterator<T>;

  static_assert(std::is_base_of<intrusive_list_node < T>, T > ::value,
                "list node must inherits from intrusive_list_node<T>");

  constexpr intrusive_list() noexcept = default;

  intrusive_list(const intrusive_list &&) = delete;

  intrusive_list &operator=(const intrusive_list &) = delete;

  iterator begin()
  {
    return iterator{head_.next};
  }

  iterator end()
  {
    return iterator{&head_};
  }

  void push_front(pointer node) noexcept
  {
    insert_between(node, &head_, head_.next);
  }

  void push_back(pointer node) noexcept
  {
    insert_between(node, head_.prev, &head_);
  }

  static void remove(pointer node) noexcept
  {
    delete_entry(node);
    node->prev = poison;
    node->next = poison;
  }

  static void replace(pointer old, pointer node) noexcept
  {
    node->next = old->next;
    node->next->prev = node;
    node->prev = old->prev;
    node->prev->next = node;
  }

  static void replace_init(pointer old, pointer node) noexcept
  {
    replace(old, node);
    old->reinit_list_node();
  }

  static void remove_init(pointer node) noexcept
  {
    delete_entry(node);
    node->reinit_list_node();
  }

  void remove_and_push_back(pointer list) noexcept
  {
    delete_entry(list);
    push_back(list);
  }

  void remove_and_push_front(pointer list) noexcept
  {
    delete_entry(list);
    push_front(list);
  }

  pointer back() const noexcept
  {
    assert(!empty());
    return pointer(head_.prev);
  }

  pointer front() const noexcept
  {
    assert(!empty());
    return pointer(head_.next);
  }

  bool empty() const noexcept
  {
    return head_.next == &head_;
  }

  void move_front_to_back() noexcept
  {
    if (!empty())
    {
      auto first = front();
      remove_and_push_back(first);
    }
  }

  void move_back_to_front() noexcept
  {
    if (!empty())
    {
      auto last = back();
      remove_and_push_front(last);
    }
  }

  bool is_singular() const noexcept
  {
    return !empty() && (head_.next == head_.prev);
  }

  void move_front_nodes_to(intrusive_list &list, pointer entry)
  {
    if (empty())
      return;

    if (is_singular() && front() != entry && &head_ != entry)
      return;

    if (entry == &head_)
    {
      list.head_.reinit_list_node();
      return;
    }

    pointer new_first = entry->next;
    list->next = front();
    list->next->prev = list;
    list->prev = entry;
    entry->next = list;
    front() = new_first;
    new_first->prev = &head_;
  }

private:

  static void insert_between(node_ptr node, node_ptr prev, node_ptr next) noexcept
  {
    next->prev = node;
    node->next = next;
    node->prev = prev;
    prev->next = node;
  }

  static void delete_between(node_ptr prev, node_ptr next) noexcept
  {
    next->prev = prev;
    prev->next = next;
  }

  static void delete_entry(node_ptr entry) noexcept
  {
    delete_between(entry->prev, entry->next);
  }

  // Use next as the head and prev as the tail.
  intrusive_list_node <T> head_;

  static const pointer poison;
};

template <typename T>
const typename intrusive_list<T>::pointer intrusive_list<T>::poison = reinterpret_cast<pointer>(0xdeaddead);


}
}

#endif //ASYNCNET_DETAIL_INTRUSIVE_LIST_HPP
