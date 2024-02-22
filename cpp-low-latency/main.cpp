#include <chrono>
#include <format>
#include <vector>
#include <deque>
#include "./memory-pool.h"
#include "./lockfree-queue.h"
#include "./thread-launcher.h"
#include "./network/tcp-server.h"
#include "./iterator-mt.h"
#include "./stack-mt.h"
#include "./pub-sub.h"

using namespace std;
using namespace cpp_low_latency;
using namespace cpp_low_latency::network;
using namespace cpp_low_latency::mt;

struct tile2D
{
    size_t sizex;
    size_t sizey;
};

void readTile(const std::string &url, int intervalSeconds)
{
    while (1)
    {
        // cout << format("readTile from {} on cpu: {}\n", url, sched_getcpu());
        std::this_thread::sleep_for(chrono::seconds(intervalSeconds));
    }
}

void updateInput()
{
     cout << "updateInput\n";
}

void renderingFrame()
{
    cout << "renderingFrame\n";
}

int main()
{
    vector<int> dq;
    for(int i = 0; i < 100; ++i){
        dq.push_back(i);
    }

    auto r = AccumulateMT(begin(dq), end(dq), 0);
    cout << r << endl;

    // thread-safe stack
    TestStackMt<int>();

    // pub-sub
    TestStreamPubSub();

    // cpp_low_latency::MemoryPool<tile2D> mp{64 * 64};
    // cpp_low_latency::LockFreeQueue<tile2D> queue(100);
    auto t1 = launchThread(0, "readTile#0", readTile, "grpc.server.com:443", 6);
    auto t2 = launchThread(1, "readTile#1", readTile, "grpc.server.com:4431", 3);

    // const std::string iface = "lo";
    // const std::string ip = "127.0.0.1";
    // const int port = 8888;

    SocketConfig sfg{
        ""s,
        "lo"s,
        3000};
    sfg.isListening = true; // key
    auto cb1 = [&](TcpSocket *socket, timeval rx_time) noexcept
    {
        string msg = move(socket->recv());
        cout << format("{}\n", msg);
    };
    auto cb2 = [&]() noexcept
    {
        cout << format("recv done for this pool!\n");
    };
    auto tcpSocket = make_unique<TcpSocket>(make_unique<Socket>(sfg), cb1);
    auto tcpServer = make_unique<TcpServer>(move(tcpSocket), cb1, cb2);

    // sfg for client
    std::vector<unique_ptr<TcpSocket>> clients;
    SocketConfig clientsfg{
        "127.0.0.1"s,
        "lo"s,
        3000};
    constexpr int numClients = 10;
    for (size_t i = 0; i < numClients; ++i)
    {
        clients.emplace_back(make_unique<TcpSocket>(make_unique<Socket>(clientsfg), cb1));
        tcpServer->poll();
    }

    while (true)
    {
        updateInput();
        renderingFrame();

        for (size_t i = 0; i < numClients; ++i)
        {
            const std::string msg{format("From Client: {}", i)};
            clients[i]->sendToBuffer(msg.data(), msg.length());
            clients[i]->sendAndRecv();
            // main thread
            std::this_thread::sleep_for(500ms);
            tcpServer->poll();
            tcpServer->sendAndRecv();
        }
    }
}
