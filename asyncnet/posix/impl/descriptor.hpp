//
// Created by lince on 11/6/20.
//

#ifndef ASYNCNET_POSIX_IMPL_DESCRIPTOR_HPP
#define ASYNCNET_POSIX_IMPL_DESCRIPTOR_HPP

#include <asyncnet/posix/descriptor.hpp>

namespace asyncnet {
namespace posix {
namespace detail {

template<typename WaitHandler>
void descriptor_service::async_wait(descriptor_base::wait_type w, WaitHandler &&handler)
{

}

}
}
}

#endif //ASYNCNET_POSIX_IMPL_DESCRIPTOR_HPP
