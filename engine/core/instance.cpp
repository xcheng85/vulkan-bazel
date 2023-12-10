// #define VK_NO_PROTOTYPES    // for volk
#include <format>
#include <ranges>
#include <cassert>
// ranges::copy
#include <algorithm>
// #define VOLK_IMPLEMENTATION // for volk
#define VK_NO_PROTOTYPES // for volk
#include <volk.h>
#include <spdlog/spdlog.h>
#include "instance.h"
#include "initializer.h"

using namespace std;
namespace rv = std::ranges::views;
namespace rg = std::ranges;
namespace Engine
{
    namespace Core
    {
        static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugUtilsCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT severity,
            VkDebugUtilsMessageTypeFlagsEXT type,
            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
            void *pUserData)
        {
            // Log debug message
            if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
            {
                spdlog::warn("{}-{}: {}", pCallbackData->messageIdNumber, pCallbackData->pMessageIdName, pCallbackData->pMessage);
            }
            else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
            {
                spdlog::error("{}-{}: {}", pCallbackData->messageIdNumber, pCallbackData->pMessageIdName, pCallbackData->pMessage);
            }
            return VK_FALSE;
        }

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

        VulkanInstance::VulkanInstance(
            IInitializer &initializer,
            IInstanceSettings &settings)
        {
            spdlog::info("--> VulkanInstance::VulkanInstance");

            // only extensions provided by the Vulkan implementation or by implicitly enabled layers are returned
            uint32_t instanceExtensionsCount;
            VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionsCount, nullptr));

            vector<VkExtensionProperties> allInstanceExtensions(instanceExtensionsCount);
            VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionsCount, allInstanceExtensions.data()));

            // check extensions by layer
            const auto &validationLayers = settings.requiredValidationLayers();
            for (auto &layer : validationLayers)
            {
                uint32_t layerInstanceExtensionsCount;
                VK_CHECK(vkEnumerateInstanceExtensionProperties(layer.c_str(), &layerInstanceExtensionsCount, nullptr));
                std::vector<VkExtensionProperties> allLayerInstanceExtensions(layerInstanceExtensionsCount);
                VK_CHECK(vkEnumerateInstanceExtensionProperties(layer.c_str(), &layerInstanceExtensionsCount, allLayerInstanceExtensions.data()));
                std::copy(allLayerInstanceExtensions.begin(), allLayerInstanceExtensions.end(), std::back_inserter(allInstanceExtensions));
            }

            for (auto &ext : allInstanceExtensions)
            {
                spdlog::info(ext.extensionName);
            }

            // lamda: check if required instance exts are supported
            auto checkExtensionAvailability = [](std::vector<string> const &exts, vector<VkExtensionProperties> &avails)
            {
                for (auto &ext : exts)
                {
                    bool isExtSupported = false;
                    for (auto &avail : avails)
                    {
                        if (strcmp(avail.extensionName, ext.c_str()) == 0)
                        {
                            isExtSupported = true;
                            break;
                        }
                    }
                    if (!isExtSupported)
                    {
                        spdlog::error("Extension: {} is not supported!", ext);
                        return false;
                    }
                }
                return true;
            };
            const auto &isExtSupported = checkExtensionAvailability(settings.requiredExtensions(), allInstanceExtensions);
            if (!isExtSupported)
            {
                throw std::runtime_error{format("Found unsupported extensions")};
            }

            const auto &appName = settings.name();

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

            // VK_EXT_debug_utils
            // instance extensions
            VkDebugUtilsMessengerCreateInfoEXT debugUtilsCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
                .messageSeverity =
                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                .messageType =
                    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                .pfnUserCallback = &VulkanDebugUtilsCallback,
            };
            VkInstanceCreateInfo createInfo =
                {
                    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                    .pNext = &debugUtilsCreateInfo,
                    .flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR, // mac os, see readme
                    .pApplicationInfo = &appinfo,
                    .enabledLayerCount = static_cast<uint32_t>(layer_names.size()),
                    .ppEnabledLayerNames = layer_names.data(),
                    .enabledExtensionCount = static_cast<uint32_t>(extensitions_names.size()),
                    .ppEnabledExtensionNames = extensitions_names.data(),
                };

            // // deprecated extensions
            // VkDebugReportCallbackCreateInfoEXT debugReportCreateInfo = {
            //     .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT};

            // Best practice validation
            // https://vulkan.lunarg.com/doc/view/1.3.204.1/windows/khronos_validation_layer.html
            std::vector<VkValidationFeatureEnableEXT> enable_features{
                VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT,
                VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT,
                VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT,
                VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT,
            };

            VkValidationFeaturesEXT validation_features_info = {
                .sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT,
                .pNext = createInfo.pNext, // insert node in a single linked list
                .enabledValidationFeatureCount = static_cast<uint32_t>(enable_features.size()),
                .pEnabledValidationFeatures = enable_features.data(),
            };
            createInfo.pNext = &validation_features_info;

            VK_CHECK(vkCreateInstance(&createInfo, nullptr, &_handle));
            // https://github.com/zeux/volk/blob/master/README.md
            volkLoadInstance(_handle);

            VK_CHECK(vkCreateDebugUtilsMessengerEXT(_handle, &debugUtilsCreateInfo, nullptr, &_messenger));
            spdlog::info("<-- VulkanInstance::VulkanInstance");
        };

        VulkanInstance::~VulkanInstance()
        {
            vkDestroyDebugUtilsMessengerEXT(_handle, _messenger, nullptr);
            vkDestroyInstance(_handle, nullptr);
        }
    }
}
