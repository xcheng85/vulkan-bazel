#include <string>
#include <iostream>
#include <chrono>
#include <exception>
#include <algorithm>
#include <spdlog/spdlog.h>

#include "object-pool.h"

using namespace std::chrono_literals;
using namespace std;

using BrickPool = core::ObjectPool<core::Brick>;

// strategy pattern
// inversion of control
class VolumeRayCaster
{
public:
    explicit VolumeRayCaster(BrickPool &pool, size_t numSteps)
        : _pool(pool), _numSteps(numSteps) {}
    virtual ~VolumeRayCaster() = default;

    void start()
    {
        // ++i is better i++, avoid create tmp obj
        for (size_t i{0}; i < _numSteps; ++i)
        {
            // explicitly call the ctor
            auto brick{_pool.acquireObject(std::to_string(i))};
            spdlog::info(brick->id());
        }

        auto brick{_pool.acquireObject()};
    }

private:
    BrickPool &_pool;
    size_t _numSteps{0};
};

int main()
{
    core::Vector3f axis{};
    std::cout << axis << std::endl;

    // normally done in ioc container
    BrickPool brickPool;
    // performance measurement
    auto start1{chrono::steady_clock::now()};

    VolumeRayCaster rc{brickPool, 500};
    rc.start();

    auto end1{chrono::steady_clock::now()};
    auto diff1{end1 - start1};
    spdlog::info("{}", chrono::duration<double, milli>(diff1).count());
}