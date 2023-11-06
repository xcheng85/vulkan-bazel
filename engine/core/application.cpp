#define VK_NO_PROTOTYPES // for volk
#define VOLK_IMPLEMENTATION // for volk
#include <volk.h>
#include <glslang/Public/ShaderLang.h>
#include <glslang/Public/resource_limits_c.h>
#include <spdlog/spdlog.h>
#include <iostream>
#include "application.h"
// #include "context.h"

using namespace std;
namespace Engine
{
    namespace Core
    {
        Application::Application(
            // std::unique_ptr<IContext> ctx
        )
        // : _context(std::move(ctx))
        {
            spdlog::info("--> Application::Application");

            glslang::InitializeProcess();

            volkInitialize();
            
            spdlog::info("<-- Application::Application");
        }

        Application::~Application()
        {
            glslang::FinalizeProcess();
        }

        void Application::init()
        {
        }

        void Application::update()
        {
        }

        void Application::resize(const uint32_t width, const uint32_t height)
        {
            spdlog::info("--> Application::resize");

            // this->_context->resize(width, height);

            spdlog::info("<-- Application::resize");
        }

        void Application::terminate()
        {
            spdlog::info("--> Application::terminate");
            spdlog::info("<-- Application::terminate");
        }
    }
}
