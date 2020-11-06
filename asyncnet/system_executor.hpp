//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_SYSTEM_EXECUTOR_HPP
#define ASYNCNET_SYSTEM_EXECUTOR_HPP

#include <asyncnet/execution_context.hpp>
#include <asyncnet/executor.hpp>

namespace asyncnet {

class system_context;

class system_executor
{
public:
  system_executor() = default;

  system_context &context() const;

  template <typename Function, typename ProtoAllocator>
  void post(Function &&f, const ProtoAllocator &a);

  template <typename Function, typename ProtoAllocator>
  void dispatch(Function &&f, const ProtoAllocator &a);

  void on_work_started() const noexcept;

  void on_work_finished() const noexcept;

  friend bool operator==(const system_executor &, const system_executor &) noexcept { return true; }

  friend bool operator!=(const system_executor &, const system_executor &) noexcept { return false; }
};

}

#include <asyncnet/impl/system_executor.hpp>

#endif //ASYNCNET_SYSTEM_EXECUTOR_HPP
