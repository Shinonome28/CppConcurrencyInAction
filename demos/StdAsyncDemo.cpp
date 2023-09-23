#include "CommonUtils.h"

#include <future>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <iostream>

static int Task()
{
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(1s);
	return GetGlobalRandomNumber(std::uniform_int_distribution<>(0, 100000));
}

static void StdAsyncDemoMain()
{
	// std::async provides a way to get a function's result asynchronously.
	// std::async let you write asynchronous code in a serial manner.
	// Note that we normally don't create std::future by hand, but used as return result of other functions.
	std::vector<std::future<int>> futures;
	for (int i = 0; i < 10; i++)
	{
		futures.push_back(std::async(std::launch::async, Task)); // use std::launch::async to force a new thread
	}
	for (auto &f : futures)
	{
		std::cout << f.get() << std::endl;
	}
}

void main()
{
	MeasureRuntime(StdAsyncDemoMain);
}
