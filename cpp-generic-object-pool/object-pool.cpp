#include <numeric> // for iota
#include <spdlog/spdlog.h>
#include "object-pool.h"

using namespace std;

namespace core
{
    template <typename T, typename Allocator>
    ObjectPool<T, Allocator>::ObjectPool(const Allocator &allocator)
        : _allocator{allocator}
    {
    }

    template <typename T, typename Allocator>
    ObjectPool<T, Allocator>::~ObjectPool()
    {
        assert(_freeObjects.size() == initialChunkSize * (std::pow(2, _pool.size()) - 1));

        // Deallocate all allocated memory.
        std::size_t chunkSize{initialChunkSize};
        for (auto *chunk : _pool)
        {
            _allocator.deallocate(chunk, chunkSize);
            chunkSize *= 2;
        }
        _pool.clear();
    }

    // template <typename T, typename Allocator>
    // template <typename... Args>
    // std::shared_ptr<T> ObjectPool<T, Allocator>::acquireObject(Args &&...args)
    // {
    //     if (_freeObjects.empty())
    //     {
    //         addChunk();
    //     }
    //     // uniform init
    //     T *object{_freeObjects.back()};

    //     // placement new operator
    //     // only difference is (ptr) in the middlew
    //     // example of perfert forwarding + variadic parameters
    //     ::new (object) T{std::forward<Args>(args)...};

    //     // in the case T has const
    //     // Launder:
    //     // Obtains a pointer to an object created in storage occupied by an existing object of the same type, even if it has const or reference members.
    //     T *constructedObject{std::launder(object)};

    //     // Remove the object from the list of free objects.
    //     _freeObjects.pop_back();

    //     // example of shared_ptr uniform init
    //     // with custom deletor
    //     // example of in-class lamda with this capture to access _freeObjects
    //     //
    //     return std::shared_ptr<T>{constructedObject, [this](T *object)
    //                               {
    //                                   // Destroy object without release memory
    //                                   std::destroy_at(object);
    //                                   _freeObjects.push_back(object);
    //                               }};
    // }

    template <typename T, typename Allocator>
    void ObjectPool<T, Allocator>::addChunk()
    {
        spdlog::info("addChunk");
        // use custom allocator to specifically allocate memory,
        // avoid fragmentation
        _pool.push_back(nullptr);
        try
        {
            _pool.back() = _allocator.allocate(_newChunkSize);
        }
        catch (...)
        {
            _pool.pop_back();
            // rethrow pattern
            throw;
        }
        // update reference to free objects in the object pool
        auto freeObjectsSize{_freeObjects.size()};
        _freeObjects.resize(freeObjectsSize + _newChunkSize);

        // iota: generation sequential number with initial value + 1,
        // store the offset
        std::iota(begin(_freeObjects) + freeObjectsSize, end(_freeObjects), _pool.back());

        // make it aggressive next time
        _newChunkSize *= 2;
    }
}

template class core::ObjectPool<core::Brick>;
