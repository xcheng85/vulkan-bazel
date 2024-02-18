#pragma once

#include <random>
#include <time.h>
#include <functional>

using namespace std;

namespace rand_algo
{
    // using functor
    // 2 * rand2() + rand2()
    // 0 0  0
    // 0 1  1
    // 1 1  3   abort and retry
    // 1 0  2
    class generateThreeEqualFunctor
    {
    public:
        generateThreeEqualFunctor()
        {
            // Initialize seed randomly
            srand(time(0));
        }
        int operator()()
        {
            auto r = 2 * rand2() + rand2();
            if (r == 3)
            {
                return this->operator()();
            }
            return r;
        }

    protected:
        int rand2() // 0 and 1 equal probablity
        {
            return rand() & 1;
        }
    };
}