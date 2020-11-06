cmake_minimum_required(VERSION 3.17)
project(asyncnet)

set(CMAKE_CXX_STANDARD 14)

find_package(Threads REQUIRED)

set(ASYNCNET_HEADERS
        asyncnet/execution_context.hpp asyncnet/service.hpp asyncnet/thread_pool.hpp asyncnet/async_result.hpp asyncnet/async_completion.hpp asyncnet/io_context.hpp asyncnet/impl/io_context.ipp asyncnet/detail/wrapped_handler.hpp asyncnet/impl/execution_context.ipp asyncnet/impl/execution_context.hpp asyncnet/detail/key_index.hpp asyncnet/detail/impl/key_index.ipp asyncnet/impl/src.hpp asyncnet/impl/io_context.hpp asyncnet/post.hpp asyncnet/impl/post.hpp asyncnet/executor_work_guard.hpp asyncnet/associated_executor.hpp asyncnet/associated_allocator.hpp asyncnet/executor.hpp asyncnet.hpp asyncnet/detail/move_only.hpp asyncnet/dispatch.hpp asyncnet/impl/dispatch.hpp asyncnet/system_context.hpp asyncnet/system_executor.hpp asyncnet/impl/system_context.ipp asyncnet/impl/system_executor.ipp asyncnet/impl/system_executor.hpp asyncnet/posix/descriptor.hpp asyncnet/posix/impl/descriptor.ipp asyncnet/posix/impl/descriptor.hpp)

include_directories(.)
add_executable(asyncnet main.cpp ${ASYNCNET_HEADERS})
target_link_libraries(asyncnet PRIVATE Threads::Threads)
