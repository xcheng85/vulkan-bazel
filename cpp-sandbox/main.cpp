#include <iostream>
#include <spdlog/spdlog.h>

// packing multiple type arguments into type pack T_values
// class or typename
template <class... T_values>
class Base
{
public:
    // unpacking values and call somthing with multiple arguments
    virtual void something(T_values... values) = 0;
};

class Derived1 : public Base<int, short, double>
{
public:
    void something(int a, short b, double c) override;
};

class Derived2 : public Base<std::string, char>
{
public:
    void something(std::string a, char b) override;
};

template <typename... Args>
decltype(auto) fp(Args... values)
{
    spdlog::info("fp {}", sizeof...(Args));
}

template <typename... Args>
void printImpl(const Args &...tupleArgs)
{
    size_t index = 0;
    auto printElem = [&index](const auto &x)
    {
        if (index++ > 0)
            std::cout << ", ";
        std::cout << x;
    };

    (printElem(tupleArgs), ...);
}

template <typename... T_values>
class Thing
{
public:
    // store variadoc parms into tuple
    Thing(T_values &&...values) : _tuple(std::forward<T_values>(values)...)
    {
    }

    void store(T_values... values)
    {
        _tuple = std::tuple<T_values...>(values...);
    }

    decltype(auto) call()
    {
        return callHelper(std::forward<std::tuple<T_values...>>(_tuple),
                          std::index_sequence_for<T_values...>{});
    }

    // c++ 17
    decltype(auto) callv2()
    {
        return printTupleApplyFn(printImpl<T_values...>, _tuple);
    }

    // c++ 17
    template <typename Fn>
    decltype(auto) callv3(Fn fn)
    {
        return fpApply(_tuple, fn);
    }

protected:
    // template <typename Functor, typename... Arg, std::size_t... I>
    // decltype(auto) callHelper(Functor f, std::tuple<Arg...> &&params,
    //                           std::index_sequence<I...>)
    // {
    //     return f(std::get<I>(params)...);
    // }

    template <typename... Arg, std::size_t... I>
    decltype(auto) callHelper(std::tuple<Arg...> &&params,
                              std::index_sequence<I...>)
    {
        return fp(std::get<I>(params)...);
    }

    template <typename Functor, typename... Args>
    void printTupleApplyFn(Functor f, const std::tuple<Args...> &tp)
    {
        std::cout << "(";
        std::apply(f, tp); // <<
        std::cout << ")";
    }

    template <typename TupleT, typename Fn>
    void fpApply(TupleT &&tp, Fn &&fn)
    {
        std::apply(
            [&fn]<typename... T>(T &&...args)
            {
                (fn(std::forward<T>(args)), ...);
            },
            std::forward<TupleT>(tp));
    }

private:
    std::tuple<T_values...> _tuple;
};

int main()
{
    Thing t(1, 2, 3, 4);
    t.call();

    t.callv2();

    t.callv3([](auto &&x)
             { x *= 2; });
    return 0;
}