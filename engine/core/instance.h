#pragma once

#include <string>
#include <boost/di.hpp>
#include "utils.h"

using namespace std;

namespace Engine
{
    namespace Core
    {
        class IInitializer;
        class IInstance : public VulkanObject<VkInstance, VK_OBJECT_TYPE_INSTANCE>
        {
        public:
            virtual ~IInstance() noexcept = default;
        };

        class IInstanceSettings
        {
        public:
            virtual ~IInstanceSettings() noexcept = default;
            virtual std::string name() = 0;
            virtual std::string version() = 0;
            virtual bool headless() = 0;
            virtual std::vector<std::string> requiredValidationLayers() = 0;
            virtual std::vector<std::string> requiredExtensions() = 0;
        };

        static auto APP_NAME = [] {};
        static auto APP_VERSION = [] {};
        static auto APP_HEADLESS = [] {};
        static auto REQUIRED_INSTANCE_VALIDATION_LAYERS = [] {};
        static auto REQUIRED_INSTANCE_EXTENSIONS = [] {};

        class VulkanInstanceSettings : public IInstanceSettings
        {
        public:
            BOOST_DI_INJECT(VulkanInstanceSettings,
                            (named = APP_NAME) const std::string name,
                            (named = APP_VERSION) const std::string version,
                            (named = APP_HEADLESS) const bool headless,
                            (named = REQUIRED_INSTANCE_VALIDATION_LAYERS) std::vector<std::string> const &,
                            (named = REQUIRED_INSTANCE_EXTENSIONS) std::vector<std::string> const &
            );
            inline std::string name() override
            {
                return _name;
            };
            inline std::string version() override
            {
                return _version;
            };
            inline bool headless() override {
                return _headless;
            };
            inline std::vector<std::string> requiredValidationLayers() override{
                return _requiredValidationLayers;
            };
            inline std::vector<std::string> requiredExtensions() override {
                return _requiredExtensions;
            };

        private:
            std::string _name;
            std::string _version;
            bool _headless;
            std::vector<std::string> _requiredValidationLayers;
            std::vector<std::string> _requiredExtensions;
        };

        // vulkan imp
        class VulkanInstance : public IInstance
        {
        public:
            BOOST_DI_INJECT(VulkanInstance,
                            IInitializer &,
                            IInstanceSettings &);
            virtual ~VulkanInstance();

        private:
            // debugger
            VkDebugUtilsMessengerEXT _messenger;
            VkDebugReportCallbackEXT _reportCallback;
        };
    }
};
