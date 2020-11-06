//
// Created by tsing on 2020/11/6.
//

#ifndef ASYNCNET_DETAIL_KEY_INDEX_HPP
#define ASYNCNET_DETAIL_KEY_INDEX_HPP

namespace asyncnet {
    namespace detail {

        unsigned long key_counter_inc();

        template <typename Key>
        unsigned long key_index()
        {
            static unsigned long index = key_counter_inc();
            return index;
        }

    }
}

#endif //ASYNCNET_DETAIL_KEY_INDEX_HPP
