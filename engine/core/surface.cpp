// #define VK_NO_PROTOTYPES    // for volk
#include <format>
#include <ranges>
#include <cassert>
// ranges::copy
#include <algorithm>
// #define VOLK_IMPLEMENTATION // for volk
#define VK_NO_PROTOTYPES // for volk
#include <volk.h>
#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "instance.h"
#include "surface.h"
#include "window.h"

using namespace std;
namespace Engine
{
    namespace Core
    {
        VulkanSurface::VulkanSurface(
            std::shared_ptr<IWindow> window,
            std::shared_ptr<IInstance> instance) : _instance(instance)
        {
            spdlog::info("--> VulkanSurface::VulkanSurface");
            auto const &handle = window->getWindow();
            // safe check for std::any
            if (handle.has_value())
            {
                // rtii for std::any
                auto const &type = handle.type();
                auto const &typeName = string{type.name()};
                // substring member function version
                if (typeName.find("GLFWwindow") != typeName.npos)
                {
                    auto glfw = std::any_cast<GLFWwindow *>(handle);
                    auto const &vkInstance = instance->getVkHandle();
                    VK_CHECK(glfwCreateWindowSurface(vkInstance, glfw, NULL, &_handle))
                } else {
                    throw runtime_error(format("Window: {} is not supported!", typeName));
                }
            }
            spdlog::info("<-- VulkanSurface::VulkanSurface");
        }

        VulkanSurface::~VulkanSurface()
        {
            auto const &vkInstance = _instance->getVkHandle();
            vkDestroySurfaceKHR(vkInstance, _handle, nullptr);
        }
    }
}
