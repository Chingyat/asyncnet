//
// Created by lince on 11/8/20.
//

#ifndef ASYNCNET_DETAIL_EVENT_HPP
#define ASYNCNET_DETAIL_EVENT_HPP

#include <asyncnet/detail/config.hpp>

#include <atomic>
#include <cassert>
#include <condition_variable>
#include <mutex>

namespace asyncnet {
namespace detail {

class event
{
public:
  event()
      : state_(0)
  {
  }

  ~event() noexcept
  = default;

  void reset(std::unique_lock<std::mutex> &lock)
  {
    assert(lock.owns_lock());
    (void) lock;
    state_ &= ~std::size_t(1);
  }

  void wait(std::unique_lock<std::mutex> &lock)
  {
    assert(lock.owns_lock());
    while ((state_ & std::size_t(1)) == 0)
    {
      waiter w(state_);
      cond_.wait(lock);
    }
  }

  bool wait_for_usec(std::unique_lock<std::mutex> &lock, long usec)
  {
    assert(lock.owns_lock());
    if ((state_ & std::size_t(1)) == 0)
    {
      waiter w(state_);
      cond_.wait_for(lock, std::chrono::microseconds(usec));
    }
    return (state_ & std::size_t(1)) == 0;
  }

  void notify_all(std::unique_lock<std::mutex> &lock)
  {
    assert(lock.owns_lock());
    (void)lock;
    state_ |= std::size_t(1);
    cond_.notify_all();
  }

  void notify_all(std::lock_guard<std::mutex> &lock)
  {
    (void)lock;
    state_ |= std::size_t(1);
    cond_.notify_all();
  }

  void unlock_and_notify_one(std::unique_lock<std::mutex> &lock)
  {
    assert(lock.owns_lock());
    state_ |= std::size_t (1);
    bool have_waiters = (state_ > 1);
    lock.unlock();
    if (have_waiters)
      cond_.notify_one();
  }

  void unlock_and_notify_one_protected(std::unique_lock<std::mutex> &lock)
  {
    assert(lock.owns_lock());
    state_ |= std::size_t (1);
    bool have_waiters = (state_ > 1);
    if (have_waiters)
      cond_.notify_one();
    lock.unlock();
  }

  bool maybe_unlock_and_notify_one(std::unique_lock<std::mutex> &lock)
  {
    assert(lock.owns_lock());
    state_ |= std::size_t (1);
    if (state_ > 1)
    {
      lock.unlock();
      cond_.notify_one();
      return true;
    }
    return false;
  }

private:
  class waiter {
  public:
    explicit waiter(std::size_t &state) noexcept
        : state_(state)
    {
      state_ += 2;
    }

    ~waiter() noexcept
    {
      state_ -= 2;
    }

  private:
    std::size_t &state_;
  };


  std::condition_variable cond_;
  std::size_t state_;
};


}// namespace detail
}// namespace asyncnet


#endif//ASYNCNET_DETAIL_EVENT_HPP
