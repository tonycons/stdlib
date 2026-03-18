#pragma once

#ifdef __inline_core_header__

namespace cm {

template<typename T>
struct IAllocator
{
    constexpr IAllocator() = default;
    constexpr IAllocator(IAllocator const&) = default;
    constexpr IAllocator& operator=(IAllocator const&) = default;
    constexpr IAllocator(IAllocator&&) = default;
    constexpr IAllocator& operator=(IAllocator&&) = default;

    virtual ~IAllocator() = default;
    virtual constexpr Optional<T*> allocate(usize count) = 0;
    virtual constexpr bool deallocate(T const* basePtr) = 0;

    constexpr Optional<ArrayRef<T>> allocateArray(usize count)
    {
        Optional<T*> ptr = allocate(count);
        if (ptr != None) {
            return ArrayRef<T>(ptr.val(), count);
        }
        return None;
    }

    constexpr auto deallocateArray(ArrayRef<T> const& array) { return deallocate(array.data()); }
};


template<typename T>
struct HeapAllocator final : IAllocator<T>
{
    Optional<T*> allocate(usize count) override { return new T[count]{}; }

    bool deallocate(T const* basePtr) override
    {
        delete[] basePtr;
        return true;
    }
};

template<typename T>
inline HeapAllocator<T> mainAllocator{};

#define with_default_allocator(T) IAllocator<T>& allocator = mainAllocator<T>


}  // namespace cm

template<typename T>
constexpr void* operator new(usize sizeBytes, cm::IAllocator<T>& allocator)
{
    return allocator.allocate(sizeBytes / sizeof(T)).val();
}

template<typename T>
constexpr void* operator new(usize sizeBytes, cm::IAllocator<T>* allocator)
{
    return allocator->allocate(sizeBytes / sizeof(T)).val();
}

template<typename T>
constexpr void* operator new[](usize sizeBytes, cm::IAllocator<T>& allocator)
{
    return allocator.allocate(sizeBytes / sizeof(T)).val();
}

template<typename T>
constexpr void* operator new[](usize sizeBytes, cm::IAllocator<T>* allocator)
{
    return allocator->allocate(sizeBytes / sizeof(T)).val();
}

template<typename T>
constexpr void operator delete(void* ptr, cm::IAllocator<T>& allocator)
{
    allocator.deallocate(static_cast<T*>(ptr));
}
template<typename T>
constexpr void operator delete(void* ptr, cm::IAllocator<T>* allocator)
{
    allocator->deallocate(static_cast<T*>(ptr));
}
template<typename T>
constexpr void operator delete[](void* ptr, cm::IAllocator<T>& allocator)
{
    allocator.deallocate(static_cast<T*>(ptr));
}
template<typename T>
constexpr void operator delete[](void* ptr, cm::IAllocator<T>* allocator)
{
    allocator->deallocate(static_cast<T*>(ptr));
}

#endif
