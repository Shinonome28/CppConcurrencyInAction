#pragma once

#include <random>
#include <mutex>
#include <functional>
#include <iostream>
#include <string>

static std::mt19937 random_engine;
static std::mutex random_engine_mutex;

// This is not guaranteed to be thread-safe.
void InitizeRandomEngine();

// This is guaranteeded to be thread-safe.
template <typename T>
typename T::result_type GetGlobalRandomNumber(T distribution)
{
	std::lock_guard<std::mutex> lock(random_engine_mutex);
	return distribution(random_engine);
}

double MeasureRuntime(std::function<void(void)> f, bool print = true);

template<typename Container>
void printContainer(const Container& container, std::string sperator = " ", const size_t new_line_per_element = 10)
{
	size_t count = 0;
	for (auto& element : container)
	{
		std::cout << element << sperator;
		if (++count % new_line_per_element == 0)
		{
			std::cout << std::endl;
			count = 0;
		}
	}
	std::cout << std::endl;
}