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
#include "physical_device_manager.h"
#include "physical_device.h"
#include "utils.h"

using namespace std;
namespace Engine
{
    namespace Core
    {
        VulkanPhysicalDeviceManager::VulkanPhysicalDeviceManager(
            std::shared_ptr<IInstance> instance)
        {
            spdlog::info("--> VulkanPhysicalDeviceManager::VulkanPhysicalDeviceManager");
            uint32_t count{0};
            auto const &vkHandle = instance->getVkHandle();
            VK_CHECK(vkEnumeratePhysicalDevices(vkHandle, &count, nullptr));

            std::vector<VkPhysicalDevice> vkPDevices;
            vkPDevices.resize(count);
            _pDevices.resize(count);
            VK_CHECK(vkEnumeratePhysicalDevices(vkHandle, &count, vkPDevices.data()));

            // c++23 iterate with index, golang for range
            for (auto const &[index, vkPDevice] : vkPDevices | std::views::enumerate)
            {
                _pDevices[index] = std::make_unique<VulkanPhysicalDevice>(instance, vkPDevice);
            }
            spdlog::info("<-- VulkanPhysicalDeviceManager::VulkanPhysicalDeviceManager");
        }

        VulkanPhysicalDeviceManager::~VulkanPhysicalDeviceManager()
        {
            
        }
    }
}
