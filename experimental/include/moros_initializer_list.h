// -*- C++ -*-
#ifndef _CXX_MOROS_INITIALIZER_LIST_H
#define _CXX_MOROS_INITIALIZER_LIST_H

namespace std
{
    template<class T>
    class initializer_list
    {
    public:
        typedef T value_type;
        typedef const T& reference;
        typedef const T& const_reference;
        typedef size_t size_type;
        typedef const T* iterator;
        typedef const T* const_iterator;

    private:
        iterator _array;
        size_type _size;

        constexpr initializer_list(const_iterator _a, size_type _s)
            : _array(_a), _size(_s)
        {
        }

    public:
        constexpr initializer_list() noexcept
            : _array(0), _size(0)
        {
        }

    // Number of elements.
    constexpr size_type size() const noexcept
    {
        return _size;
    }

    // First element.
    constexpr const_iterator begin() const noexcept
    {
        return _array;
    }

    // One past the last element.
    constexpr const_iterator end() const noexcept
    {
        return begin() + size();
    }
};

template<class T>
constexpr const T* begin(initializer_list<T> ilist) noexcept
{
    return ilist.begin();
}

template<class T>
constexpr const T* end(initializer_list<T> ilist) noexcept
{
    return ilist.end();
}

} // std

#endif // _CXX_MOROS_INITIALIZER_LIST_H