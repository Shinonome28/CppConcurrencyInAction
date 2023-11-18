#include <atomic>
#include <cstdio>
#include <sstream>
#include <thread>

#include "CommonUtils.h"

std::vector<int> queue_data;
std::atomic<int> count;

void populate_queue() {
  const int numer_of_items = 2000;
  queue_data.clear();
  for (int i = 0; i < numer_of_items; i++) {
    queue_data.push_back(i);
  }
  count.store(numer_of_items, std::memory_order_release);
}

void comsume_queue_items() {
  using namespace std::chrono_literals;
  while (true) {
    int item_index;
    if ((item_index = count.fetch_sub(1, std::memory_order_acquire)) <= 0) {
      std::this_thread::sleep_for(1000ms);
      if ((item_index = count.fetch_sub(1, std::memory_order_acquire)) <= 0) {
        break;
      }
    }
    printf("%d\n", queue_data[item_index - 1]);
  }
}

int main() {
  InitizeRandomEngine();

  std::thread a(populate_queue);
  std::thread b(comsume_queue_items);
  std::thread c(comsume_queue_items);
  a.join();
  b.join();
  c.join();
  return 0;
}