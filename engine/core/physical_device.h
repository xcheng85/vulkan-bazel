#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
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

        // This imp should provided in the host app not in the library.
        // It is customized per app
        // Should have VulkanPhysicalDevice as dependencies
        class IPhysicalDeviceFeatureRequester
        {
        public:
            virtual ~IPhysicalDeviceFeatureRequester() noexcept = default;
            // return required extensions for these features
            virtual std::unordered_set<string> requestPhysicalDeviceFeatures(IPhysicalDevice *) noexcept = 0;
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
            inline const auto &features() const
            {
                return _features;
            };
            // for developing purpose, enable all the features by default
            inline const auto &requestedFeatures() const
            {
                return _features;
            };
            inline const auto &properties() const
            {
                return _properties;
            };
            inline const auto &queueFamilyProperties() const
            {
                return _queueFamilyProperties;
            };
            // get the first matched queueFamily
            uint32_t queueFamilyIndex(VkQueueFlagBits queueFlag) const;
            bool isExtensionSupported(const std::string &extension) const noexcept;

            // third-party features
            // T and VkStructureType must match
            // return writable address to the host app to change
            template <typename T>
            T &requestFeatures(VkStructureType type);

            inline void *requestedFeaturesLinkedList() const
            {
                return _requestFeaturesLinkedListHeader;
            }

        private:
            void init();
            std::shared_ptr<IInstance> _instance;
            VkPhysicalDeviceFeatures _features; // all the features supported
            VkPhysicalDeviceProperties _properties;
            VkPhysicalDeviceMemoryProperties _memoryProperties;
            std::vector<VkQueueFamilyProperties> _queueFamilyProperties;
            // device extensions
            std::vector<VkExtensionProperties> _extensions;
            // linked list to chain device features, which will be passed to logic device create info
            void *_requestFeaturesLinkedListHeader{nullptr};
            std::unordered_map<VkStructureType, std::shared_ptr<void>> _requestedFeatures;
        };
    }
};

// // due to template impl
// #include "physical_device.cpp"