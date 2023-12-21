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
        class IPhysicalDevice;
        class ISurface;
        class IPhysicalDeviceManager
        {
        public:
            virtual ~IPhysicalDeviceManager() noexcept = default;
            virtual const IPhysicalDevice *getPhysicalDevice(ISurface const*) const = 0;
        };

        class VulkanPhysicalDeviceManager : public IPhysicalDeviceManager
        {
        public:
            explicit VulkanPhysicalDeviceManager(std::shared_ptr<IInstance>);
            virtual ~VulkanPhysicalDeviceManager();
            const IPhysicalDevice *getPhysicalDevice(ISurface const*) const override;
        private:
            std::vector<std::unique_ptr<IPhysicalDevice>> _pDevices;
        };
    }
};
