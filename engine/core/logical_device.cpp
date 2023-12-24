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

#define VMA_IMPLEMENTATION // needed for undefined reference
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 0
#define VMA_VULKAN_VERSION 1003000 // Vulkan 1.3

#include <vk_mem_alloc.h>

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
                .pEnabledFeatures = &imp->requestedFeatures()};

            VK_CHECK(vkCreateDevice(imp->getVkHandle(), &createInfo, nullptr, &_handle));

            _logicDeviceQueues.resize(count);
            for (uint32_t familyIndex = 0; familyIndex < count; familyIndex++)
            {
                const auto &queueFamilyProperty = queueFamilyProperties[familyIndex];

                auto isPresentable = imp->isSurfacePresentable(surface->getVkHandle(), familyIndex);

                for (uint32_t queueIndex = 0; queueIndex < queueFamilyProperty.queueCount; queueIndex++)
                {
                    // emplace_back vs push_back: save an extra copy or move operatation
                    // https://en.cppreference.com/w/cpp/container/vector/emplace_back
                    _logicDeviceQueues[familyIndex].emplace_back(*this, familyIndex, queueFamilyProperty, isPresentable, queueIndex);
                }
            }

            VmaVulkanFunctions vkFuncPointers{
                .vkGetPhysicalDeviceProperties = vkGetPhysicalDeviceProperties,
                .vkGetPhysicalDeviceMemoryProperties = vkGetPhysicalDeviceMemoryProperties,
                .vkAllocateMemory = vkAllocateMemory,
                .vkFreeMemory = vkFreeMemory,
                .vkMapMemory = vkMapMemory,
                .vkUnmapMemory = vkUnmapMemory,
                .vkFlushMappedMemoryRanges = vkFlushMappedMemoryRanges,
                .vkInvalidateMappedMemoryRanges = vkInvalidateMappedMemoryRanges,
                .vkBindBufferMemory = vkBindBufferMemory,
                .vkBindImageMemory = vkBindImageMemory,
                .vkGetBufferMemoryRequirements = vkGetBufferMemoryRequirements,
                .vkGetImageMemoryRequirements = vkGetImageMemoryRequirements,
                .vkCreateBuffer = vkCreateBuffer,
                .vkDestroyBuffer = vkDestroyBuffer,
                .vkCreateImage = vkCreateImage,
                .vkDestroyImage = vkDestroyImage,
                .vkCmdCopyBuffer = vkCmdCopyBuffer,
            };

            VmaAllocatorCreateInfo ci{
                .physicalDevice = imp->getVkHandle(),
                .device = _handle,
                .pVulkanFunctions = &vkFuncPointers,
                .instance = imp->getInstance()->getVkHandle(),
            };

            // device extensions as pre-req: VK_KHR_get_memory_requirements2, VK_KHR_dedicated_allocation
            if (imp->isExtensionSupported(VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME) && imp->isExtensionSupported(VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME))
            {
                ci.flags |= VMA_ALLOCATOR_CREATE_KHR_DEDICATED_ALLOCATION_BIT;
                vkFuncPointers.vkGetBufferMemoryRequirements2KHR = vkGetBufferMemoryRequirements2KHR;
                vkFuncPointers.vkGetImageMemoryRequirements2KHR = vkGetImageMemoryRequirements2KHR;
            }
            // device extensions as pre-req + vk > 1.0
            if (imp->isExtensionSupported(VK_EXT_MEMORY_BUDGET_EXTENSION_NAME))
            {
                ci.flags |= VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
                vkFuncPointers.vkGetPhysicalDeviceMemoryProperties2KHR = vkGetPhysicalDeviceMemoryProperties2KHR;
            }
            if (imp->isExtensionSupported(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME))
            {
                ci.flags |= VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
            }
            VK_CHECK(vmaCreateAllocator(&ci, &_vmaAllocator));
            spdlog::info("<-- VulkanLogicalDevice::VulkanLogicalDevice");
        }

        VulkanLogicalDevice::~VulkanLogicalDevice()
        {
            VmaTotalStatistics stats;
            vmaCalculateStatistics(_vmaAllocator, &stats);
            spdlog::info("Total device memory allocated: {} bytes.", stats.total.statistics.blockBytes);
            vmaDestroyAllocator(_vmaAllocator);
            vkDestroyDevice(_handle, nullptr);
        }

        VulkanLogicalDeviceQueue::VulkanLogicalDeviceQueue(
            VulkanLogicalDevice &logicDevice,
            uint32_t familyIndex,
            VkQueueFamilyProperties properties,
            bool isPresentable,
            uint32_t queueIndex) : _familyIndex(familyIndex),
                                   _properties(properties),
                                   _isPrensentable(isPresentable),
                                   _queueIndex(queueIndex)
        {
            vkGetDeviceQueue(logicDevice.getVkHandle(), familyIndex, queueIndex, &_handle);
        }

        VulkanLogicalDeviceQueue::VulkanLogicalDeviceQueue(VulkanLogicalDeviceQueue &&other) noexcept
        {
        }

        VulkanLogicalDeviceQueue::~VulkanLogicalDeviceQueue()
        {
        }
    }
}
