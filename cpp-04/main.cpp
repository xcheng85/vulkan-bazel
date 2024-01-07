#include <iostream>
#include <memory>
#include <spdlog/spdlog.h>

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
    RenderPass &operator=(const RenderPass &s) &
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
    // non const version
    // & only allow lvalue
    auto &operator[](size_t i) &
    {
        assert(i >= 0 && i < 2);
        if (i == 0)
        {
            return _numPasses;
        }
        else
        {
            // return 1; // auto& must be a lvalue
            return _numPasses;
        }
    }

    // works for both const and non const
    // & only allow lvalue, because it is a reference
    const auto &operator[](size_t i) const &
    {
        assert(i >= 0 && i < 2);
        if (i == 0)
        {
            return _numPasses;
        }
        else
        {
            // return 1; // auto& must be a lvalue
            return _numPasses;
        }
    }
    // for rvalue, non-reference
    const auto operator[](size_t i) &&
    {
        assert(i >= 0 && i < 2);
        if (i == 0)
        {
            return _numPasses;
        }
        else
        {
            // return 1; // auto& must be a lvalue
            return _numPasses;
        }
    }

    friend RenderPass operator+(RenderPass const &p1, RenderPass const &p2)
    {
        spdlog::info("operator+ is called!");
        return RenderPass(p1._numPasses + p2._numPasses, p1._debug);
    }

    friend auto &NumRenderPass(RenderPass &p);
    friend const auto &NumRenderPass(const RenderPass &p);

private:
    size_t _numPasses{0};
    bool _debug{true};
};

// best member accessor pattern
inline auto &NumRenderPass(RenderPass &p)
{
    return p._numPasses;
};

inline const auto &NumRenderPass(const RenderPass &p)
{
    return p._numPasses;
};

int main()
{
    RenderPass p{1, true}; // uniform init
    NumRenderPass(p)++;

    const auto &p2 = p;
    // const attribute
    // NumRenderPass(p2)++;

    spdlog::info(p[0]);
    // without const version operator, the following won't work
    spdlog::info(p2[0]);

    RenderPass p3{1, true}; // uniform init
    // The folowing bogus expression is protected using & at line34
    //(p + p3) = p;

    spdlog::info((p+p3)[0]);
    return 0;
}