#include <sys/resource.h>
#include <iostream>
#include <chrono>
#include <functional> // Include for std::function

std::size_t get_memory_usage()
{
    struct rusage r_usage;
    getrusage(RUSAGE_SELF, &r_usage);
    return r_usage.ru_maxrss; // ru_maxrss is in kilobytes
}

template <typename Func>
struct ResourceUsage
{
    std::size_t memory_usage; // in kilobytes
    long long elapsed_time;   // in microseconds
};

template <typename Func>
ResourceUsage<Func> measure_resource_usage(Func func)
{
    // Measure start time
    auto start_time = std::chrono::steady_clock::now();

    // Measure start memory usage
    std::size_t start_memory = get_memory_usage();

    // Execute the provided function
    func();

    // Measure end time
    auto end_time = std::chrono::steady_clock::now();

    // Measure end memory usage
    std::size_t end_memory = get_memory_usage();

    // Calculate elapsed time
    auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    // Calculate memory usage difference
    std::size_t memory_usage = end_memory - start_memory;

    return {memory_usage, elapsed_time};
}