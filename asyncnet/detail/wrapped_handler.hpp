//
// Created by tsing on 2020/11/5.
//

#ifndef ASYNCNET_DETAIL_WRAPPED_HANDLER_HPP
#define ASYNCNET_DETAIL_WRAPPED_HANDLER_HPP

#include <utility>
#include <memory>
#include <cassert>
#include <asyncnet/detail/intrusive_list_node.hpp>

namespace asyncnet {
namespace detail {

template <typename Signature>
class wrapped_handler;

template <typename Signature>
class handler_base;

template <typename ...Params>
class handler_base<void(Params...)> :
    public intrusive_list_node<handler_base<void(Params...)>>
{
public:
  using signature = void(Params...);

  friend class wrapped_handler<signature>;

  virtual void release() noexcept = 0;

  virtual void invoke(Params ...params) = 0;
};

template <typename Signature, typename T, typename Allocator>
class handler_impl;

template <typename T, typename ProtoAllocator, typename ...Params>
class handler_impl<void(Params...), T, ProtoAllocator> final
    : public handler_base<void(Params...)>, ProtoAllocator
{
public:
  explicit handler_impl(T &&t) : t_(std::move(t)) {}

  using allocator_type = typename std::allocator_traits<ProtoAllocator>::template rebind_alloc<handler_impl>;

  allocator_type get_allocator() const
  {
    return *this;
  }

  void release() noexcept final
  {
    using traits_type = typename std::allocator_traits<allocator_type>;
    allocator_type alloc(get_allocator());
    traits_type::destroy(alloc, this);
    traits_type::deallocate(alloc, typename traits_type::pointer(this), 1);
  }

  void invoke(Params ...args) final
  {
    // Make a copy of the handler and deallocates the allocated memory.
    auto h = std::move(t_);
    release();

    h(static_cast<Params>(args)...);
  }

private:
  T t_;
};

template <typename Signature, typename Function, typename ProtoAllocator>
handler_impl<Signature, std::decay_t<Function>, ProtoAllocator> *
allocate_handler(Function &&function, const ProtoAllocator &alloc)
{
  using impl_type = handler_impl<Signature, std::decay_t<Function>, ProtoAllocator>;
  using traits_type = typename std::allocator_traits<ProtoAllocator>::template rebind_traits<impl_type>;
  typename traits_type::allocator_type a(alloc);

  auto pointer = traits_type::allocate(a, 1);
  try
  {
    auto address = std::addressof(*pointer);
    traits_type::construct(a, address, std::forward<Function>(function));
    return address;
  }
  catch (...)
  {
    traits_type::deallocate(a, pointer, 1);

    // fixme: should I rethrow or just return null?
//    throw;
    return nullptr;
  }
}

template <typename Signature>
class wrapped_handler
{
public:
  template <typename U, typename ProtoAllocator>
  explicit wrapped_handler(U &&u, const ProtoAllocator &alloc)
  {
    using impl_type = handler_impl<Signature, std::decay_t<U>, ProtoAllocator>;
    using traits_type = typename std::allocator_traits<ProtoAllocator>::template rebind_traits<impl_type>;
    typename traits_type::allocator_type a(alloc);

    auto pointer = traits_type::allocate(a, 1);
    try
    {
      auto address = std::addressof(*pointer);
      traits_type::construct(a, address, std::forward<U>(u));
      impl_ = address;
    }
    catch (...)
    {
      traits_type::deallocate(a, pointer, 1);
      throw;
    }
  }

  wrapped_handler(wrapped_handler &&other) noexcept
      : impl_(std::exchange(other.impl_, nullptr)) {}

  wrapped_handler &operator=(wrapped_handler &&other) noexcept
  {
    using std::swap;
    swap(impl_, other.impl_);
    return *this;
  }

  ~wrapped_handler() noexcept
  {
    if (impl_)
      impl_->release();
  }

  template <typename ...Args>
  void invoke(Args &&...args)
  {
    assert(impl_);

    impl_->invoke(std::forward<Args>(args)...);
    impl_ = nullptr;
  }

  bool has_value() const noexcept
  {
    return impl_ != nullptr;
  }

private:
  handler_base<Signature> *impl_;
};
}
}

#endif //ASYNCNET_DETAIL_WRAPPED_HANDLER_HPP
