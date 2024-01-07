#include <iostream>
#include <memory>
#include <spdlog/spdlog.h>

// reference: https://medium.com/pranayaggarwal25/custom-deleters-with-shared-ptr-and-unique-ptr-524bb7bd7262
// mimic c-style library
class Environment
{
public:
    static Environment *createEnvironment()
    {
        spdlog::info("createEnvironment is called!");
        return new Environment();
    }

    static void terminateEnvironment(Environment const *ptr)
    {
        spdlog::info("terminateEnvironment is called!");
        delete ptr;
    }
};

// template< typename T >
// struct array_deleter
// {
//   void operator ()( T const * p)
//   {
//     delete[] p;
//   }
// };

struct EnvironmentDeleter
{
    void operator()(Environment const *env)
    {
        Environment::terminateEnvironment(env);
    }
};

int main()
{
    // mark_shared does not work with custom deleter
    std::shared_ptr<Environment> sp(Environment::createEnvironment(), EnvironmentDeleter{});
    // use lamda
    std::shared_ptr<Environment> sp2(Environment::createEnvironment(), [](Environment *e)
                                     { Environment::terminateEnvironment(e); });

    // for unique_ptr
    auto deleter = [](Environment *e)
    {
        Environment::terminateEnvironment(e);
    };
    std::unique_ptr<Environment, decltype(deleter)> up(Environment::createEnvironment(), deleter);
    return 0;
}