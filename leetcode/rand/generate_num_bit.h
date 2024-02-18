#pragma once

#include <random>
#include <time.h>
#include <functional>

using namespace std;

namespace rand_algo
{
    // using functor
    class generate7525OnBitOpFunctor
    {
    public:
        generate7525OnBitOpFunctor()
        {
            // Initialize seed randomly
            srand(time(0));
        }
        int operator()()
        {
            // 75% 1 and 25%0
            return rand50() | rand50();
            // // 75% 0 and 25%1
            // return rand50() & rand50();
        }

    protected:
        int rand50()
        {
            return rand() & 1;
        }
    };
}