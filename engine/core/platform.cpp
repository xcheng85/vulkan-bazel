#include <spdlog/spdlog.h>
// #include <format>
#include "platform.h"
#include "window.h"
#include "application.h"

using namespace std;
namespace Engine
{
    namespace Core
    {
        void IPlatform::main_loop()
        {
            while (!_window->ShouldClose())
            {
                update();

                _window->ProcessEvents();
            }
        }

        void IPlatform::update()
        {
            _application->update();
        }

        void IPlatform::terminate()
        {
            _application->terminate();
        }

        void IPlatform::close()
        {
            _window->Close();
        }

        void IPlatform::resize(uint32_t width, uint32_t height)
        {
            spdlog::info("--> IPlatform::resize");
            _application->resize(width, height);
            spdlog::info("--> IPlatform::resize");
        }

        UnixPlatform::UnixPlatform(std::shared_ptr<IWindow> window, std::shared_ptr<IApplication> application)
        {
            cout << format("--> UnixPlatform::UnixPlatform") << std::endl;
            // cannot initialzie parents member in the ctor init list
            this->_window = window;
            this->_application = application;
            this->_window->usePlatform(this);
            cout << format("<-- UnixPlatform::UnixPlatform") << std::endl;
        }

        UnixPlatform::~UnixPlatform()
        {
        }
    }
}
