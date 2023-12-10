#pragma once
#define VK_NO_PROTOTYPES // for volk
// #define VOLK_IMPLEMENTATION // for volk
#include <volk.h>
#include <spdlog/spdlog.h>
using namespace std;

namespace Engine
{
    namespace Core
    {
#define VK_CHECK(value) CHECK(value == VK_SUCCESS, __FILE__, __LINE__);

        // forward declaration to avoid multiple definition.
        inline void CHECK(bool check, const char *fileName, int lineNumber)
        {
            if (!check)
            {
                spdlog::info("CHECK() failed at {0} {1}", fileName, lineNumber);
                assert(false);
                exit(EXIT_FAILURE);
            }
        }

        template <typename VkHandle, VkObjectType OBJECT_TYPE>
        class VulkanObject
        {
        public:
            VulkanObject(VkHandle handle = VK_NULL_HANDLE) : _handle{handle}
            {
            }

            VulkanObject(const VulkanObject &) = delete;
            VulkanObject &operator=(const VulkanObject &) = delete;

            VulkanObject(VulkanObject &&other) : _handle{other._handle}
            {
                other._handle = VK_NULL_HANDLE;
            }

            VulkanObject &operator=(VulkanObject &&other)
            {
                _handle = other._handle;
                other._handle = VK_NULL_HANDLE;
                return *this;
            }

            virtual ~VulkanObject() = default;

            inline VkObjectType getObjectType() const
            {
                return OBJECT_TYPE;
            }

            inline const VkHandle &getVkHandle() const
            {
                return _handle;
            }

        protected:
            VkHandle _handle;
        };
    }
};
