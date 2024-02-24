#pragma once 

#include "db.h"
#include "mux.h"

namespace web
{
    namespace core
    {
        using namespace std;
        class IAppRoot
        {
        public:
            virtual ~IAppRoot(){};
            virtual bool run() = 0;
        };
    }
}
