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
    class ShuffleCards
    {
    public:
        ShuffleCards() : _cards(NUM_CARDS)
        {
            // Initialize seed randomly
            srand(time(0));

            // multiple ways to filling cards
            // 1.itoa
            std::iota(_cards.begin(), _cards.end(), 0); // _cards will become: [0..51]

            // // 2. transform with lamda
            // int idx = 0;
            // transform(begin(_cards), end(_cards), begin(_cards), [&idx](auto c) {
            //     return idx++;
            // });
        }
        void operator()()
        {
            for (int i = NUM_CARDS - 1; i >= 0; i--)
            {
                int r = rand() % (i + 1);
                swap(_cards[i], _cards[r]);
            }
            for (const auto& c : _cards) {
                cout << c << "\n";
            }
        }

    protected:
    private:
        constexpr static int NUM_CARDS = 52;
        vector<int> _cards;
    };
}