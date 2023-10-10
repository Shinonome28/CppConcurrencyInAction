#include <atomic>
#include <iostream>
#include <thread>

struct Values {
  int x, y, z;
};

constexpr int kLoopCount = 10;

Values buffer1[kLoopCount];
Values buffer2[kLoopCount];
Values buffer3[kLoopCount];
Values buffer4[kLoopCount];
Values buffer5[kLoopCount];

std::atomic<int> x(0), y(0), z(0);
std::atomic<bool> go;

void increament_and_load(std::atomic<int> *var_to_inc, Values *buffer) {
  while (!go) {
    std::this_thread::yield();
  }
  for (int i = 0; i < kLoopCount; i++) {
    buffer[i].x = x.load(std::memory_order_relaxed);
    buffer[i].y = y.load(std::memory_order_relaxed);
    buffer[i].z = z.load(std::memory_order_relaxed);
    var_to_inc->store(i + 1, std::memory_order_relaxed);
    std::this_thread::yield();
  }
}

void read_val(Values *buffer) {
  while (!go) {
    std::this_thread::yield();
  }
  for (int i = 0; i < kLoopCount; i++) {
    buffer[i].x = x.load(std::memory_order_relaxed);
    buffer[i].y = y.load(std::memory_order_relaxed);
    buffer[i].z = z.load(std::memory_order_relaxed);
    std::this_thread::yield();
  }
}

void print_values(Values *v) {
  for (int i = 0; i < kLoopCount; i++) {
    if (i) std::cout << ",";
    std::cout << "(" << v[i].x << "," << v[i].y << "," << v[i].z << ")";
  }
  std::cout << std::endl;
}

int main() {
  std::thread t1(increament_and_load, &x, buffer1);
  std::thread t2(increament_and_load, &y, buffer2);
  std::thread t3(increament_and_load, &z, buffer3);
  std::thread t4(read_val, buffer4);
  std::thread t5(read_val, buffer5);
  go = true;
  t5.join();
  t4.join();
  t3.join();
  t2.join();
  t1.join();
  print_values(buffer1);
  print_values(buffer2);
  print_values(buffer3);
  print_values(buffer4);
  print_values(buffer5);
}