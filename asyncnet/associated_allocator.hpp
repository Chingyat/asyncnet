//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_ASSOCIATED_ALLOCATOR_HPP
#define ASYNCNET_ASSOCIATED_ALLOCATOR_HPP

namespace asyncnet {

    template<typename T, typename Allocator = std::allocator<void>>
    struct associated_allocator {
        using type = Allocator;

        static type get(const T &t, const Allocator &a = Allocator()) {
            (void) t;
            return a;
        }
    };

    template<typename T, typename Allocator = std::allocator<void>>
    using associated_allocator_t = typename associated_allocator<T, Allocator>::type;

    template<typename T, typename Allocator = std::allocator<void>>
    associated_allocator_t<T, Allocator> get_associated_allocator(const T &t, const Allocator &a = Allocator()) {
        return associated_allocator<T, Allocator>::get(t, a);
    }
}

#endif //ASYNCNET_ASSOCIATED_ALLOCATOR_HPP
