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
#include <magic_enum/magic_enum.hpp>
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
            this->_handle = deviceHandle;

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

        uint32_t VulkanPhysicalDevice::queueFamilyIndex(VkQueueFlagBits queueFlag) const
        {
            const auto &queueFamilyProperties = this->queueFamilyProperties();
            // only for compute
            if (queueFlag & VK_QUEUE_COMPUTE_BIT)
            {
                for (uint32_t i = 0; i < queueFamilyProperties.size(); i++)
                {
                    const auto &queueFamilyProperty = queueFamilyProperties[i];
                    bool onlyCompute = (queueFamilyProperty.queueFlags & queueFlag) && !(queueFamilyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT);
                    if (onlyCompute)
                    {
                        return i;
                    }
                }
            }

            // only transfer
            if (queueFlag & VK_QUEUE_TRANSFER_BIT)
            {
                for (uint32_t i = 0; i < queueFamilyProperties.size(); i++)
                {
                    const auto &queueFamilyProperty = queueFamilyProperties[i];
                    bool onlyTransfer = (queueFamilyProperty.queueFlags & queueFlag) && !(queueFamilyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT) && !(queueFamilyProperty.queueFlags & VK_QUEUE_COMPUTE_BIT);
                    if (onlyTransfer)
                    {
                        return i;
                    }
                }
            }

            for (uint32_t i = 0; i < queueFamilyProperties.size(); i++)
            {
                const auto &queueFamilyProperty = queueFamilyProperties[i];
                if (queueFamilyProperty.queueFlags & queueFlag)
                {
                    return i;
                }
            }

            throw runtime_error(format("Could not find a matching queue family index"));
        }

        bool VulkanPhysicalDevice::isExtensionSupported(const std::string &extension) const noexcept
        {
            // lamda, capture by value: extension
            return std::find_if(_extensions.begin(), _extensions.end(),
                                [extension](auto &ext)
                                {
                                    return strcmp(ext.extensionName, extension.c_str()) == 0;
                                }) != _extensions.end();
        }

        // return reference and side-effect is to to use linked-list to management all the third-party requests
        template <typename T>
        T &VulkanPhysicalDevice::requestFeatures(VkStructureType type)
        {
            T extension{type};
            // https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPhysicalDeviceFeatures2.html
            VkPhysicalDeviceFeatures2 physical_device_features{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2};
            physical_device_features.pNext = &extension;
            vkGetPhysicalDeviceFeatures2(_handle, &physical_device_features);

            // copy ctor
            _requestedFeatures.insert({type, std::make_shared<T>(extension)});
            auto *curr = static_cast<T *>(_requestedFeatures.find(type)->second.get());

            if (_requestFeaturesLinkedListHeader)
            {
                curr->pNext = _requestFeaturesLinkedListHeader;
                // logging the memory address
                spdlog::info("{}->{}", _requestedFeatures.find(type)->second.get(), _requestFeaturesLinkedListHeader);
            }
            _requestFeaturesLinkedListHeader = curr;
            return *static_cast<T*>(_requestFeaturesLinkedListHeader);
        }

        // Avoid linking error for template function split
        void VulkanPhysicalDevice::init()
        {
            this->requestFeatures<VkPhysicalDevicePerformanceQueryFeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PERFORMANCE_QUERY_FEATURES_KHR);
            this->requestFeatures<VkPhysicalDeviceHostQueryResetFeatures>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES);
            this->requestFeatures<VkPhysicalDeviceBufferDeviceAddressFeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES_KHR);
            this->requestFeatures<VkPhysicalDeviceColorWriteEnableFeaturesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COLOR_WRITE_ENABLE_FEATURES_EXT);
            this->requestFeatures<VkPhysicalDeviceConditionalRenderingFeaturesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONDITIONAL_RENDERING_FEATURES_EXT);
            this->requestFeatures<VkPhysicalDeviceDescriptorBufferFeaturesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_FEATURES_EXT);
            this->requestFeatures<VkPhysicalDeviceDescriptorIndexingFeaturesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT);
            this->requestFeatures<VkPhysicalDevice16BitStorageFeatures>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES);
            this->requestFeatures<VkPhysicalDeviceFloat16Int8FeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT16_INT8_FEATURES_KHR);
            this->requestFeatures<VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_INPUT_DYNAMIC_STATE_FEATURES_EXT);
            this->requestFeatures<VkPhysicalDeviceTimelineSemaphoreFeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES_KHR);
            this->requestFeatures<VkPhysicalDeviceSynchronization2FeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES_KHR);
            this->requestFeatures<VkPhysicalDeviceShaderObjectFeaturesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_OBJECT_FEATURES_EXT);
            this->requestFeatures<VkPhysicalDeviceDynamicRenderingFeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR);
            this->requestFeatures<VkPhysicalDeviceDynamicRenderingFeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR);
            this->requestFeatures<VkPhysicalDeviceAccelerationStructureFeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR);
            this->requestFeatures<VkPhysicalDeviceExtendedDynamicStateFeaturesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT);
            this->requestFeatures<VkPhysicalDeviceExtendedDynamicState2FeaturesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_2_FEATURES_EXT);
            this->requestFeatures<VkPhysicalDeviceExtendedDynamicState3FeaturesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_3_FEATURES_EXT);
            this->requestFeatures<VkPhysicalDeviceMeshShaderFeaturesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT);
            this->requestFeatures<VkPhysicalDeviceGraphicsPipelineLibraryFeaturesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_GRAPHICS_PIPELINE_LIBRARY_FEATURES_EXT);
            this->requestFeatures<VkPhysicalDeviceFragmentShadingRateFeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_FEATURES_KHR);
            this->requestFeatures<VkPhysicalDeviceFragmentShaderBarycentricFeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_BARYCENTRIC_FEATURES_KHR);
            this->requestFeatures<VkPhysicalDeviceLineRasterizationFeaturesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LINE_RASTERIZATION_FEATURES_EXT);
            this->requestFeatures<VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_FEATURES_EXT);
            this->requestFeatures<VkPhysicalDeviceVulkan13Features>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES);
            this->requestFeatures<VkPhysicalDeviceVulkan12Features>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES);
            this->requestFeatures<VkPhysicalDeviceVulkan11Features>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES);
        }
    }
}
