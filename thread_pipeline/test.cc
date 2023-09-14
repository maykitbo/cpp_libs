#include "thread_pipeline.h"

#include <iostream>

void a_func(int& a) {
  a+=1;
}

void print(int& a) {
  std::cout << a << std::endl;
}

int main() {

  // создаем экземпляр
  Pipeline<int> pipe{};

  //добавляем 1 функцию в конвейер
  std::function<void(int&)> func = a_func;
  pipe.AddStage(func);

  //добавляем 2 функцию в конвейер
  std::function<void(int&)> func2 = print;
  pipe.AddStage(func2);

  //создаем переменную и запускаем конвейер
  int a = 89;
  pipe.Process(a);

  //сон чтобы главный поток не завершился раньше времени
  std::this_thread::sleep_for(std::chrono::seconds(1));

  return 0;
}





