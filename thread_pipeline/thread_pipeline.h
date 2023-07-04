#pragma once

#include <functional>
#include <queue>
#include <thread>
#include <any>
#include <vector>
#include <iostream>

template <typename T>
class PipelineElement {
  using func_type = std::function<void(T&)>;
public:
  PipelineElement() = delete;
  PipelineElement(func_type f, std::function<bool(T&)> ch) : func(f), check(ch) {
     t = new std::thread(&PipelineElement::Worker, this);
//    t.detach();
  }
  ~PipelineElement() {
    delete next;
    Finish();

  }

  void Process(T& data) {
    if (check(data)) {
      task_queue.push(data);
      cv.notify_one();
    }
  }

  void SetNext(PipelineElement * elem) { next = elem;}

private:
  void Worker() {
    while (true) {
      std::unique_lock<std::mutex> lock(mtx);
      cv.wait(lock, [this] { return is_finished || !task_queue.empty(); });
      if (is_finished && task_queue.empty()) {
        return;
      }
      T& data = task_queue.front();
      task_queue.pop();
      lock.unlock();
      func(data);
      if (next != nullptr) {
        next->Process(data);
      }
    }
  }


  void Finish() {
    {
      std::unique_lock<std::mutex> lock(mtx);
      is_finished = true;
    }
    cv.notify_all();
    t->join();
    delete t;

  }


  func_type func;
  PipelineElement* next;
  std::function<bool(T&)> check;
  std::queue<std::reference_wrapper<T>> task_queue;
  std::condition_variable cv;
  std::mutex mtx;
  bool is_finished = false;
  std::thread* t;
};

template <typename T>
  class Pipeline {
    using PipelineFunction = std::function<void(T&)>;

  public:
    Pipeline() = default;
    ~Pipeline() {
      while (first) {
        delete first;
        first = first->GetNext();
      }
    }

    void AddStage(std::function<void(int &)> func_, std::function<bool(T&)> check = [](T&){return true;}) {
      auto *new_element = new PipelineElement<T>(func_, check);
      if (first == nullptr) {
        first = new_element;
        last = new_element;
      } else {
        last->SetNext(new_element);
        last = new_element;
      }
    }

    void Process(T& data)
    {
      if (first != nullptr) {
        first->Process(data);
      }
    }

  private:
    PipelineElement<T> *first;
    PipelineElement<T> *last;
  };
