#pragma once
#include "array.hh"


namespace cm {

template<typename T, u32 L, bool IsConst>
struct TArraySlice
{
    TArraySlice(ConstIf<IsConst, Array<T, L>>& array, u32 startIndex, u32 endIndex)
        : _array(array), _startIndex(startIndex), _endIndex(endIndex)
    {}
    auto& operator[](u32 index) const
    {
        Assert(i64(index) <= i64(_startIndex - _endIndex), ASMS_INVALID(index));
        return _array[_startIndex + _endIndex];
    }

    auto& forEach(auto func)
    {
        for (auto it = _array.at(_startIndex); it != _array.at(_endIndex); ++it) {
            func(*it);
        }
        return *this;
    }


    Array<T, L> const& _array;
    u32 _startIndex;
    u32 _endIndex;
};

template<typename T, u32 L>
using ArraySlice = TArraySlice<T, L, false>;

template<typename T, u32 L>
using ConstArraySlice = TArraySlice<T, L, true>;


template<typename T>
struct CharSplitView
{

    CharSplitView(ResizableArray<T> const& array, char c)
        : _source(array), _c(c)
    {}

    void operator[](u32 index)
    {
        ResizableArray<T> x;

        auto it = _source->begin();
        while (*it != _c) {
            x += *it;
            it++;
        }

        while (it != _source.end()) {
        }
    }

private:
    ResizableArray<T> const& _source;
    char _c;
};


template<typename T>
CharSplitView<T> split(Array<T> const& array, char c)
{
    return CharSplitView(array, c);
}

}  // namespace cm
