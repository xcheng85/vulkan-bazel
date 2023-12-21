#pragma once
#include <string>
#include <boost/di.hpp>
#include "utils.h"

using namespace std;

namespace Engine
{
    namespace Core
    {
        class IPhysicalDeviceManager;
        class ISurface;
        class IPhysicalDeviceFeatureRequester;

        class ILogicDeviceSettings
        {
        public:
            virtual ~ILogicDeviceSettings() noexcept = default;
            virtual bool const &isGraphicsQueueHighPriority() const = 0;
            virtual std::vector<std::string> const &requiredPhysicalDeviceExtensions() const = 0;
        };

        static auto GRAPHICS_QUEUE_HIGH_PRIORITY = [] {};
        static auto REQUIRED_PHYISICAL_DEVICE_EXTENSIONS = [] {};
        class VulkanLogicDeviceSettings : public ILogicDeviceSettings
        {
        public:
            BOOST_DI_INJECT(VulkanLogicDeviceSettings,
                            (named = GRAPHICS_QUEUE_HIGH_PRIORITY) const bool,
                            (named = REQUIRED_PHYISICAL_DEVICE_EXTENSIONS) std::vector<std::string> const &);
            inline bool const &isGraphicsQueueHighPriority() const override
            {
                return _graphicsQueueHighPriority;
            };
            inline std::vector<std::string> const &requiredPhysicalDeviceExtensions() const override
            {
                return _requiredPhysicalDeviceExtensions;
            };

        private:
            bool _graphicsQueueHighPriority;
            std::vector<std::string> _requiredPhysicalDeviceExtensions;
        };

        class ILogicalDevice : public VulkanObject<VkDevice, VK_OBJECT_TYPE_DEVICE>
        {
        public:
            virtual ~ILogicalDevice() noexcept = default;
        };

        static auto REQUIRED_GPU_DEVICE_EXTS = [] {};

        class VulkanLogicalDevice : public ILogicalDevice
        {
        public:
            BOOST_DI_INJECT(VulkanLogicalDevice,
                            std::shared_ptr<IPhysicalDeviceManager>,
                            std::shared_ptr<ISurface>,
                            std::shared_ptr<ILogicDeviceSettings>,
                            std::shared_ptr<IPhysicalDeviceFeatureRequester>
                            );
            virtual ~VulkanLogicalDevice();
        };
    }
};
