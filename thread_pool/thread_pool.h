#pragma once
#include <thread>
#include <vector>
#include <queue>
#include <iostream>

class ThreadPool {
public:
  ThreadPool() : ThreadPool(std::thread::hardware_concurrency()) {}

  ThreadPool(size_t size) : stopped(false) {
    for (size_t i = 0; i < size; ++i) {
      threads.emplace_back(std::thread(&ThreadPool::ThreadRun, this));
    }
  }

  template<class F, class... Args>
  void AddTask(F&& f, Args&&... args) {
    {
      std::unique_lock<std::mutex> lock(queueMutex);
      tasks.emplace([=] { f(args...); });
    }
    condition.notify_one();
  }

  void AddTask(std::function<void()> f) {
    {
      std::unique_lock<std::mutex> lock(queueMutex);
      tasks.push(f);
    }
    condition.notify_one();
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
    }
  }

  std::mutex queueMutex;
  std::vector<std::thread> threads;
  std::queue<std::function<void()>> tasks;
  std::condition_variable condition;
  std::atomic_bool stopped;
};