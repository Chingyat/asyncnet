//
// Created by tsing on 2020/11/5.
//

#ifndef ASYNCNET_DETAIL_WRAPPED_HANDLER_HPP
#define ASYNCNET_DETAIL_WRAPPED_HANDLER_HPP

#include <utility>
#include <memory>
#include <cassert>

namespace asyncnet {
    namespace detail {

        template<typename Signature>
        struct handler_base;

        template<typename ...Params>
        struct handler_base<void(Params...)> {
            virtual void release() noexcept = 0;

            virtual void invoke(Params ...params) = 0;
        };

        template<typename Signature, typename T, typename Allocator>
        struct handler_impl;

        template<typename T, typename ProtoAllocator, typename ...Params>
        struct handler_impl<void(Params...), T, ProtoAllocator> final
                : handler_base<void(Params...)>, ProtoAllocator {
            using allocator_type = typename std::allocator_traits<ProtoAllocator>::
            template rebind_alloc<handler_impl>;

            T t;

            allocator_type get_allocator() const {
                return *this;
            }

            void release() noexcept final {
                using traits_type = typename std::allocator_traits<allocator_type>;
                allocator_type alloc(get_allocator());
                traits_type::destroy(alloc, this);
                traits_type::deallocate(alloc, typename traits_type::pointer(this), 1);
            }

            void invoke(Params ...args) final {
                auto h = std::move(t);
                release();

                h(static_cast<Params>(args)...);
            }

            template<typename U>
            explicit handler_impl(int, U &&u)
                    : t(std::forward<U>(u)) {}

        };

        template<typename Signature>
        struct wrapped_handler {
            handler_base<Signature> *_owned_handler;

            template<typename U, typename ProtoAllocator>
            explicit wrapped_handler(U &&u, const ProtoAllocator &alloc) {
                using impl_type = handler_impl<Signature, std::decay_t<U>, ProtoAllocator>;
                using traits_type = typename std::allocator_traits<ProtoAllocator>::
                template rebind_traits<impl_type>;
                typename traits_type::allocator_type a(alloc);

                auto pointer = traits_type::allocate(a, 1);
                try {
                    auto address = std::addressof(*pointer);
                    traits_type::construct(a, address, int(), std::forward<U>(u));
                    _owned_handler = address;
                } catch (...) {
                    traits_type::deallocate(a, pointer, 1);
                    throw;
                }
            }

            wrapped_handler(wrapped_handler &&other) noexcept
                    : _owned_handler(std::exchange(other._owned_handler, nullptr)) {}

            wrapped_handler &operator=(wrapped_handler &&other) noexcept {
                using std::swap;
                swap(_owned_handler, other._owned_handler);
                return *this;
            }

            ~wrapped_handler() noexcept {
                if (_owned_handler) {
                    _owned_handler->release();
                }
            }

            template<typename ...Args>
            void invoke(Args &&...args) {
                assert(_owned_handler);

                _owned_handler->invoke(std::forward<Args>(args)...);
                _owned_handler = nullptr;
            }

            bool has_value() const {
                return _owned_handler != nullptr;
            }
        };
    }
}

#endif //ASYNCNET_DETAIL_WRAPPED_HANDLER_HPP
