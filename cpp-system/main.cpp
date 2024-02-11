#include <thread>
#include <future>
#include <exception>
#include <vector>
#include <iostream>
#include <format>
#include <random>
#include <chrono>

using namespace std;

void updateInput() {
    //cout << "updateInput\n";
}

void renderingFrame() {
    //cout << "renderingFrame\n";
}

void readTile(int clientId, promise<int> p)
{
    // mimic random exception
    random_device seeder; // functor
    const auto seed{seeder.entropy() ? seeder() : time(nullptr)};
    // use seeder for the software engin
    mt19937 engine{seed};
    // distribution
    uniform_int_distribution<int> distribution{1, 10};
    auto randomSeconds = distribution(engine);
    this_thread::sleep_for(chrono::seconds(randomSeconds));
    if (clientId & 1 == 0)
    {
        // mimic exception in separate thread
        p.set_exception(std::make_exception_ptr("cannot connect to tile server"));
    }
    else
    {
        p.set_value(clientId);
    }
}

int main()
{
    // step1: create a promise (like create a channle in golang)
    constexpr int NUM_THREADS = 4;
    promise<int> readTilePromise[NUM_THREADS];
    std::future<int> readTileFuture[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        readTileFuture[i] = readTilePromise[i].get_future();
    }

    // assuming we want to launch 4 threads for readTiles
    vector<jthread> readTileThreads;
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        // create 4 threads
        readTileThreads.emplace_back(readTile, i, move(readTilePromise[i]));
    }

    // polling the result from 4 threads with loop
    // like wait group
    bool isFinished[NUM_THREADS];
    for(int i = 0; i < NUM_THREADS; ++i) {
        isFinished[i] = false;
    }
    int ct = 0;
    while (true)
    {
        for (int i = 0; i < NUM_THREADS; ++i)
        {
            if(isFinished[i])
                continue;
            auto status = readTileFuture[i].wait_for(0s);
            if (status == future_status::ready)
            {
                isFinished[i] = true;
                ct++;
                // blocking call
                int readTileResult{readTileFuture[i].get()};
                cout << format("threadId: {} done with value: {}\n", i, readTileResult);
            }
        }
        updateInput();
        renderingFrame();
        if(ct == NUM_THREADS) {
            break;
        }
    }
    return 0;
}