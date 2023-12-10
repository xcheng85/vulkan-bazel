#pragma once

#include <string>
#include <boost/di.hpp>

using namespace std;

namespace Engine
{
    namespace Core
    {
        class IInitializer
        {
        public:
            virtual ~IInitializer() noexcept = default;
        };

        class VulkanInitializer: public IInitializer
        {
        public:
            BOOST_DI_INJECT(VulkanInitializer);
            virtual ~VulkanInitializer();
        };
    }
};
