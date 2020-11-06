cmake_minimum_required(VERSION 3.17)
project(asyncnet)

set(CMAKE_CXX_STANDARD 14)

find_package(Threads REQUIRED)

set(ASYNCNET_HEADERS
        asyncnet.hpp
        asyncnet/associated_allocator.hpp
        asyncnet/associated_executor.hpp
        asyncnet/async_completion.hpp
        asyncnet/async_result.hpp
        asyncnet/detail/impl/key_index.ipp
        asyncnet/detail/key_index.hpp
        asyncnet/detail/move_only.hpp
        asyncnet/detail/wrapped_handler.hpp
        asyncnet/dispatch.hpp
        asyncnet/execution_context.hpp
        asyncnet/executor.hpp
        asyncnet/executor_work_guard.hpp
        asyncnet/impl/dispatch.hpp
        asyncnet/impl/execution_context.hpp
        asyncnet/impl/execution_context.ipp
        asyncnet/impl/io_context.hpp
        asyncnet/impl/io_context.ipp
        asyncnet/impl/post.hpp
        asyncnet/impl/src.hpp
        asyncnet/impl/system_context.ipp
        asyncnet/impl/system_executor.hpp
        asyncnet/impl/system_executor.ipp
        asyncnet/io_context.hpp
        asyncnet/posix/descriptor.hpp
        asyncnet/posix/impl/descriptor.hpp
        asyncnet/posix/impl/descriptor.ipp
        asyncnet/post.hpp
        asyncnet/service.hpp
        asyncnet/system_context.hpp
        asyncnet/system_executor.hpp
        asyncnet/thread_pool.hpp
        asyncnet/detail/intrusive_list_node.hpp asyncnet/detail/intrusive_list.hpp)

include_directories(.)
add_executable(asyncnet main.cpp ${ASYNCNET_HEADERS})
target_link_libraries(asyncnet PRIVATE Threads::Threads)
