include_directories(${CppConcurrency_SOURCE_DIR}/includes)

add_executable(StdAsyncDemo StdAsyncDemo.cpp)
target_link_libraries(StdAsyncDemo CommonUtils)

add_executable(QuickSort QuickSort.cpp)
target_link_libraries(QuickSort CommonUtils)

add_executable(StdPackageTaskDemo StdPackagedTaskDemo.cpp)
target_link_libraries(StdPackageTaskDemo CommonUtils)

add_executable(StdPromiseDemo StdPromiseDemo.cpp)
target_link_libraries(StdPromiseDemo CommonUtils)

add_executable(ThreadSafeQueue ThreadSafeQueue.cpp)
target_link_libraries(ThreadSafeQueue CommonUtils)

add_executable(PromiseWhenAllDemo PromiseWhenAllDemo.cpp)
target_link_libraries(PromiseWhenAllDemo CommonUtils Boost::thread)