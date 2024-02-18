#pragma once

#include <random>
#include <time.h>
#include <functional>

using namespace std;

// modern c++
// random header
// engine and distribution

// physical engine: random_device, need hardware support
// software engine: three, need seed
// engine: implemented as class template

// using alias: mt19937

namespace rand_algo
{
    // using functor
    class generateNumberBasedOnProbabiliyFunctor
    {
    public:
        // assumption: px + py + pz = 100
        int operator()(int x, int y, int z, int px, int py, int pz)
        {
            random_device seeder; // functor
            const auto seed{seeder.entropy() ? seeder() : time(nullptr)};
            // use seeder for the software engin
            mt19937 engine{seed};
            // distribution
            uniform_int_distribution<int> distribution{1, 100};
            // return distribution(engine);
            // simplify with std::bind utility
            auto randomGenerator{bind(distribution, engine)};
            auto r = randomGenerator();

            if (r <= px) {
                return x;
            }
            if (r <= px + py) {
                return y;
            }
            return z;
        }
    };
}