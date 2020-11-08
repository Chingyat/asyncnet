//
// Created by tsing on 2020/11/6.
//

#ifndef ASYNCNET_DETAIL_IMPL_KEY_INDEX_IPP
#define ASYNCNET_DETAIL_IMPL_KEY_INDEX_IPP

#include <asyncnet/detail/key_index.hpp>

#include <atomic>

namespace asyncnet {
namespace detail {

unsigned long key_counter_inc() noexcept
{
  static std::atomic_ulong counter{0};
  return ++counter;
}

}
}

#endif //ASYNCNET_DETAIL_IMPL_KEY_INDEX_IPP
