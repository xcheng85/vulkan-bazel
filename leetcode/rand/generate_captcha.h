#pragma once

#include <random>
#include <time.h>
#include <functional>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

namespace rand_algo
{
    class CapchaCommand
    {
    public:
        CapchaCommand()
        {
            // Initialize seed randomly
            srand(time(0));
        }
        string operator()(int length)
        {
            string captcha = "";
            
            for(int i = 0; i < length; i++) {
                auto idx = rand() % _candidates.size();
                captcha += _candidates[idx];
            }
            return captcha;
        }

    protected:
    private:
        string _candidates{
            "abcdefghijklmnopqrstuvwxyzABCDEFGHI"
            "JKLMNOPQRSTUVWXYZ0123456789"s};
    };
}