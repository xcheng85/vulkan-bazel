#pragma once
#include <iostream>
#include <atomic>
#include <thread>
#include <unistd.h>
#include <sys/syscall.h>
#include <memory>
namespace cpp_low_latency
{
    using namespace std;
    // The std::thread library is implemented on top of pthreads in an environment
    // supporting pthreads (for example: libstdc++).

    // cpu info
    // cat /proc/cpuinfo
    // lscpu
    // unsigned num_cpus = std::thread::hardware_concurrency();

    // setting CPU affinity per thread
    // On Linux, we can use the pthread-specific pthread_setaffinity_np function. H
    inline auto setThreadCoreAffinity(int cpu_id, jthread::native_handle_type handle) noexcept
    {
        cpu_set_t cpuset;

        CPU_ZERO(&cpuset);
        CPU_SET(cpu_id, &cpuset);
        // != 0, failure
        return (pthread_setaffinity_np(handle, sizeof(cpu_set_t), &cpuset) == 0);
    }

    template<typename F, typename... Args>
    auto launchThread(int cpuId, const std::string& threadName, F&& func, Args&&... args) noexcept {
        // main thread
        // use atomic for lock-free communication between main thread and new thread
        atomic<bool> running{false}, inited{true};
        // reference capture all
        unique_ptr<jthread> t {make_unique<jthread>([&](){
            running = true;
            std::forward<F>(func)(forward<Args>(args)...);
        })};
        auto pinningCpuCore = setThreadCoreAffinity(cpuId, t->native_handle());
        if(!pinningCpuCore) {
            throw std::runtime_error("Failed to set cpu affinity !");
        }
        return t;
    }
}