//
// Created by lince on 11/7/20.
//

#ifndef ASYNCNET_POSIX_REACTOR_H
#define ASYNCNET_POSIX_REACTOR_H

#include <asyncnet/service.hpp>
#include <asyncnet/detail/config.hpp>
#include <asyncnet/detail/wrapped_handler.hpp>

namespace asyncnet {
namespace posix {
namespace detail {

using namespace asyncnet::detail;

class reactor : public execution_context::service
{
public:
  using event_type = uint32_t;

  enum : event_type
  {
    event_read = 0x01,
    event_write = 0x02,
    event_error = 0x04,
  };

  struct per_descriptor_data : intrusive_list_node<per_descriptor_data>
  {
    int fd;
  };

  /// Registers a file descriptor.
  void register_descrptor(int fd, per_descriptor_data &data);

  /// Deregister a file descriptor.
  void deregister_descriptor(per_descriptor_data &data);

  /// Sets handler for an event.
  void set_event_handler(per_descriptor_data &data, event_type event, handler_base<void()> *handler);

  /// Enables an event.
  void enable_event(per_descriptor_data &data, event_type event);

  /// Waits for an event.
  /// This function only returns when events are ready or explicitly interrupted by interrupt().
  void run(intrusive_list<handler_base<void()>> &ready_handlers);

  /// Interrupts run calls.
  void interrupt();

private:
  intrusive_list_node<per_descriptor_data> descriptor_list_;
};

}
}
}

#endif //ASYNCNET_POSIX_REACTOR_H
