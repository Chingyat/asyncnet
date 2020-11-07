#include <iostream>
#include <asyncnet.hpp>
#include <asyncnet/impl/src.hpp>

#include <thread>
#include <list>

using namespace asyncnet;

class test_service : public execution_context::service
{
public:
  using key_type = test_service;

  explicit test_service(execution_context &c) : service(c)
  {

  }

  void shutdown() noexcept override
  {

  }
};

static std::atomic_int n{0};

struct handler
{
  std::array<char, 100> obj;

  handler() = default;

  int operator()()
  {
    std::fill(obj.begin(), obj.end(), n);
    return ++n;
  }
};

int main()
{
  static io_context ioc;

  use_service<test_service>(ioc);

  std::list<std::thread> threads;
  auto w = make_work(ioc);

  for (int i = 0; i != 100; ++i)
    threads.emplace_back(&io_context::run,
                         &ioc);

  for (int j = 0; j != 10000; ++j)
    post(ioc, handler());

  w.reset();

  for (auto &t: threads)
    t.join();

  std::cout << n << std::endl;
}
