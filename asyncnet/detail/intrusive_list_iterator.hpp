//
// Created by lince on 11/7/20.
//

#ifndef ASYNCNET_DETAIL_INTRUSIVE_LIST_ITERATOR_HPP
#define ASYNCNET_DETAIL_INTRUSIVE_LIST_ITERATOR_HPP

#include <asyncnet/detail/intrusive_list_node.hpp>

namespace asyncnet {
namespace detail {

template <typename T>
class intrusive_list_iterator
{
public:
  using value_type = T;
  using reference = T &;
  using pointer = T *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;

  constexpr intrusive_list_iterator() noexcept: data_() {}

  explicit constexpr intrusive_list_iterator(T *p) noexcept: data_(p) {}

  intrusive_list_iterator(const intrusive_list_iterator &) = default;

  intrusive_list_iterator &operator=(const intrusive_list_iterator &) = default;

  reference operator*() const
  {
    return *data_;
  }

  pointer operator->() const
  {
    return static_cast<pointer>(data_);
  }

  intrusive_list_iterator &operator++()
  {
    data_ = data_->next;
    return *this;
  }

  intrusive_list_iterator &operator--()
  {
    data_ = data_->prev;
    return *this;
  }

  intrusive_list_iterator operator++(int)
  {
    auto ret = *this;
    ++*this;
    return ret;
  }

  intrusive_list_iterator operator--(int)
  {
    auto ret = *this;
    --*this;
    return ret;
  }

  friend bool operator==(const intrusive_list_iterator<T> &x, const intrusive_list_iterator<T> &y) noexcept
  {
    return x.data_ == y.data_;
  }

  friend bool operator!=(const intrusive_list_iterator<T> &x, const intrusive_list_iterator<T> &y) noexcept
  {
    return x.data_ != y.data_;
  }

private:
  T *data_;
};

}
}

#endif //ASYNCNET_DETAIL_INTRUSIVE_LIST_ITERATOR_HPP
