// 这个demo反映了memory_order_relaxed不保证任何全局同步性
// 只保证在同一个线程中的同步性

#include <atomic>
#include <iostream>
#include <thread>

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x_then_y() {
  x.store(true, std::memory_order_relaxed);
  y.store(true, std::memory_order_relaxed);
}

void read_y_then_x() {
  while (!y.load(std::memory_order_relaxed))
    ;
  if (x.load(std::memory_order_relaxed)) ++z;
}

int main() {
  x = false;
  y = false;
  z = 0;
  std::thread t1(write_x_then_y);
  std::thread t2(read_y_then_x);
  t1.join();
  t2.join();
  if (z.load()) {
    std::cout << "Sync successfully" << std::endl;
  } else {
    std::cout << "Sync fail" << std::endl;
  }
  return 0;
}