
#define BOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY
#define BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
#include <boost/thread/future.hpp>
#include <vector>
#include <iostream>
#include "CommonUtils.h"

void main()
{
    std::vector<boost::future<int>> tasks;
    for (int i = 0; i < 100; i++)
    {
        tasks.push_back(boost::async([]() -> int
                                     { return GetGlobalRandomNumber(std::uniform_int_distribution<int>(1, 10000)); }));
    }
    auto all = boost::when_all(tasks.begin(), tasks.end());
    auto r = all.then([](decltype(all) a) -> int
                      {
        auto vec = a.get();
        int r = 0;
        for (auto i = vec.begin(); i != vec.end(); i++)
        {
            std::cout << i->get() << std::endl;
            r += i->get();
        } 
        return r; });

    std::cout << r.get() << std::endl;
}