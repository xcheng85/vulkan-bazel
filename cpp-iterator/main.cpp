#include <concepts>
#include <type_traits>
#include <iostream>
#include <format>
#include <vector>
#include <list>
#include <memory>
#include <functional>
#include <initializer_list>
#include <algorithm>
#include <numeric>
#include <exception>

using namespace std;

// implement a function requires object movable and copable
class IFoo
{
public:
    virtual void todo() noexcept = 0;
    virtual ~IFoo(){

    };
};

class Foo final : public IFoo
{
public:
    // rules of six: all default or all define
    explicit Foo(string id) : _id(move(id))
    {
    }
    virtual ~Foo()
    {
    }
    // // not copyable
    // Foo(const Foo&) = delete;
    // // not movable
    // Foo(Foo&&) = delete;
    Foo &operator=(const Foo &) = default;
    void todo() noexcept override
    {
    }

private:
    string _id{""};
};

// global stream operator
std::ostream &operator<<(std::ostream &of, const Foo &foo)
{
    of << "fff\n";
    return of;
}

// template function requires movable and copable

// // pack and unpack
// template <typename... T>
//     requires std::copyable<T...>
// void f1(T &&...)
// {
// }

// syntex simpler
template <std::copyable... T>
void f2(T &&...)
{
}

template <typename... T>
void f0(T &&...)
{
}

// lvalue only
template <typename T>
    requires std::copyable<T>
void f3(T &)
{
}

// constraints template by interface

template <derived_from<IFoo>... T>
void d0(T &&...value)
{
    // if constexpr ((std::is_object<T> && ...)) {

    // }
    // fold expression for variable template
    // constexpr is a must, so it could evaluate at compile time
    // constexpr bool t = is_void_v<T> == true;
    //(cout << format({}, value) << endl, ...);

    (std::cout << ... << value) << '\n';
}

class NotCopyable final : public IFoo
{
public:
    // rules of six: all default or all define
    NotCopyable() = delete;
    explicit NotCopyable(string &&id) : _id{move(id)}
    {
    }
    // explicit NotCopyable(int offset, initializer_list<string> ids) : _id{}
    // {
    //     _id = *(ids.begin());
    // }
    virtual ~NotCopyable()
    {
    }
    // // not copyable
    NotCopyable(const NotCopyable &src)
    {
        cout << "NotCopyable copy ctor\n";
        _id = src._id;
    }
    // // not movable
    NotCopyable(NotCopyable &&src) noexcept
    {
        cout << "NotCopyable move ctor\n";
        _id = src._id;
    }
    NotCopyable &operator=(const NotCopyable &src)
    {
        if (this == &src)
        {
            return *this;
        }
        cout << "NotCopyable copy assign\n";
        _id = src._id;
        return *this;
    }
    NotCopyable &operator=(NotCopyable &&src) noexcept
    {
        if (this == &src)
        {
            return *this;
        }
        cout << "NotCopyable move assign\n";
        _id = std::move(src._id);
        return *this;
    }
    NotCopyable &operator+(const NotCopyable &src)
    {
        _id += src._id;
        return *this;
    }
    NotCopyable &operator+(NotCopyable &&src)
    {
        _id += src._id;
        return *this;
    }
    NotCopyable &operator+=(const string &src)
    {
        _id += src;
        return *this;
    }
    NotCopyable &operator+=(string &&src)
    {
        _id += src;
        return *this;
    }
    bool operator==(const NotCopyable &src)
    {
        return _id == src._id;
    }
    bool operator==(NotCopyable &&src)
    {
        return _id == src._id;
    }
    bool operator<(const NotCopyable &src)
    {
        return _id < src._id;
    }
    bool operator<(NotCopyable &&src)
    {
        return _id < src._id;
    }
    void todo() noexcept override
    {
    }
    inline auto id() const
    {
        return _id;
    }

private:
    string _id{""};
};

int main(int arg, char *argv[])
{
    // cannot compile and error is super clear
    // cpp-iterator/main.cpp:21:6: note: constraints not satisfied
    // f1(Foo{});
    f0(Foo{"f0"});
    Foo f{"flvalue"};
    f3(f);
    // f3(Foo{})

    d0(Foo{"arg1"}, Foo{"arg2"});

    auto v = vector<int>{1, 2, 3};
    // after c++23
    // cout << format("{}", v) << endl;

    // custom class without copyable
    auto nc = NotCopyable{"1"s};
    auto c = vector<NotCopyable>(1, nc);
    auto c2 = vector<NotCopyable>{nc};

    // re-initialize pattern
    // c is not const
    // requires copyable
    c.assign({{NotCopyable{"id0"s}}});
    c.assign(10, NotCopyable{"id0"s});

    using NotCopyableVector = vector<NotCopyable>;

    auto p = make_unique<NotCopyableVector>(20, NotCopyable{"id0"s});
    c.swap(*p);
    cout << c.size() << endl;
    cout << p->size() << endl;

    // vector + reference
    auto r1 = make_unique<NotCopyable>("id0"s);
    vector<reference_wrapper<NotCopyable>> w{ref(*r1)};
    w[0].get() = NotCopyable{"id0_1"};
    cout << r1->id() << endl;

    // fp on vector: erase_if
    auto fp = make_unique<NotCopyableVector>();
    fp->assign({{NotCopyable{"id0"s}}, {NotCopyable{"id1"s}}});
    // perfect forwarding + two emplace
    auto id2Ref = fp->emplace_back("id2"s);
    auto idLastItr = fp->emplace(fp->end(), "id_last"s);

    // find + predict
    {
        auto itr = find_if(fp->begin(), fp->end(), [](auto e)
                           { return e.id() == "id0"s; });
        if (itr != fp->end())
        {
            cout << "id0 is found\n";
        }
    }

    {
        // requirs operator==
        auto itr = find(fp->begin(), fp->end(), NotCopyable{"id0"s});
        if (itr != fp->end())
        {
            cout << "id0 is found\n";
        }
    }

    // find_if_not
    {
        auto itr = find_if_not(fp->begin(), fp->end(), [](auto e)
                               { return e.id() == "id0"s; });
    }

    // find_first_of
    {
    }

    // search: find subsequence (first)
    {
        {
            // use member iterator
            vector<NotCopyable> subs{NotCopyable{"id0"s}, NotCopyable{"id1"s}};
            auto itr = search(fp->begin(), fp->end(), subs.begin(), subs.end());
            if (itr != fp->end())
            {
                cout << "v1: subsequence is found\n";
            }
        }
        {
            // use global
            vector<NotCopyable> subs{NotCopyable{"id0"s}, NotCopyable{"id1"s}};
            auto itr = search(cbegin(*fp), cend(*fp), begin(subs), end(subs));
            if (itr != cend(*fp))
            {
                cout << "v2: subsequence is found\n";
            }
        }
    }

    // erase by predict match
    cout << "before erase_if: " << fp->size() << endl;
    erase_if(*fp, [](auto e)
             { return e.id() == "id0"s; });
    cout << "after erase_if: " << fp->size() << endl;

    // accumulate (reducer)
    // requirs the operator+
    auto cacat{accumulate(fp->begin(), fp->end(), nc)};
    cout << "accumulate: " << cacat.id() << endl;

    // init in lamda capture
    auto lamda1{[tally = 0](NotCopyable e1, NotCopyable e2) mutable
                {
                    cout << format("{}", ++tally) << endl;
                    return e1;
                }};
    // pass ref to avoid copy from accumlate algorithm
    auto cacat2{accumulate(fp->begin(), fp->end(), nc, std::ref(lamda1))};
    cout << "accumulate2: " << cacat2.id() << endl;

    // try to avoid copy the vector for performance concern
    cout << "before copy with lvalue";
    auto copyWithLValue = c;
    cout << "before copy with rvalue";
    // rvalue does not have copy.
    auto copyWithRValue = vector<NotCopyable>(1, nc);

    // copy without back_inserter
    {
        vector<NotCopyable> src{NotCopyable{"id0"s}, NotCopyable{"id1"s}};
        // enough size
        vector<NotCopyable> dst(3, NotCopyable{"null"s});

        copy(cbegin(src), cend(src), begin(dst));
        for (const auto &e : dst)
        {
            cout << format("{}\n", e.id());
        }
    }
    // be careful when you have more space and use copy_backwards
    {
        vector<NotCopyable> src{NotCopyable{"id0"s}, NotCopyable{"id1"s}};
        // enough size
        vector<NotCopyable> dst(3, NotCopyable{"null"s});

        // needs iterator have --. vector is fine
        copy_backward(cbegin(src), cend(src), end(dst));
        for (const auto &e : dst)
        {
            cout << format("{}\n", e.id());
        }
    }

    // selective copy
    {
        vector<NotCopyable> src{NotCopyable{"id0"s}, NotCopyable{"id1"s}};
        // you may over allocate size, you can erase
        vector<NotCopyable> dst(1, NotCopyable{"null"s});

        // needs iterator have --. vector is fine
        copy_if(cbegin(src), cend(src), begin(dst), [](auto e)
                { return e.id() == "id0"s; });
        for (const auto &e : dst)
        {
            cout << format("{}\n", e.id());
        }
    }

    // in-place replace and transform
    {
        vector<NotCopyable> src{NotCopyable{"id0"s}, NotCopyable{"id1"s}};
        // you may over allocate size, you can erase
        vector<NotCopyable> dst(1, NotCopyable{"null"s});
        replace_if(
            begin(src), end(src), [](auto e)
            { return e.id() == "id0"s; },
            NotCopyable{"id_replace"s});
        for (const auto &e : src)
        {
            cout << format("in-place replace: {}\n", e.id());
        }
    }
    // replace to dst
    {
        vector<NotCopyable> src{NotCopyable{"id0"s}, NotCopyable{"id1"s}};
        // you may over allocate size, you can erase
        vector<NotCopyable> dst(2, NotCopyable{"null"s});
        replace_copy_if(
            begin(src), end(src), begin(dst), [](auto e)
            { return e.id() == "id0"s; },
            NotCopyable{"id_replace"s});
        for (const auto &e : dst)
        {
            cout << format("replace in dst: {}\n", e.id());
        }
    }
    // in-place transform
    {
        // need operator+=
        vector<NotCopyable> src{NotCopyable{"id0"s}, NotCopyable{"id1"s}};
        transform(
            cbegin(src), cend(src), begin(src), [](auto e)
            { return e += "transform"; });
        for (const auto &e : src)
        {
            cout << format("in-place transform: {}\n", e.id());
        }
    }
    // transform to another
    {
        vector<NotCopyable> src{NotCopyable{"id0"s}, NotCopyable{"id1"s}};
        // not enough size (crash)
        vector<NotCopyable> dst{2, NotCopyable{"id0"s}};
        transform(
            cbegin(src), cend(src), begin(dst), [](auto e)
            { return e += "transform"; });
        // try
        // {
        //     transform(
        //         cbegin(src), cend(src), begin(dst), [](auto e)
        //         { return e += "transform"; });
        // }
        // catch (std::exception e)
        // {
        //     cout << e.what() << endl;
        // }
        for (const auto &e : dst)
        {
            cout << format("dst transform: {}\n", e.id());
        }

        // move element: remove

        // move element: unique
        // input sorted with dups
        {
            vector<NotCopyable> src{NotCopyable{"id0"s}, NotCopyable{"id1"s}, NotCopyable{"id1"s}, NotCopyable{"id2"s}};
            auto itr = unique(src.begin(), src.end());
            src.erase(itr, src.end());
            for (const auto &e : src)
            {
                cout << format("in place unique: {}\n", e.id());
            }
        }

        // quick sort: pivot, group all element small than pivot and larger than pivot
        // nthelement
        {
            vector<NotCopyable> src{NotCopyable{"id2"s}, NotCopyable{"id0"s}, NotCopyable{"id3"s}, NotCopyable{"id1"s}};
            // requires operator <
            nth_element(begin(src), begin(src) + 2, end(src));

            for (const auto &e : src)
            {
                cout << format("in place unique: {}\n", e.id());
            }
        }

        // search
        {
            vector<NotCopyable> src{NotCopyable{"id2"s}, NotCopyable{"id0"s}, NotCopyable{"id4"s}, NotCopyable{"id1"s}};
            sort(begin(src), end(src));
            auto itr = lower_bound(begin(src), end(src), NotCopyable{"id3"s});
            if (itr != src.end())
            {
                // id4
                cout << format("lowerbound of id3 is: {}\n", itr->id());
                // insert id3 here
                src.insert(itr, NotCopyable{"id3"s});
                for (const auto &e : src)
                {
                    cout << format("after lowbound insert: {}\n", e.id());
                }
            }
            else
            {
                cout << format("all the elements less than id3\n");
            }
        }

        // merge two containers
        {
            list<NotCopyable> src1{NotCopyable{"id2"s}, NotCopyable{"id0"s}};
            vector<NotCopyable> src2{NotCopyable{"id3"s}, NotCopyable{"id1"s}};
            vector<NotCopyable> dst(4, NotCopyable{"n/a"s});
            list<NotCopyable> dst2(4, NotCopyable{"n/a"s});

            // not support list
            // sort(begin(src1), end(src1));
            src1.sort();
            sort(begin(src2), end(src2));
            // not in place, const
            merge(cbegin(src1), cend(src1), begin(src2), end(src2), begin(dst));
            merge(cbegin(src1), cend(src1), begin(src2), end(src2), begin(dst2));
            for (const auto &e : dst2)
            {
                cout << format("after merge: {}\n", e.id());
            }
        }
    }
}