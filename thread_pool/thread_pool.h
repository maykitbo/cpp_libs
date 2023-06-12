#pragma once
#include <thread>
#include <vector>
#include <queue>
#include <iostream>

class ThreadPool {
public:
  ThreadPool() : ThreadPool(std::thread::hardware_concurrency()) {}

  explicit ThreadPool(unsigned size) : stopped(false) {
    for (int i = 0; i < size; ++i) {
      threads.emplace_back(&ThreadPool::ThreadRun, this);
    }
  }

  ~ThreadPool() {
    if (!stopped) {
      ShutDown();
    }
  }

  template<class F, class... Args>
  void AddTask(F&& f, Args&&... args) {
    AddTask([=] { f(args...); });
  }

  void AddTask(const std::function<void()>& f) {
    {
      std::unique_lock<std::mutex> lock(queueMutex);
      tasks.push(f);
      ++active;
    }
    condition.notify_one();
  }

  void ShutDown() {
    stopped = true;
    condition.notify_all();

    for (std::thread &thread : threads) {
      thread.join();
    }
    threads.clear();
    stopped = false;
  }

  void WaitForComplete() {
    std::unique_lock<std::mutex> lock(queueMutex);
    condition.wait(lock, [this] {return !active;});
    if (!active) {
      condition.notify_one();
      WaitForComplete();
    }
  }

private:
  void ThreadRun() {
    std::function<void()> task;
    while (true) {
      {
        std::unique_lock<std::mutex> lock(queueMutex);
        condition.wait(lock, [this] { return stopped || !tasks.empty(); });
        if (stopped && tasks.empty())
          return;
        task = std::move(tasks.front());
        tasks.pop();
      }
      try {
        task();
      } catch (std::runtime_error &ex) {
        std::cout << "at " << std::this_thread::get_id() << " " << ex.what() << std::endl;
      }
      if (active)
        --active;
      condition.notify_all();
    }

  }

  std::mutex queueMutex;
  std::vector<std::thread> threads;
  std::queue<std::function<void()>> tasks;
  std::condition_variable condition;
  std::atomic_bool stopped;
  std::atomic_uint active{};
};