#include <atomic>
#include <cstdio>
#include <thread>

std::atomic<bool> x(false), y(false);
std::atomic<int> z(0);

void write_x_then_y() {
  x.store(true, std::memory_order_relaxed);
  std::atomic_thread_fence(std::memory_order_release);
  y.store(true, std::memory_order_relaxed);
}

void read_y_then_x() {
  while (!y.load(std::memory_order_relaxed))
    ;
  std::atomic_thread_fence(std::memory_order_acquire);
  if (x.load(std::memory_order_relaxed)) {
    z++;
  }
}

int main() {
  std::thread a(write_x_then_y);
  std::thread b(read_y_then_x);
  a.join();
  b.join();
  if (z.load() != 0) {
    std::printf("Sucess\n");
  } else {
    std::printf("Fail\n");
  }
  return 0;
}