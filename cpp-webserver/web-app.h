#pragma once
#include <memory>
#include <exception>
#include "app-root.h"
#include "db.h"
#include "mux.h"

namespace web
{
    namespace application
    {
        using namespace std;
        using namespace core;
        class WebApp : public IAppRoot
        {
        public:
            WebApp(IDb *db, IMux *mux)
            {
                _db = unique_ptr<IDb>(db);
                _mux = unique_ptr<IMux>(mux);
            }
            WebApp() = delete;
            virtual ~WebApp(){};
            virtual bool run() override
            {
                while (true)
                {
                    try
                    {
                        _mux->handleRequest("dummy request");
                       // this_thread::sleep_for(5s);
                        break;
                    }
                    catch (exception e)
                    {
                        auto eptr = std::current_exception();
                        rethrow_exception(eptr);
                    }
                }
                return true;
            }

        private:
            unique_ptr<IDb> _db;
            unique_ptr<IMux> _mux;
        };
    }
}
