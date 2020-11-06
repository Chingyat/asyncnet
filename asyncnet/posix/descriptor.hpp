//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_POSIX_DESCRIPTOR_HPP
#define ASYNCNET_POSIX_DESCRIPTOR_HPP

#include <asyncnet/service.hpp>
#include <asyncnet/async_completion.hpp>

#include <unistd.h>

namespace asyncnet {
    namespace posix {

        class descriptor_base {
        public:
            enum wait_type {
                wait_read,
                wait_write,
                wait_error,
            };
        };

        namespace detail {
            class descriptor_service : execution_context::service {
            public:
                /// Native descriptor type.
                using native_handle_type = int;

                using descriptor_state = unsigned long;

                enum : descriptor_state {
                    state_busy = 0,

                    state_readable = 0x01,

                    state_writable = 0x02,

                    state_error = 0x04,

                    state_readable_writable = state_readable | state_writable,

                    state_all = state_readable | state_writable | state_error,
                };


                /// Implementation type of the descriptor.
                class implementation_type {
                public:

                    /// Default constructor.
                    implementation_type()
                            : _fd(-1), _state(0) {

                    }

                private:
                    /// Only this service will have access to the internal values.
                    friend descriptor_service;

                    /// The native file descriptor.
                    int _fd;

                    /// The state of the descriptor
                    descriptor_state _state;

                };

                /// Constructs a descriptor service.
                descriptor_service(execution_context &context)
                        : execution_context::service(context) {

                }

                /// Constructs a descriptor implementation.
                void construct(implementation_type &impl);

                /// Destroys the descriptor implementation.
                void destroy(implementation_type &impl);

                /// Move constructs a descriptor implementation.
                void move_construct(implementation_type &impl, implementation_type &other) noexcept;

                /// Move assigns a descriptor implementation.
                void move_assign(implementation_type &impl, implementation_type &other) noexcept;

                /// Assign a raw file descriptor to the implementation.
                std::error_code assign(implementation_type &impl, native_handle_type fd);

                /// Close the file descriptor.
                std::error_code close(implementation_type &impl);

                /// Indicates whether the implementation has an open file descriptor.
                bool is_open(const implementation_type &impl) const;

                /// Indicates whether the descriptor is ready to be read.
                bool is_readable(const implementation_type &impl) const;

                /// Indicates whether the descriptor is ready to be written.
                bool is_writable(const implementation_type &impl) const;

                /// Returns the native descriptor type.
                native_handle_type get_native_handle(const implementation_type &impl) const;

                /// Indicates whether the descriptor is in non-blocking mode.
                bool non_blocking(const implementation_type &impl) const;

                /// Sets non-blocking mode.
                std::error_code set_nonblocking(implementation_type &impl, bool mode);

                /// Asynchronous waits for events.
                template <typename WaitHandler>
                void async_wait(descriptor_base::wait_type w, WaitHandler &&handler);

            private:
                /// Destroy all handlers allocated by the service.
                void shutdown() noexcept override;
            };
        }

        template<typename Executor>
        class basic_descriptor : public descriptor_base {
        public:
            using executor_type = Executor;
            using service_type = detail::descriptor_service;
            using implementation_type = typename service_type::implementation_type;

            using native_handle_type = typename service_type::native_handle_type;



            explicit basic_descriptor(const executor_type &ex)
                    : _ex(ex),
                      _service(&use_service<service_type>(ex.context())) {
                get_service().construct(_impl);
            }

            basic_descriptor(const executor_type &ex, native_handle_type fd)
                    : basic_descriptor(ex) {
                get_service().construct(_impl);
                if (auto ec = get_service().assign(_impl, fd))
                    throw std::system_error(ec, __func__);
            }

            basic_descriptor(basic_descriptor &&other)  noexcept
                    : _ex(other.get_executor())
                    , _service(&other.get_service()) {
                get_service().move_construct(_impl, other.get_implementation());
            }

            basic_descriptor &operator=(basic_descriptor &&other) noexcept {
                get_service().move_assign(get_implementation(), other.get_implementation());
            }

            ~basic_descriptor() noexcept {
                get_service().destroy(get_implementation());
            }

            executor_type get_executor() {
                return _ex;
            }

            bool non_blocking(std::error_code &ec) const;

            void non_blocking(bool v, std::error_code &ec);

            // operations

            template<typename CompletionToken>
            typename async_result<CompletionToken, void(std::error_code)>::result_type
            async_wait(wait_type wt, CompletionToken &&token);

            template<typename MutableBufferSequence, typename CompletionToken>
            typename async_result<CompletionToken, void(std::error_code)>::result_type
            async_receive(const MutableBufferSequence &bs, CompletionToken &&token);

            template<typename MutableBufferSequence, typename CompletionToken>
            typename async_result<CompletionToken, void(std::error_code)>::result_type
            async_send(const MutableBufferSequence &bs, CompletionToken &&token);


        private:
            implementation_type &get_implementation() { return _impl; }

            const implementation_type &get_implementation() const { return _impl; }

            service_type &get_service() { return *_service; }

            const service_type &get_service() const { return *_service; }

            executor_type _ex;
            service_type *_service;
            implementation_type _impl;
        };
    }
}

#endif //ASYNCNET_POSIX_DESCRIPTOR_HPP
