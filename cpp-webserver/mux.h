#pragma once

#include <string>
#include <iostream>
#include <format>
#include <thread>

namespace web
{
    namespace core
    {
        using namespace std;
        // interface
        class IMux
        {
        public:
            virtual ~IMux(){};
            virtual string handleRequest(string req) = 0;
        };

        class DefaultMux : public IMux
        {
        public:
            DefaultMux() = default;
            virtual ~DefaultMux() {

            }
           virtual string handleRequest(string req) override{
                this_thread::sleep_for(1s);
                return req;
            }

        private:
            uint32_t _timeoutInSeconds{5};
        };
    }
}
