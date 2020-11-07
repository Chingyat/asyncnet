//
// Created by tsing on 2020/11/5.
//

#ifndef ASYNCNET_ASYNC_COMPLETION_HPP
#define ASYNCNET_ASYNC_COMPLETION_HPP

#include <asyncnet/async_result.hpp>
#include <utility>

namespace asyncnet {

/// A convenient class that helps to obtain completion handlers and async_result objects
/// from completion tokens.
///
/// @tparam CompletionToken Completion token type.
/// @tparam Signature Signature of the completion handler.
template <typename CompletionToken, typename Signature>
struct async_completion
{
  using completion_handler_type = typename async_result<
      std::decay_t<CompletionToken>, Signature>::completion_handler_type;

  completion_handler_type completion_handler;

  async_result<std::decay_t<CompletionToken>, Signature> result;

  explicit async_completion(CompletionToken &token)
      : completion_handler(std::forward<CompletionToken>(token)),
        result(completion_handler)
  {
  }
};

} // namespace asyncnet

#endif //ASYNCNET_ASYNC_COMPLETION_HPP
