#include "CommonUtils.h"
#include <iostream>
#include <future>
#include <queue>
#include <vector>

template <typename ResultType>
class ThreadPool
{
	size_t mThreadCount;
	bool mHasInitialized = false;
	bool mHasStopped = false;
	std::vector<std::thread> mThreads;
	std::queue<std::packaged_task<ResultType()>> mTasks;
	std::condition_variable mEventNewTask;
	std::mutex mBigMutex;

	void Worker();

public:
	ThreadPool(size_t threadCount) : mThreadCount(threadCount) {}
	void Initialize();
	bool HasStopped() { return mHasStopped; }
	void SetStopped() { mHasStopped = true; }
	std::future<ResultType> Add(std::function<ResultType()> f);
	~ThreadPool();
};

void StdPackagedTaskDemo();

template <typename ResultType>
inline void ThreadPool<ResultType>::Worker()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(mBigMutex);
		mEventNewTask.wait(lock, [this]
						   { return !mTasks.empty() || mHasStopped; });
		if (mHasStopped && mTasks.empty())
			return;
		auto task = std::move(mTasks.front());
		mTasks.pop();
		lock.unlock();
		task();
	}
}

template <typename ResultType>
inline void ThreadPool<ResultType>::Initialize()
{
	if (mHasInitialized)
		return;
	mHasInitialized = true;
	mThreads.reserve(mThreadCount);
	for (size_t i = 0; i < mThreadCount; i++)
	{
		mThreads.push_back(std::thread([this]
									   { Worker(); }));
	}
}

template <typename ResultType>
inline std::future<ResultType> ThreadPool<ResultType>::Add(std::function<ResultType()> f)
{
	Initialize();
	if (mHasStopped)
		throw std::runtime_error("Try to add new task on a stopped thread pool");
	std::packaged_task<ResultType()> task(f);
	auto future = task.get_future();
	{
		std::lock_guard<std::mutex> lock(mBigMutex);
		mTasks.push(std::move(task));
	}
	mEventNewTask.notify_one();
	return future;
}

template <typename ResultType>
inline ThreadPool<ResultType>::~ThreadPool()
{
	SetStopped();
	mEventNewTask.notify_all();
	for (auto &t : mThreads)
	{
		if (t.joinable())
			t.join();
	}
}

static int CalculatingSubtask()
{
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(1s);
	return GetGlobalRandomNumber(std::uniform_int_distribution<int>(1, 100000));
}

void StdPackagedTaskDemo_()
{
	ThreadPool<int> pool(4);
	pool.Initialize();
	std::vector<std::future<int>> futures;
	for (int i = 0; i < 10; i++)
	{
		futures.push_back(pool.Add(CalculatingSubtask));
	}
	for (auto &future : futures)
	{
		std::cout << future.get() << std::endl;
	}
}

void main()
{
	MeasureRuntime(StdPackagedTaskDemo_);
}
