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
#include "surface.h"
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

        const IPhysicalDevice *VulkanPhysicalDeviceManager::getPhysicalDevice(ISurface const *surface) const
        {
            assert(!_pDevices.empty());

            for (auto &pDevice : _pDevices)
            {
                // const auto &vkHandle = pDevice->getVkHandle();
                const auto &imp = dynamic_cast<VulkanPhysicalDevice *>(pDevice.get());
                assert(imp);

                if (imp->properties().deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                {
                    const auto &queueCount = imp->queueFamilyProperties().size();
                    for (uint32_t qId = 0; qId < queueCount; qId++)
                    {
                        // device support surface present
                        if (imp->isSurfacePresentable(surface->getVkHandle(), qId))
                        {
                            return pDevice.get();
                        }
                    }
                }
            }
            throw std::runtime_error{format("could not find device that could present the surface")};
        }
    }
}
