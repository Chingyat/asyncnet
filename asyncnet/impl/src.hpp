//
// Created by tsing on 2020/11/6.
//

#ifndef ASYNCNET_IMPL_SRC_HPP
#define ASYNCNET_IMPL_SRC_HPP

#define ASYNCNET_SOURCE 1

#if !defined(ASYNCNET_SEPARATE_COMPILATION) && !defined(ASYNCNET_DYN_LINK)
#  error "Do not include this file in header only mode!"
#endif

#include <asyncnet/detail/config.hpp>

#include <asyncnet/detail/impl/key_index.ipp>
#include <asyncnet/impl/execution_context.ipp>
#include <asyncnet/impl/io_context.ipp>
#include <asyncnet/impl/system_context.ipp>
#include <asyncnet/impl/system_executor.ipp>

#endif //ASYNCNET_IMPL_SRC_HPP
