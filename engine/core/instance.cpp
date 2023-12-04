#define VK_NO_PROTOTYPES    // for volk
#define VOLK_IMPLEMENTATION // for volk
#include <ranges>
#include <cassert>
// ranges::copy
#include <algorithm>
#include <volk.h>
#include <spdlog/spdlog.h>
#include "instance.h"

using namespace std;
namespace rv = std::ranges::views;
namespace rg = std::ranges;
namespace Engine
{
    namespace Core
    {
        VulkanInstanceSettings::VulkanInstanceSettings(
            const std::string name,
            const std::string version,
            const bool headless,
            std::vector<std::string> const &requiredValidationLayers,
            std::vector<std::string> const &requiredExtensions)
            : _name(name),
              _version(version),
              _headless(headless),
              _requiredValidationLayers(requiredValidationLayers),
              _requiredExtensions(requiredExtensions)
        {
        }

        VulkanInstance::VulkanInstance(IInstanceSettings &settings)
        {
            spdlog::info("--> VulkanInstance::VulkanInstance");

            uint32_t instanceExtensionsCount;
            VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionsCount, nullptr));

            vector<VkExtensionProperties> allInstanceExtensions(instanceExtensionsCount);
            VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionsCount, allInstanceExtensions.data()));

            const auto &appName = settings.name();
            const auto &validationLayers = settings.requiredValidationLayers();
            const auto &instanceExtensions = settings.requiredExtensions();
            const VkApplicationInfo appinfo =
                {
                    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                    .pNext = nullptr,
                    .pApplicationName = appName.c_str(),
                    .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                    .pEngineName = "No Engine",
                    .engineVersion = VK_MAKE_VERSION(1, 0, 0),
                    .apiVersion = VK_API_VERSION_1_3};

            std::vector<const char *> layer_names(validationLayers.size());
            std::vector<const char *> extensitions_names(instanceExtensions.size());
            // lamda
            auto convert = [](std::vector<string> const &in, std::vector<const char *> &out)
            {
                out.resize(in.size());
                auto v = in | rv::transform([](const string &s)
                                            { return s.c_str(); });
                rg::copy(v, out.begin());
            };

            convert(validationLayers, layer_names);
            convert(instanceExtensions, extensitions_names);

            const VkInstanceCreateInfo createInfo =
                {
                    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                    .pNext = nullptr,
                    .flags = 0,
                    .pApplicationInfo = &appinfo,
                    .enabledLayerCount = static_cast<uint32_t>(layer_names.size()),
                    .ppEnabledLayerNames = layer_names.data(),
                    .enabledExtensionCount = static_cast<uint32_t>(extensitions_names.size()),
                    .ppEnabledExtensionNames = extensitions_names.data()};

#ifdef USE_VALIDATION_LAYERS
            VkDebugUtilsMessengerCreateInfoEXT debug_utils_create_info = {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
            VkDebugReportCallbackCreateInfoEXT debug_report_create_info = {VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT};
            if (has_debug_utils)
            {
                debug_utils_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
                debug_utils_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
                debug_utils_create_info.pfnUserCallback = debug_utils_messenger_callback;

                instance_info.pNext = &debug_utils_create_info;
            }
            else
            {
                debug_report_create_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
                debug_report_create_info.pfnCallback = debug_callback;

                instance_info.pNext = &debug_report_create_info;
            }
#endif

#if (defined(VKB_ENABLE_PORTABILITY))
            instance_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

            // Some of the specialized layers need to be enabled explicitly
#ifdef USE_VALIDATION_LAYER_FEATURES
            VkValidationFeaturesEXT validation_features_info = {VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT};
            std::vector<VkValidationFeatureEnableEXT> enable_features{};
            if (validation_features)
            {
#if defined(VKB_VALIDATION_LAYERS_GPU_ASSISTED)
                enable_features.push_back(VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT);
                enable_features.push_back(VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT);
#endif
#if defined(VKB_VALIDATION_LAYERS_BEST_PRACTICES)
                enable_features.push_back(VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT);
#endif
#if defined(VKB_VALIDATION_LAYERS_SYNCHRONIZATION)
                enable_features.push_back(VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT);
#endif
                validation_features_info.enabledValidationFeatureCount = static_cast<uint32_t>(enable_features.size());
                validation_features_info.pEnabledValidationFeatures = enable_features.data();
                validation_features_info.pNext = instance_info.pNext;
                instance_info.pNext = &validation_features_info;
            }
#endif

            VK_CHECK(vkCreateInstance(&createInfo, nullptr, &_handle));
            // This function will load all required Vulkan entrypoints, including all extensions; you can use Vulkan from here on as usual.
            volkLoadInstance(_handle);
            // add vulkan debugger layer
            {
                const VkDebugUtilsMessengerCreateInfoEXT ci = {
                    .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
                    .messageSeverity =
                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                    .messageType =
                        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                    .pfnUserCallback = &VulkanDebugCallback,
                    .pUserData = nullptr};
                VK_CHECK(vkCreateDebugUtilsMessengerEXT(_handle, &ci, nullptr, &_messenger));
            }
            // enable the following will crash
            {

                const VkDebugReportCallbackCreateInfoEXT ci = {
                    .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
                    .pNext = nullptr,
                    .flags =
                        VK_DEBUG_REPORT_WARNING_BIT_EXT |
                        VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
                        VK_DEBUG_REPORT_ERROR_BIT_EXT |
                        VK_DEBUG_REPORT_DEBUG_BIT_EXT,
                    .pfnCallback = &VulkanDebugReportCallback,
                    .pUserData = nullptr};
                VK_CHECK(vkCreateDebugReportCallbackEXT(_handle, &ci, nullptr, &_reportCallback));
            }

            spdlog::info("<-- VulkanInstance::VulkanInstance");
        }

        VulkanInstance::~VulkanInstance()
        {
            vkDestroyDebugReportCallbackEXT(_handle, _reportCallback, nullptr);
        }

    }
}
