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
        class IWindow;
        class ISurface : public VulkanObject<VkSurfaceKHR, VK_OBJECT_TYPE_SURFACE_KHR>
        {
        public:
            virtual ~ISurface() noexcept = default;
        };

        class VulkanSurface : public ISurface
        {
        public:
            explicit VulkanSurface(std::shared_ptr<IWindow>, std::shared_ptr<IInstance>);
            virtual ~VulkanSurface();

        private:
            std::shared_ptr<IInstance> _instance;
        };
    }
};
