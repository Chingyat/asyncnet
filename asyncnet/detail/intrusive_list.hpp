//
// Created by tsing on 2020/11/7.
//

#ifndef ASYNCNET_DETAIL_INTRUSIVE_LIST_HPP
#define ASYNCNET_DETAIL_INTRUSIVE_LIST_HPP

#include <asyncnet/detail/intrusive_list_node.hpp>
#include <asyncnet/detail/intrusive_list_iterator.hpp>

#include <type_traits>
#include <cassert>

namespace asyncnet {
namespace detail {

template <typename T>
class intrusive_list
{
public:
  using value_type = std::decay_t<T>;
  using reference = T &;
  using node_type = T;
  using pointer = T *;
  using node_ptr = intrusive_list_node<T> *;
  using iterator = intrusive_list_iterator<T>;
  using const_iterator = intrusive_list_iterator<std::add_const_t<T>>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;


  constexpr intrusive_list() noexcept = default;

  intrusive_list(const intrusive_list &&) = delete;

  intrusive_list &operator=(const intrusive_list &) = delete;

  iterator begin() noexcept
  {
    return iterator{head_.next};
  }

  iterator end() noexcept
  {
    return iterator{&head_};
  }

  const_iterator begin() const noexcept
  {
    return const_iterator{head_.next};
  }

  const_iterator end() const noexcept
  {
    return const_iterator{&head_};
  }

  const_iterator cbegin() const noexcept
  {
    return begin();
  }

  const_iterator cend() const noexcept
  {
    return end();
  }

  reverse_iterator rbegin() noexcept
  {
    return reverse_iterator(end());
  }

  reverse_iterator rend() noexcept
  {
    return reverse_iterator(begin());
  }

  const_reverse_iterator rbegin() const noexcept
  {
    return const_reverse_iterator(end());
  }

  const_reverse_iterator rend() const noexcept
  {
    return const_reverse_iterator(end());
  }

  const_reverse_iterator crbegin() const noexcept
  {
    return rbegin();
  }

  const_reverse_iterator crend() const noexcept
  {
    return rend();
  }

  /// Indicates whether the list is empty.
  /// Complexity: constant.
  bool empty() const noexcept
  {
    return head_.next == &head_;
  }

  void push_front(reference node) noexcept
  {
    delete_entry(&node);
    insert_between(&node, &head_, head_.next);
  }

  void push_back(reference node) noexcept
  {
    delete_entry(&node);
    insert_between(&node, head_.prev, &head_);
  }

  void pop_front()
  {
    erase(front());
  }

  void pop_back()
  {
    erase(back());
  }

  static void replace(reference old, reference node) noexcept
  {
    node.next = old.next;
    node.next->prev = &node;
    node.prev = old.prev;
    node.prev->next = &node;

    old.reinit_list_node();
  }

  void erase(reference node)
  {
    delete_entry(&node);
    node.reinit_list_node();
  }

  void erase(iterator pos)
  {
    erase(*pos);
  }

  reference back() const
  {
    assert(!empty());
    return *pointer(head_.prev);
  }

  reference front() const
  {
    assert(!empty());
    return *pointer(head_.next);
  }

  /// Tests whether the list has exactly one elements.
  /// Complexity: constant.
  bool is_singular() const noexcept
  {
    return !empty() && (head_.next == head_.prev);
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
  intrusive_list_node<T> head_;
};

}
}

#endif //ASYNCNET_DETAIL_INTRUSIVE_LIST_HPP
