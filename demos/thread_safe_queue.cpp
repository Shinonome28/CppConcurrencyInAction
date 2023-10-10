#include "CommonUtils.h"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <queue>

template <typename T>
class ThreadSafeQueue
{
private:
	mutable std::mutex mux;
	std::queue<T> data_queue;
	std::condition_variable data_cond;

public:
	void Push(const T &new_value)
	{
		std::lock_guard<std::mutex> lock(mux);
		data_queue.push(new_value);
		data_cond.notify_one();
	}
	bool TryPop(T &value)
	{
		std::lock_guard<std::mutex> lock(mux);
		if (data_queue.empty())
			return false;
		value = std::move(data_queue.front());
		data_queue.pop();
		return true;
	}
	std::shared_ptr<T> TryPop()
	{
		std::lock_guard<std::mutex> lock(mux);
		if (data_queue.empty())
			return std::shared_ptr<T>();
		std::shared_ptr<T> res(std::make_shared<T>(std::move(data_queue.front())));
		data_queue.pop();
		return res;
	}
	void WaitAndPop(T &value)
	{
		// �������ʹ��unique_lock����Ϊ����Ҫ���������ڿ�����
		std::unique_lock<std::mutex> lock(mux);
		data_cond.wait(lock, [this]
					   { return !data_queue.empty(); });
		// ��ʱ�Ѿ��Զ�����
		value = std::move(data_queue.front());
		data_queue.pop();
	}
	std::shared_ptr<T> WaitAndPop()
	{
		std::unique_lock lock(mux);
		data_cond.wait(lock, [this]
					   { return !data_queue.empty(); });
		std::shared_ptr<T> res(std::make_shared<T>(std::move(data_queue.front())));
		data_queue.pop();
		return res;
	}
};

static void do_computing(ThreadSafeQueue<int> &queue)
{
	using namespace std::chrono_literals;

	std::this_thread::sleep_for(1000ms);
	queue.Push(std::rand());
}

static void process_data(ThreadSafeQueue<int> &queue)
{
	for (int i = 0; i < 10; i++)
	{
		int a;
		queue.WaitAndPop(a);
		// WARNING: this is not thread safe, but we only use it in a single thread
		std::cout << "a = " << a << std::endl;
	}
}

static void test_ThreadSafeQueue_main_()
{
	ThreadSafeQueue<int> queue;
	std::vector<std::thread> threads;
	for (int i = 0; i < 10; i++)
	{
		threads.push_back(std::thread(do_computing, std::ref(queue)));
	}
	std::thread t2(process_data, std::ref(queue));
	for (auto &t : threads)
	{
		t.join();
	}
	t2.join();
}

void main()
{
	MeasureRuntime(test_ThreadSafeQueue_main_);
}