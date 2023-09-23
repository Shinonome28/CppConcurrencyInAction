#include "CommonUtils.h"

#include <chrono>
#include <iostream>

void InitizeRandomEngine()
{
	random_engine.seed(std::random_device()());
}

double MeasureRuntime(std::function<void(void)> f, bool print)
{
	auto start = std::chrono::high_resolution_clock::now();
	f();
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> d = end - start;
	if (print)
	{
		std::cout << "Run for " << d.count() << " miliseconds" << std::endl;
	}
	return d.count();
}
