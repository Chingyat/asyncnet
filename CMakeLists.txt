cmake_minimum_required(VERSION 3.17)
project(asyncnet)

set(CMAKE_CXX_STANDARD 14)

set(ASYNCNET_HEADERS
        asyncnet/execution_context.hpp asyncnet/service.hpp asyncnet/thread_pool.hpp asyncnet/async_result.hpp asyncnet/async_completion.hpp asyncnet/io_context.hpp asyncnet/impl/io_context.ipp asyncnet/detail/wrapped_handler.hpp asyncnet/impl/execution_context.ipp asyncnet/impl/execution_context.hpp asyncnet/detail/key_index.hpp asyncnet/detail/impl/key_index.ipp asyncnet/impl/src.hpp)

include_directories(.)
add_executable(asyncnet main.cpp ${ASYNCNET_HEADERS})
