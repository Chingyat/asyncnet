#include <iostream>
#include <asyncnet.hpp>

#include <thread>
#include <list>
#include <array>


using namespace asyncnet;

class test_service : public execution_context::service
{
public:
  using key_type = test_service;

  explicit test_service(execution_context &c) : service(c)
  {
      std::cout << __func__ << "\n";
  }

  ~test_service()
  {
      std::cout << __func__ << '\n';
  }

  void shutdown() noexcept override
  {
      std::cout << __func__ << '\n';
  }

  std::array<int, 1000> obj;
};

static std::atomic_int n{0};

struct handler
{
  using executor_type = system_executor;

  static executor_type get_executor()
  {
    return {};
  }

  using allocator_type = std::allocator<void>;
  static allocator_type get_allocator()
  {
    return {};
  }

  handler() = default;

  int operator()()
  {
    std::fill(obj.begin(), obj.end(), n);
    return ++n;
  }

  std::array<int, 100> obj;
};

void test()
{
  static io_context ioc;

  use_service<test_service>(ioc);
 // use_service<test_service>(ioc);

  std::list<std::thread> threads;
  auto w = make_work(ioc);

  for (int i = 0; i != 10; ++i)
    threads.emplace_back(&io_context::run,
                         &ioc);

  for (int j = 0; j != 1200; ++j)
    post(ioc, handler());

  w.reset();
  ioc.stop();

  for (auto &t: threads)
    t.join();

  ioc.restart();

  post(ioc, handler());
  ioc.run();

  std::cout << n << std::endl;
}


int main() {
  test();
}