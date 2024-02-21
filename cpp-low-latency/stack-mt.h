#pragma once

#include <exception>
#include <stack>
#include <mutex>
#include <memory>
#include <thread>
#include <format>

namespace cpp_low_latency
{
    namespace mt
    {
        using namespace std;
        class stackEmptyError : public exception
        {
            virtual const char *what() const noexcept override
            {
                return "stackMt is empy";
            }
        };

        template <typename T>
        class stackMt
        {
        public:
            stackMt() = default;
            stackMt(const stackMt<T> &src)
            {
                if (this == &src)
                {
                    return;
                }
                lock_guard<mutex> lk{src._m};
                _st = src._st;
            }

            void push(T value)
            {
                // lock_guard is raii for mutex
                lock_guard<mutex> lk{_m};
                // _st.emplace(value); // avoid copy
                // push has rvalue overload to avoid copy
                _st.push(std::move(value));
            }

            // top and pop are two steps in a transaction. (combine to 1 step)
            void pop(T &value)
            {
                lock_guard<mutex> lk{_m};
                if (_st.empty())
                {
                    throw stackEmptyError{};
                }
                value = _st.top();
                _st.pop();
            }

            std::shared_ptr<T> pop()
            {
                lock_guard<mutex> lk{_m};
                if (_st.empty())
                {
                    throw stackEmptyError{};
                }
                shared_ptr<T> res{make_shared<T>(_st.top())};
                _st.pop();
                return res;
            }

            inline bool empty() const
            {
                lock_guard<mutex> lk{_m};
                return _st.empty();
            }
            inline auto size() const
            {
                lock_guard<mutex> lk{_m};
                return _st.size();
            }

            // defined in the body directly
            inline friend void swap(stackMt &st1, stackMt &st2)
            {
                if (&st1 == &st2)
                {
                    return;
                }
                // variadic lock
                scoped_lock guard(st1._m, st2._m);
                st1._st.swap(st2._st);
            }

        private:
            stack<T> _st;
            // lock_guard
            mutable mutex _m;
        };

        template<typename T>
        void TestStackMt()
        {
            // thread-safe stack
            stackMt<T> st1, st2;

            // initialize two threads with lamda
            auto lamda = [](stackMt<T>& st, int min, int max){
                for(int i = min; i <= max; i++){
                    st.push(i);
                }
            };
            std::jthread t1(lamda, ref(st1), 0, 100);
            std::jthread t2(lamda, ref(st2), 101, 150);
            t1.join();
            t2.join();
            
            swap(st1, st2);
            cout << format("st1: {}, st2: {}\n", st1.size(), st2.size());
        }
    }
}