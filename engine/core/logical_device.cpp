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
#include "logical_device.h"
#include "physical_device.h"
#include "physical_device_manager.h"
#include "surface.h"

using namespace std;
namespace Engine
{
    namespace Core
    {
        VulkanLogicDeviceSettings::VulkanLogicDeviceSettings(
            const bool graphicsQueueHighPriority,
            std::vector<std::string> const &requiredPhysicalDeviceExtensions)
            : _graphicsQueueHighPriority(graphicsQueueHighPriority),
              _requiredPhysicalDeviceExtensions(requiredPhysicalDeviceExtensions)
        {
        }

        VulkanLogicalDevice::VulkanLogicalDevice(
            std::shared_ptr<IPhysicalDeviceManager> manager,
            std::shared_ptr<ISurface> surface,
            std::shared_ptr<ILogicDeviceSettings> settings,
            std::shared_ptr<IPhysicalDeviceFeatureRequester> featureRequester)
        {
            spdlog::info("--> VulkanLogicalDevice::VulkanLogicalDevice");
            auto const pDevice = manager->getPhysicalDevice(surface.get());
            const auto &imp = dynamic_cast<const VulkanPhysicalDevice *>(pDevice);
            const auto &queueFamilyProperties = imp->queueFamilyProperties();
            auto count = queueFamilyProperties.size();
            // each queueFamily has array of queues
            // each queue has different priorities.
            std::vector<std::vector<float>> queueFamilyPriorities(count);
            std::vector<VkDeviceQueueCreateInfo> queueCreateInfos(count, {.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO});
            for (uint32_t i = 0U; i < count; i++)
            {
                const auto &queueFamilyProperty = queueFamilyProperties[i];
                if (settings->isGraphicsQueueHighPriority())
                {
                    const auto matchedIndex = imp->queueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
                    if (matchedIndex == i)
                    {
                        // reserve does not change the logic size, needs push_back
                        // each queue in the queue family needs a priority value
                        queueFamilyPriorities[i].reserve(queueFamilyProperty.queueCount);
                        // first queue in the queue family will be graphics
                        queueFamilyPriorities[i].push_back(1.0f);
                        for (uint32_t k = 1; k < queueFamilyProperty.queueCount; k++)
                        {
                            queueFamilyPriorities[i].push_back(0.5f);
                        }
                    }
                    else
                    {
                        // this queuefamily does not have graphics cap
                        queueFamilyPriorities[i].resize(queueFamilyProperty.queueCount, 0.5f);
                    }
                }
                else
                {
                    queueFamilyPriorities[i].resize(queueFamilyProperty.queueCount, 0.5f);
                }

                auto &queue_create_info = queueCreateInfos[i];

                queue_create_info.queueFamilyIndex = i;
                queue_create_info.queueCount = queueFamilyProperty.queueCount;
                // vector<float> to raw float*
                queue_create_info.pQueuePriorities = queueFamilyPriorities[i].data();
            }

            auto thirdPartyDeviceExts = featureRequester->requestPhysicalDeviceFeatures(const_cast<IPhysicalDevice *>(pDevice));
            for (const auto &ext : thirdPartyDeviceExts)
            {
                spdlog::info("[3rd] {}", ext);
            }
            // check if all the required extensions are available
            auto const requiredExts = settings->requiredPhysicalDeviceExtensions();
            for (const auto &ext : requiredExts)
            {
                if (!imp->isExtensionSupported(ext))
                {
                    throw runtime_error(format("required extension is not available {}", ext));
                }
                thirdPartyDeviceExts.insert(ext);
            }
            // unordered_set to vector
            // https://www.geeksforgeeks.org/convert-set-to-vector-in-cpp/
            vector<const char *> allExts;
            allExts.reserve(thirdPartyDeviceExts.size());
            for (const auto &ext : thirdPartyDeviceExts)
            {
                allExts.push_back(ext.c_str());
            }

            VkDeviceCreateInfo createInfo = {
                .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                .pNext = imp->requestedFeaturesLinkedList(),
                .queueCreateInfoCount = queueCreateInfos.size(),
                .pQueueCreateInfos = queueCreateInfos.data(),
                .enabledExtensionCount = allExts.size(),
                .ppEnabledExtensionNames = allExts.data(),
                .pEnabledFeatures = &imp->requestedFeatures()           
                };

            VK_CHECK(vkCreateDevice(imp->getVkHandle(), &createInfo, nullptr, &_handle));
            spdlog::info("<-- VulkanLogicalDevice::VulkanLogicalDevice");
        }

        VulkanLogicalDevice::~VulkanLogicalDevice()
        {
            vkDestroyDevice(_handle, nullptr);
        }
    }
}
