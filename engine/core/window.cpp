#include <format>
#include <iostream>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "window.h"
#include "platform.h"

using namespace std;
namespace Engine
{
    namespace Core
    {
        // unnamed namespace to avoid naming clash for function symbol such as:
        namespace
        {
            void error_callback(int error, const char *description)
            {
                spdlog::info("GLFW Error (code {}): {}", error, description);
            }
        }

        void resize(GLFWwindow *window, int width, int height)
        {
            spdlog::info("--> GlfwWindow::resize");
            if (auto platform = reinterpret_cast<IPlatform *>(glfwGetWindowUserPointer(window)))
            {
                platform->resize(width, height);
            }
            spdlog::info("<-- GlfwWindow::resize");
        }

        void keyCB(GLFWwindow *window, int key, int /*scancode*/, int action, int /*mods*/)
        {
            spdlog::info("--> GlfwWindow::keyCB");
            spdlog::info("<-- GlfwWindow::keyCB");
        }

        void IWindow::usePlatform(const IPlatform *const p)
        {
            this->_platform = const_cast<IPlatform *>(p);
        }

        GlfwWindow::GlfwWindow()
        {
            spdlog::info("--> GlfwWindow::GlfwWindow");
            if (!glfwInit())
            {
                throw std::runtime_error("GLFW couldn't be initialized.");
            }

            glfwSetErrorCallback(error_callback);
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

            // to be extended
            _window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

            if (!_window)
            {
                throw std::runtime_error("Couldn't create glfw window.");
            }

            glfwSetWindowSizeCallback(_window, resize);
            glfwSetKeyCallback(_window, keyCB);

            spdlog::info("<-- GlfwWindow::GlfwWindow");
        }

        GlfwWindow::~GlfwWindow()
        {
            glfwDestroyWindow(_window);

            glfwTerminate();
        }

        bool GlfwWindow::ShouldClose()
        {
            return glfwWindowShouldClose(_window);
        }

        void GlfwWindow::Close()
        {
            // correlate with ShouldClose()
            glfwSetWindowShouldClose(_window, GLFW_TRUE);
        }

        void GlfwWindow::ProcessEvents()
        {
            glfwPollEvents();
        }

        void GlfwWindow::usePlatform(const IPlatform *const p)
        {
            IWindow::usePlatform(p);
            glfwSetWindowUserPointer(_window, const_cast<IPlatform *>(p));
        }

        std::any GlfwWindow::getWindow()
        {
            return this->_window;
        }
    }
}