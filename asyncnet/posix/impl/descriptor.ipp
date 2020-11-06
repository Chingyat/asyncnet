//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_POSIX_IMPL_DESCRIPTOR_IPP
#define ASYNCNET_POSIX_IMPL_DESCRIPTOR_IPP

#include <asyncnet/posix/descriptor.hpp>

#include <fcntl.h>

namespace asyncnet {
namespace posix {
namespace detail {

void descriptor_service::construct(implementation_type &impl)
{
  impl = implementation_type();
}

void descriptor_service::destroy(implementation_type &impl)
{
  if (is_open(impl))
  {
    close(impl);
  }
}

void descriptor_service::move_construct(implementation_type &impl, implementation_type &other) noexcept
{
  impl = implementation_type(std::move(other));
  other = implementation_type();
}

// fixme: other service
void descriptor_service::move_assign(implementation_type &impl, implementation_type &other) noexcept
{
  using std::swap;
  swap(impl, other);
}

std::error_code descriptor_service::close(implementation_type &impl)
{
  if (!is_open(impl))
    return std::make_error_code(std::errc::not_connected);

  if (-1 == ::close(impl.fd_))
  {
    return {errno, std::generic_category()};
  }

  impl.fd_ = -1;
  return {};
}

bool descriptor_service::is_open(const implementation_type &impl) const
{
  return impl.fd_ != -1;
}

bool descriptor_service::is_readable(const descriptor_service::implementation_type &impl) const
{
  return impl.state_ & state_readable;
}

bool descriptor_service::is_writable(const descriptor_service::implementation_type &impl) const
{
  return impl.state_ & state_writable;
}

std::error_code descriptor_service::assign(descriptor_service::implementation_type &impl,
                                           descriptor_service::native_handle_type fd)
{
  if (is_open(impl))
  {
    if (auto err = close(impl))
      return err;
  }

  impl.fd_ = fd;
  impl.state_ = 0;

  return {};
}

void descriptor_service::shutdown() noexcept
{

}

bool descriptor_service::non_blocking(const descriptor_service::implementation_type &impl) const
{
  int r = ::fcntl(get_native_handle(impl), F_GETFL);
  return r & O_NONBLOCK;
}

descriptor_service::native_handle_type
descriptor_service::get_native_handle(const descriptor_service::implementation_type &impl) const
{
  return impl.fd_;
}

std::error_code
descriptor_service::set_nonblocking(descriptor_service::implementation_type &impl, bool mode)
{
  auto fd = get_native_handle(impl);
  auto flag = ::fcntl(fd, F_GETFL);
  if (mode)
    flag |= O_NONBLOCK;
  else
    flag &= ~O_NONBLOCK;
  if (-1 == ::fcntl(fd, F_SETFL, flag))
    return {errno, std::generic_category()};
  return {};
}

}
}
}

#endif //ASYNCNET_POSIX_IMPL_DESCRIPTOR_IPP
