//
// Created by tsing on 2020/11/5.
//

#ifndef ASYNCNET_SERVICE_HPP
#define ASYNCNET_SERVICE_HPP

#include <asyncnet/execution_context.hpp>
#include <asyncnet/detail/intrusive_list_node.hpp>

namespace asyncnet {

class execution_context::service : public detail::intrusive_list_node<service>
{
  friend execution_context;

public:
  explicit service(execution_context &context) {}

  virtual ~service() = default;

private:
  virtual void shutdown() noexcept = 0;

  virtual void notify_fork(fork_event e) {}

  unsigned long index_;
};

}

#endif //ASYNCNET_SERVICE_HPP
