//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_DETAIL_MOVE_ONLY_HPP
#define ASYNCNET_DETAIL_MOVE_ONLY_HPP

namespace asyncnet {
namespace detail {

class move_only
{
public:
  explicit constexpr move_only() = default;

  constexpr move_only(move_only &&) noexcept = default;

  constexpr move_only &operator=(move_only &&) noexcept = default;
};

}
}

#endif //ASYNCNET_DETAIL_MOVE_ONLY_HPP
