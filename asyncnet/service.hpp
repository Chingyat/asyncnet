//
// Created by tsing on 2020/11/5.
//

#ifndef ASYNCNET_SERVICE_HPP
#define ASYNCNET_SERVICE_HPP

#include <asyncnet/detail/intrusive_list_node.hpp>
#include <asyncnet/execution_context.hpp>

namespace asyncnet {

class execution_context::service : public detail::intrusive_list_node<service>
{
  friend execution_context;

public:
  explicit service(execution_context &context) noexcept : index_() {}

  virtual ~service() = default;

private:
  virtual void shutdown() noexcept = 0;

  virtual void notify_fork(fork_event e) {}

  unsigned long index_;
};

} // namespace asyncnet

#endif //ASYNCNET_SERVICE_HPP
