//
// Created by tsing on 2020/11/6.
//

#ifndef ASYNCNET_DETAIL_KEY_INDEX_HPP
#define ASYNCNET_DETAIL_KEY_INDEX_HPP

#include <asyncnet/detail/config.hpp>

namespace asyncnet {
namespace detail {

ASYNCNET_DECL unsigned long key_counter_inc() noexcept;

template <typename Key>
unsigned long key_index()
{
  static unsigned long index = key_counter_inc();
  return index;
}

}
}

#ifdef ASYNCNET_HEADER_ONLY
# include <asyncnet/detail/impl/key_index.ipp>
#endif

#endif //ASYNCNET_DETAIL_KEY_INDEX_HPP
