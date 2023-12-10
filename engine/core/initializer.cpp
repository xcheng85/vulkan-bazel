// #define VK_NO_PROTOTYPES    // for volk
#include <format>
#include <ranges>
#include <cassert>
// ranges::copy
#include <algorithm>
// #define VOLK_IMPLEMENTATION // for volk
#define VK_NO_PROTOTYPES // for volk
#include <volk.h>
#include <glslang/Public/ShaderLang.h>
#include <glslang/Public/resource_limits_c.h>
#include <spdlog/spdlog.h>
#include "initializer.h"

using namespace std;
namespace rv = std::ranges::views;
namespace rg = std::ranges;
namespace Engine
{
    namespace Core
    {
        VulkanInitializer::VulkanInitializer()
        {
            spdlog::info("--> VulkanInitializer::VulkanInitializer");
            glslang::InitializeProcess();
            volkInitialize();
            spdlog::info("<-- VulkanInitializer::VulkanInitializer");
        };

        VulkanInitializer::~VulkanInitializer()
        {
            glslang::FinalizeProcess();
        }
    }
}
