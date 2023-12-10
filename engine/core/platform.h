#pragma once
// #define VK_NO_PROTOTYPES // for volk
// #include <volk.h>
#include <stdint.h>
#include <optional>
#include <string>
#include <vector>
#include <memory>
#include <iostream>

using namespace std;

namespace Engine
{
    namespace Core
    {
        class IWindow;
        class IApplication;
        class IPlatform
        {
        public:
            virtual ~IPlatform() = default;
            void main_loop();
            void update();
            void terminate();
            virtual void close();
            virtual void resize(uint32_t width, uint32_t height);

        protected:
            std::shared_ptr<IWindow> _window;
            std::shared_ptr<IApplication> _application;
        };

        class UnixPlatform : public IPlatform
        {
        public:
            explicit UnixPlatform(std::shared_ptr<IWindow>, std::shared_ptr<IApplication>);
            virtual ~UnixPlatform();
        };
    }
};
