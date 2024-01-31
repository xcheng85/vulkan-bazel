#pragma once

// for std::allocator
#include <memory>
#include <vector>
#include <iostream>

namespace core
{
    class Brick
    {
    public:
        Brick() = default;
        explicit Brick(std::string id) : _id{id} {}
        virtual ~Brick() = default;
        // for perfect forwarding
        Brick(Brick &&) = default;
        Brick &operator=(Brick &&) = default;
        inline decltype(auto) id() const
        {
            return _id;
        }

    private:
        std::string _id;
        std::array<unsigned char, 1024 * 1024 * 1024> _data;
    };

    template <typename T, typename Allocator = std::allocator<T>>
    class ObjectPool final
    {
    public:
        ObjectPool() = default;
        explicit ObjectPool(const Allocator &allocator);
        ~ObjectPool();

        // copy ctor and assignment operator
        ObjectPool(const ObjectPool &) = delete;
        ObjectPool &operator=(const ObjectPool &) = delete;

        // move src cannot be const
        ObjectPool(ObjectPool &&) = delete;
        ObjectPool &operator=(ObjectPool &&) = delete;

        // Template function inside a template class
        // universal ref to allow rvalue
        template <typename... Args>
        std::shared_ptr<T> acquireObject(Args &&...args)
        {
            if (_freeObjects.empty())
            {
                addChunk();
            }
            // uniform init
            T *object{_freeObjects.back()};

            // placement new operator
            // only difference is (ptr) in the middlew
            // example of perfert forwarding + variadic parameters
            ::new (object) T{std::forward<Args>(args)...};

            // in the case T has const
            // Launder:
            // Obtains a pointer to an object created in storage occupied by an existing object of the same type, even if it has const or reference members.
            T *constructedObject{std::launder(object)};

            // Remove the object from the list of free objects.
            _freeObjects.pop_back();

            // example of shared_ptr uniform init
            // with custom deletor
            // example of in-class lamda with this capture to access _freeObjects
            //
            return std::shared_ptr<T>{constructedObject, [this](T *object)
                                      {
                                          // Destroy object without release memory
                                          std::destroy_at(object);
                                          _freeObjects.push_back(object);
                                      }};
        }

    private:
        void addChunk();

        std::vector<T *> _pool;
        std::vector<T *> _freeObjects;

        // modern way for in-class constant
        static constexpr std::size_t initialChunkSize{5};
        std::size_t _newChunkSize{initialChunkSize}; // aggressive
        Allocator _allocator;
    };

    template <typename T, std::size_t CHANNELS = 3>
    class Vector final
    {
    public:
        Vector()
        {
            std::cout << "Vector" << CHANNELS << std::endl;
            _data.resize(CHANNELS);
        }

        Vector(const Vector &src) = default;
        Vector &operator=(const Vector &src) = default;

        Vector(Vector &&src) noexcept
        {
            _data = std::move(src._data);
        }

        Vector &operator=(Vector &&src) noexcept
        {
            _data = std::move(src._data);
            return *this;
        }

        Vector &operator+(const Vector &src) noexcept
        {
            assert(this->_data.size() == src._data.size());
            // loop vector
            for (auto i = 0u; i < _data.size(); ++i)
            {
                this->_data[i] = src._data[i];
            }
            return *this;
        }
        inline decltype(auto) get(size_t i) const
        {
            // check bound
            return _data.at(i);
        }

    private:
        std::vector<T> _data;
    };

    using Vector3f = Vector<float, 3>;
    using Vector3i = Vector<int, 3>;
    using Vector3d = Vector<double, 3>;

    template <typename T, std::size_t CHANNELS = 3>
    std::ostream &operator<<(std::ostream &ostr, const Vector<T, CHANNELS> &v)
    {
        for (auto i = 0u; i < CHANNELS; ++i)
        {
            ostr << v.get(i) << std::endl;
        }
        return ostr;
    }
}
