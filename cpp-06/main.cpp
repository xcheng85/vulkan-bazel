#include <iostream>
#include <memory>
#include <spdlog/spdlog.h>

class RenderPass
{
public:
    explicit RenderPass(size_t numPasses = 1, bool debug = false)
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

inline void Logging(RenderPass &p)
{
    spdlog::info("Logging is called!");
};

inline void Logging(RenderPass &&p)
{
    spdlog::info("Logging Universal is called!");
};

// auto type deduction stripped out reference
template <typename Container>
auto Access(Container &c, size_t idx)
{
    return c[idx];
};

// auto type deduction stripped out reference
template <typename Container>
decltype(auto) AccessV2(Container &c, size_t idx)
{
    return c[idx];
};

int main()
{
    auto pw = std::make_unique<RenderPass>(2, true);
    // // init capture with move
    // auto func = [pw = std::move(pw)]
    // {
    //     spdlog::info(pw->operator[](0));
    // };

    // func();

    decltype(auto) x = 1;
    // type deductioni from initializer using decltype rules
    decltype(auto) y = std::move(x);
    // z1 is size_t
    decltype(auto) z1 = Access(*pw, 0);
    // z2 is size_t&
    decltype(auto) z2 = AccessV2(*pw, 0);

    // generic lamda
    // 1. only accepet lvalue
    {
        spdlog::info("generic lamda 1");
        auto f = [](auto x)
        {
            spdlog::info(x[0]);
        };
        f(*pw);
    }
    // 2. accept ravalue + perfect fowarding for rvalue
    {
        spdlog::info("generic lamda 2");
        auto f = [](auto &&x)
        {
            // spdlog::info(x[0]);
            Logging(std::forward<decltype(x)>(x));
        };
        // rvalue, perfect forwarding to Logging
        f(RenderPass(2, false));
    }

    // 3. accept ravalue + not perfect fowarding for rvalue
    {
        spdlog::info("generic lamda 3");
        auto f = [](auto &&x)
        {
            // rvalue become lvalue, passing to the lvalue logging version
            Logging(x);
        };
        // rvalue, perfect forwarding to Logging
        f(RenderPass(2, false));
    }

    // if pw is moved, this will segmentation fault
    // spdlog::info(pw->operator[](0));
}