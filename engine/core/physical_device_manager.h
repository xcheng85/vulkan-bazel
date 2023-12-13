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
        class IPhysicalDeviceManager
        {
        public:
            virtual ~IPhysicalDeviceManager() noexcept = default;
        };

        class VulkanPhysicalDeviceManager : public IPhysicalDeviceManager
        {
        public:
            explicit VulkanPhysicalDeviceManager(std::shared_ptr<IInstance>);
            virtual ~VulkanPhysicalDeviceManager();

        private:
            std::vector<std::unique_ptr<IPhysicalDevice>> _pDevices;
        };
    }
};
