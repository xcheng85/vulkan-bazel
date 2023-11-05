#pragma once
#define VK_NO_PROTOTYPES // for volk
#include <stdint.h>
#include <optional>
#include <string>
#include <vector>
#include <iostream>
#include <memory>

using namespace std;

namespace Engine
{
    namespace Core
    {
        // class IContext;
        class IApplication
        {
        public:
            virtual ~IApplication() noexcept = default;

            virtual void init() = 0;

            virtual void update() = 0;

            virtual void resize(const uint32_t width, const uint32_t height) = 0;

            virtual void terminate() = 0;
        };

        class Application : public IApplication
        {
        public:
            // explicit Application() = delete;
            explicit Application(
                // std::unique_ptr<IContext>
            );
            virtual ~Application();

            void init() override;
            void update() override;
            void resize(const uint32_t width, const uint32_t height) override;
            void terminate() override;

        protected:
            // std::unique_ptr<IContext> _context{nullptr};
        };
    }
};
