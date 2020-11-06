#include <iostream>
#include <asyncnet.hpp>
#include <asyncnet/impl/src.hpp>

#include <thread>
#include <list>

using namespace asyncnet;


static std::atomic<std::uint32_t> n{0};

void handler() {
    ++n;
}

static io_context ioc;

int main() {

    std::list<std::thread> threads;
    for (int i = 0; i != 100; ++i)
        threads.emplace_back(&io_context::run,
                             &ioc);

    for (int j = 0; j != 10000000; ++j)
        post(ioc, handler);

    for (auto &t: threads)
        t.join();

    std::cout << n << std::endl;
}
