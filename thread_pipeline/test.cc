#include "thread_pipeline.h"

#include <iostream>

void a_func(int& a) {
  a+=1;
}

void print(int& a) {
  std::cout << a << std::endl;
}

int main() {
  std::cout << "ok1" << std::endl;
  Pipeline<int> pipe{};
  std::cout << "ok2" << std::endl;

  std::function<void(int&)> func = a_func;
  pipe.AddStage(func);
  std::function<void(int&)> func2 = print;

  pipe.AddStage(func2);
  int a = 89;
  pipe.Process(a);
//  std::this_thread::sleep_for(std::chrono::seconds(1));
  return 0;
}





