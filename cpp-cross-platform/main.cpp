#include <iostream>

using namespace std;

enum class Platform
{
    Darwin,
    Linux,
    Windows
};

#ifdef __darwin__
constexpr Platform BuildPlatform = Platform::Darwin;
#elif __linux__
constexpr Platform BuildPlatform = Platform::Linux;
#elif __WIN32
constexpr Platform BuildPlatform = Platform::Windows;
#endif

int main()
{
    if constexpr (BuildPlatform == Platform::Darwin)
    {
        cout << "Running on Mac\n"; 
    }
    else if constexpr (BuildPlatform == Platform::Linux)
    {
        cout << "Running on Linux\n";
    }
    else if constexpr (BuildPlatform == Platform::Windows)
    {
        cout << "Running on Windows\n";
    }
}