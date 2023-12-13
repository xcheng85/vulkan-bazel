#pragma once
#include <string>
#include <boost/di.hpp>
#include "utils.h"

using namespace std;

namespace Engine
{
    namespace Core
    {
        class IInstance;
        class ISurface;
        class IPhysicalDevice : public VulkanObject<VkPhysicalDevice, VK_OBJECT_TYPE_PHYSICAL_DEVICE>
        {
        public:
            virtual ~IPhysicalDevice() noexcept = default;
            virtual IInstance *getInstance() const = 0;
        };

        class VulkanPhysicalDevice : public IPhysicalDevice
        {
        public:
            explicit VulkanPhysicalDevice(std::shared_ptr<IInstance>, VkPhysicalDevice);
            virtual ~VulkanPhysicalDevice();
            // used when create logical Device
            inline IInstance *getInstance() const override
            {
                return _instance.get();
            };

            bool isSurfacePresentable(VkSurfaceKHR const &surface, uint32_t queueFamilyIndex) const;

        private:
            std::shared_ptr<IInstance> _instance;
            VkPhysicalDeviceFeatures _features;
            VkPhysicalDeviceProperties _properties;
            VkPhysicalDeviceMemoryProperties _memoryProperties;
            std::vector<VkQueueFamilyProperties> _queueFamilyProperties;
            // device extensions
            std::vector<VkExtensionProperties> _extensions;
        };
    }
};
