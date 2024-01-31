#include <iostream>
#include <spdlog/spdlog.h>

class FrameBuffer
{
public:
    // FrameBuffer() = default; // using default ctor, will disable delgated ctor
    size_t numAttachements;
};

class RenderPass
{
public:
    RenderPass(size_t numPasses = 1, bool debug = false)
        : _numPasses{numPasses}, _debug{debug}
    {
        spdlog::info("ctor1!");
    }

    RenderPass(const RenderPass &src) : RenderPass(src._numPasses, src._debug)
    {
        spdlog::info("ctor2!");
    }

    // move constructor
    // no memory allocation, no except risk
    RenderPass(RenderPass &&src) noexcept
        : _numPasses{src._numPasses}, _debug{src._debug}
    {
        spdlog::info("move ctor!");
    }

    // constructor delegation
    RenderPass(int numPasses = 1)
        : RenderPass(numPasses, true)
    {
    }

    // assignment operator
    RenderPass &operator=(const RenderPass &s)
    {
        spdlog::info("copy ctor is called!");
        if (this == &s)
        {
            return *this;
        }
        _numPasses = s._numPasses;
        _debug = s._debug;
        return *this;
    }

    // move assignment operator
    // no memory allocation, no exception
    RenderPass &operator=(RenderPass &&s) noexcept
    {
        spdlog::info("move ctor is called!");
        _numPasses = s._numPasses;
        _debug = s._debug;
        return *this;
    }

    // RenderPass &operator=(const double &s)
    // {
    //     _numPasses = s;
    //     _debug = true;
    //     return *this;
    // }

    friend RenderPass operator+(RenderPass const &p1, RenderPass const &p2)
    {
        spdlog::info("operator+ is called!");
        return RenderPass(p1._numPasses + p2._numPasses, p1._debug);
    }

    size_t _numPasses{0};
    bool _debug{true};
};

void inline logging(RenderPass p)
{
}

int main()
{
    // logging({2}); // cannot compile due to explict
    RenderPass p{1, true}; // uniform init

    // cannot apply delegated init due to user-defined ctor
    // RenderPass p2
    //     ._numPasses = 1,
    //     ._debug = true};
    FrameBuffer p2{
        .numAttachements = 1};

    // p = 2; // // cannot compile due to explict
    // due to compiler's Copy Ellision opt, no copy ctor is called
    RenderPass p_copy(p);
    RenderPass p_copy2 = p;
    auto p_copy3 = p_copy + p_copy2;

    auto p_move = std::move(p_copy2);
    return 0;
}