#include <concepts>
#include <type_traits>
#include <iostream>
#include <format>

using namespace std;

// without fold expression
// recursive
template <typename T1, typename... Ts>
void f1(T1 &&arg1, Ts &&...args)
{
    // base case
    cout << arg1 << '\n';
    if constexpr (sizeof...(args) == 0)
    {
        return;
    }
    else
    {
        // perfect forwarding for universal reference + template + variadic
        f1(forward<Ts>(args)...);
    }
}

// folder version
template <typename... Ts>
void f2(Ts &&...args)
{
    // base case: folder + << operator
    // (cout << ... << args );

    // folder + binary + operator
    cout << (... + args ) << endl;
    // (cout << args << '\n', ...);
}

int main(int arg, char *argv[])
{
    auto lv = "leftvalue"s;
    // f1(lv, "rvalue"s);
    f2(lv, "rvalue"s);
}