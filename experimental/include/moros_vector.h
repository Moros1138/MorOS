// -*- C++ -*-
#ifndef _CXX_MOROS_VECTOR_H
#define _CXX_MOROS_VECTOR_H

#if defined(__GLIBCXX__) || defined(__GLIBCPP__)
#   include <stdio.h>
#   include <assert.h>
#else
#   include "assert.h"
#   include "stdlib.h"
#endif

#include "moros_allocator.h"

namespace MorOS
{
    template<class T, class Allocator = MorOS::allocator<T>>
    class vector
    {
    //--------------------------------------------------------------
    // TYPEDEFS
    //--------------------------------------------------------------
    public:
        typedef T value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef pointer iterator;
        typedef const_pointer const_iterator;
        typedef Allocator allocator_type;
        typedef size_t size_type;

    //--------------------------------------------------------------
    // MEMBER FUNCTIONS
    //--------------------------------------------------------------
    public:
        vector() : _capacity(0), _elements(0), _size(0)
        {
            assert(_capacity == 0 && _elements == 0 && _size == 0);
        }

        vector(const vector& rhs)
        {
            __init();
            assign(rhs.begin(), rhs.end());
        }
        
        ~vector()
        {
            allocator_type allocator;
            
            if(_elements)
                allocator.deallocate(_elements, _capacity);
            
            _capacity = 0;
            _size = 0;
        }

        void assign(size_type count, const_reference value)
        {
            clear();
            for(size_type i = 0; i < count; i++)
                push_back(value);
        }

        template <class InputIterator>
        void assign(InputIterator first, InputIterator last)
        {
            clear();
            for(; first != last; ++first)
            {
                push_back(*first);
            }
        }

    //--------------------------------------------------------------
    // ELEMENT ACCESS
    //--------------------------------------------------------------
    public:
        // access specified element wiht bounds checking
        reference at(size_type pos)
        {
            // TODO: bounds checking
            return _elements[pos];
        }

        // access specified element wiht bounds checking
        const_reference at(size_type pos) const
        {
            // TODO: bounds checking
            return _elements[pos];
        }

        // access specified element
        reference operator[](size_type pos)
        {
            return _elements[pos];
        }

        // access specified element
        const_reference operator[](size_type pos) const
        {
            return _elements[pos];
        }

        // access the first element
        const_reference front() const
        {
            return *begin();
        }

        // access the last element
        const_reference back() const
        {
            return *(end() - 1);
        }

        // direct access to the underlying array
        pointer data()
        {
            return _elements;
        }

    //--------------------------------------------------------------
    // ITERATORS
    //--------------------------------------------------------------
    public:
        // returns an iterator to the beginning
        iterator begin(void) const
        {
            return _elements + 0;
        }
        
        // returns an iterator to the end
        iterator end(void) const
        {
            return _elements + _size;
        }

    //--------------------------------------------------------------
    // CAPACITY
    //--------------------------------------------------------------
    public:        
        // checks whether the container is empty
        bool empty() const
        {
            return _size == 0;
        }
        
        // returns the number of elements
        size_type size() const
        {
            return _size;
        }

        // returns the maximum possible number of elements
        size_type max_size() const
        {
            return (size_type)0xffffffff / sizeof(value_type);
        }

        // reserves storage
        void reserve(size_type new_cap)
        {
            if(new_cap < capacity())
                return;

            allocator_type allocator;
            pointer new_elements = allocator.allocate(new_cap);
            
            // copy elements
            for(size_type i = 0; i < size(); i++)
                new_elements[i] = _elements[i];
            
            // free previous allocation
            allocator.deallocate(_elements, _capacity);

            _elements = new_elements;
            _capacity = new_cap;
        }
        
        // returns the number of elements that can be held in current storage
        size_type capacity() const
        {
            return _capacity;
        }

        // requests removal of unused capacity
        void shrink_to_fit()
        {
            if(_size >= _capacity)
                return;

            allocator_type allocator;
            
            pointer new_elements = allocator.allocate(_size);

            // copy elements            
            for(size_type i = 0; i < size(); i++)
                new_elements[i] = _elements[i];
            
            // free previous allocation
            allocator.deallocate(_elements, _capacity);
            
            _elements = new_elements;
            _capacity = _size;
        }

    //--------------------------------------------------------------
    // MODIFIERS
    //--------------------------------------------------------------
    public:
        // clears the contents
        void clear()
        {
            _size = 0;
        }
        
        // insert
        // emplace
        
        // adds an element to the end
        void push_back(const_reference val)
        {
            resize(size() + 1);
            _elements[_size - 1] = val;
        }

        // erase elements
        iterator erase(iterator pos)
        {
            while((pos+1) != end())
            {
                (*pos) = *(pos + 1);
                pos++;
            }
            _size--;
            
            return pos;
        }

        // resizes the container to contain count elements
        void resize(size_type count)
        {
            if(count < 1)
                return;
            
            if(count > capacity())
            {
                size_type n = (count > recommended_size()) ? count : recommended_size();
                allocator_type allocator;
                
                pointer new_elements = allocator.allocate(n);

                size_type new_capacity = n;

                // copy elements            
                for(size_type i = 0; i < size(); i++)
                    new_elements[i] = _elements[i];
                
                // free previous allocation
                allocator.deallocate(_elements, _capacity);
                
                _elements = new_elements;
                _capacity = new_capacity;
            }
            
            _size = count;
        }
        
        // removes last element of the container
        void pop_back()
        {
            if(!empty())
                _size--;
        }
        
        // swaps the contents
        void swap(vector& x)
        {
            __swap(_elements, x._elements);
            __swap(_capacity, x._capacity);
            __swap(_size, x._size);
        }

        vector& operator=(initializer_list)

        vector& operator=(const vector& rhs)
        {
            if(this != &rhs)
                assign(rhs.begin(), rhs.end());
            
            return *this;
        }
        // vector& operator+=(const vector& rhs);

    private:
        void __init()
        {
            _size = 0;
            _capacity = 0;
            _elements = 0;
        }

        template <class Ty>
        inline void __swap(Ty& x, Ty& y)
        {
            Ty c(x);
            x = y;
            y = c;
        }

        size_type recommended_size() const
        {
            size_type s = 2 * capacity();
            return s;
        }

        size_type _capacity;
        pointer _elements;
        size_type _size;
    };

    // template <class T, class Allocator>
    // inline vector<T, Allocator>& vector<T, Allocator>::operator=(const vector<T, Allocator>& rhs)
    // {
    //     if(this != &rhs)
    //     {
    //         assign(rhs.begin(), rhs.end());
    //     }
    //     return *this;
    // }

} // MorOS


#endif // _CXX_MOROS_VECTOR_H