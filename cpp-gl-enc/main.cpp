#include <exception>
#include <memory>
#include "graphicswindow.h"

int main(int argc, char **argv)
{
    auto gWin = std::make_unique<GraphisWindowRAII>("glx");

    while (true)
    {
        
    }

    return 0;
}