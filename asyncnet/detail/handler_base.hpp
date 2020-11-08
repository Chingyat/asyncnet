//
// Created by tsing on 2020/11/5.
//

#ifndef ASYNCNET_DETAIL_HANDLER_BASE_HPP
#define ASYNCNET_DETAIL_HANDLER_BASE_HPP

#include <asyncnet/detail/intrusive_list_node.hpp>
#include <cassert>
#include <utility>

namespace asyncnet { namespace detail {

template <typename Signature>
class handler_base;

template <typename... Params>
class handler_base<void(Params...)> : public intrusive_list_node<handler_base<void(Params...)>>
{
public:
  using signature = void(Params...);

  virtual void release() noexcept = 0;

  virtual void invoke(Params... params) = 0;

  template <typename ProtoAllocator, typename Function>
  static handler_base *create(const ProtoAllocator &a, Function &&f);

private:
  template <typename T, typename ProtoAllocator>
  class handler_impl;
};

template <typename... Params>
template <typename T, typename ProtoAllocator>
class handler_base<void(Params...)>::handler_impl final
    : public handler_base<void(Params...)>,
      ProtoAllocator
{
public:
  explicit handler_impl(T &&t) : t_(std::move(t)) {}

  using allocator_type = ProtoAllocator;

  allocator_type get_allocator() const
  {
    return *this;
  }

  void release() noexcept final
  {
    using traits_type = typename std::allocator_traits<allocator_type>::template rebind_traits<handler_impl>;
    typename traits_type::allocator_type alloc(get_allocator());
    traits_type::destroy(alloc, this);
    traits_type::deallocate(alloc, typename traits_type::pointer(this), 1);
  }

  void invoke(Params... args) final
  {
    // Make a copy of the handler and deallocates the allocated memory.
    auto h = std::move(t_);
    release();

    std::move(h)(static_cast<Params>(args)...);
  }

private:
  T t_;
};

template <typename... Params>
template <typename ProtoAllocator, typename Function>
handler_base<void(Params...)> *handler_base<void(Params...)>::create(const ProtoAllocator &a, Function &&f)
{
  using impl_type = handler_impl<std::decay_t<Function>, ProtoAllocator>;
  using traits_type = typename std::allocator_traits<ProtoAllocator>::template rebind_traits<impl_type>;
  typename traits_type::allocator_type alloc(a);

  auto pointer = traits_type::allocate(alloc, 1);
  try
  {
    auto address = std::addressof(*pointer);
    traits_type::construct(alloc, address, std::forward<Function>(f));
    return address;
  }
  catch (...)
  {
    traits_type::deallocate(alloc, pointer, 1);
    throw;
  }
}

}} // namespace asyncnet::detail

#endif //ASYNCNET_DETAIL_HANDLER_BASE_HPP
