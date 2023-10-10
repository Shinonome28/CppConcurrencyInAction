#include "CommonUtils.h"
#include <iostream>

#include <algorithm>
#include <list>
#include <future>

template <typename T>
// 仅供测试，本算法实现并不实用
std::list<T> QuickSort(std::list<T> input)
{
	if (input.empty() || input.size() == 1)
		return input;
	std::list<T> result;
	result.splice(result.begin(), input, input.begin());
	const T &pivot = *result.begin();
	auto divide_point = std::partition(input.begin(), input.end(), [&](const T &t)
									   { return t < pivot; });
	std::list<T> lower_part;
	lower_part.splice(lower_part.end(), input, input.begin(), divide_point);
	auto new_lower(QuickSort(std::move(lower_part)));
	auto new_higher(QuickSort(std::move(input)));
	result.splice(result.begin(), new_lower);
	result.splice(result.end(), new_higher);
	return result;
}

template <typename T>
// 仅供测试，本算法实现并不实用
std::list<T> ParallelQuickSort(std::list<T> input)
{
	if (input.empty() || input.size() == 1)
		return input;
	std::list<T> result;
	result.splice(result.begin(), input, input.begin());
	const T &pivot = *result.begin();
	auto divide_point = std::partition(input.begin(), input.end(), [&](const T &t)
									   { return t < pivot; });
	std::list<T> lower_part;
	lower_part.splice(lower_part.end(), input, input.begin(), divide_point);
	std::list<T> new_lower(QuickSort(std::move(lower_part)));
	// 让标准库决定是否在新线程中执行，或是推迟运行，这是一种简单且不可靠的线程调度方法
	std::future<std::list<T>> new_higher_future = std::async(&ParallelQuickSort<T>, std::move(input));
	result.splice(result.begin(), new_lower);
	result.splice(result.end(), new_higher_future.get());
	return result;
}

void main()
{
	std::list<int> testList;
	for (int i = 1; i <= static_cast<int>(5 * 1e5); i++)
	{
		testList.push_back(GetGlobalRandomNumber(std::uniform_int_distribution<int>(1, 10000)));
	}
	std::cout << "Not parallel quick sort: " << std::endl;
	MeasureRuntime([&testList]()
				   { QuickSort(testList); });
	std::cout << "Parallel quick sort: " << std::endl;
	MeasureRuntime([&testList]()
				   { ParallelQuickSort(testList); });
}
