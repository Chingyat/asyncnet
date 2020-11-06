//
// Created by tsing on 2020/11/5.
//

#ifndef ASYNCNET_ASYNC_RESULT_HPP
#define ASYNCNET_ASYNC_RESULT_HPP

namespace asyncnet {

template <typename CompletionToken, typename Signature>
struct async_result
{
  using completion_handler_type = CompletionToken;

  using result_type = void;

  explicit async_result(completion_handler_type &h) noexcept
  {
    (void)h;
  }

  result_type get() noexcept
  {
  }
};
}

#endif //ASYNCNET_ASYNC_RESULT_HPP
