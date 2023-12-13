// #define VK_NO_PROTOTYPES    // for volk
#include <format>
#include <ranges>
#include <cassert>
#include <any>
// ranges::copy
#include <algorithm>
// #define VOLK_IMPLEMENTATION // for volk
#define VK_NO_PROTOTYPES // for volk
#include <volk.h>
#include <spdlog/spdlog.h>
#include "instance.h"
#include "physical_device.h"

using namespace std;
namespace Engine
{
    namespace Core
    {
        VulkanPhysicalDevice::VulkanPhysicalDevice(
            std::shared_ptr<IInstance> instance,
            VkPhysicalDevice deviceHandle) : _instance(instance)
        {
            spdlog::info("--> VulkanPhysicalDevice::VulkanPhysicalDevice");
            vkGetPhysicalDeviceFeatures(deviceHandle, &_features);
            vkGetPhysicalDeviceProperties(deviceHandle, &_properties);
            vkGetPhysicalDeviceMemoryProperties(deviceHandle, &_memoryProperties);

            spdlog::info("VulkanPhysicalDevice: {}", _properties.deviceName);

            uint32_t count = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(deviceHandle, &count, nullptr);
            _queueFamilyProperties = vector<VkQueueFamilyProperties>(count);
            vkGetPhysicalDeviceQueueFamilyProperties(deviceHandle, &count, _queueFamilyProperties.data());

            VK_CHECK(vkEnumerateDeviceExtensionProperties(deviceHandle, nullptr, &count, nullptr));
            _extensions = vector<VkExtensionProperties>(count);
            VK_CHECK(vkEnumerateDeviceExtensionProperties(deviceHandle, nullptr, &count, _extensions.data()));

            if (_extensions.size() > 0)
            {
                spdlog::info("VulkanPhysicalDevice: {} supports the following extensions:", _properties.deviceName);
                for (auto &extension : _extensions)
                {
                    spdlog::info("{}", extension.extensionName);
                }
            }

            spdlog::info("<-- VulkanPhysicalDevice::VulkanPhysicalDevice");
        }

        VulkanPhysicalDevice::~VulkanPhysicalDevice()
        {
        }

        bool VulkanPhysicalDevice::isSurfacePresentable(VkSurfaceKHR const &surface, uint32_t queueFamilyIndex) const
        {
            VkBool32 present_supported;
            VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(_handle, queueFamilyIndex, surface, &present_supported));
            return present_supported;
        }
    }
}
