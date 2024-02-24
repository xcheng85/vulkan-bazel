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
        class IDb
        {
        public:
            virtual ~IDb(){};
            virtual string runQuery(string query) = 0;
        };

        class DefaultDb : public IDb
        {
        public:
            DefaultDb() = delete;
            explicit DefaultDb(string dbname, string username, string password) : _db{dbname}, _username{username}, _password{password} {


            }
            virtual ~DefaultDb() {

            }
            virtual string runQuery(string query) override{
                this_thread::sleep_for(1s);
                return query;
            }

        private:
            string _db{};
            string _username{};
            string _password{};
        };
    }
}
