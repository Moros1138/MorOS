// -*- C++ -*-
#ifndef _CXX_MOROS_VECTOR_H
#define _CXX_MOROS_VECTOR_H

#include "moros_config.h"
#include "moros_allocator.h"
#include "moros_utility.h"

#if !defined(__MOROS_KERNEL__)
#   include <stdio.h>
#   include <assert.h>
#   include <initializer_list>
#else
#   include "assert.h"
#   include "stdlib.h"
#   include "moros_initializer_list.h"
#endif

#include "moros_iterator.h"

namespace __MOROS_NAMESPACE__
{
    template<class T, class Allocator = __MOROS_NAMESPACE__::allocator<T>>
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
        typedef __MOROS_NAMESPACE__::iterator<T> iterator;
        typedef __MOROS_NAMESPACE__::reverse_iterator<T> reverse_iterator;
        typedef const __MOROS_NAMESPACE__::iterator<T> const_iterator;
        typedef const __MOROS_NAMESPACE__::reverse_iterator<T> const_reverse_iterator;
        typedef Allocator allocator_type;
        typedef size_t size_type;

    //--------------------------------------------------------------
    // MEMBER FUNCTIONS
    //--------------------------------------------------------------
    public:
        vector() : _capacity(0), _elements(0), _size(0)
        {
        }

        vector(const vector& rhs)
        {
            _init();
            assign(rhs.begin(), rhs.end());
        }

        vector(std::initializer_list<value_type> ilist, const allocator_type& = allocator_type())
        {
            _init();
            // reserve(ilist.size());
            assign(ilist.begin(), ilist.end());
        }

        ~vector()
        {
            
            if(_elements)
                allocator.deallocate(_elements, _capacity);
            
            _capacity = 0;
            _size = 0;
        }

        // assigns values to the container
        vector<T>& operator=(const vector& rhs)
        {
            if(this != &rhs)
                assign(rhs.begin(), rhs.end());
            
            return *this;
        }
        // assigns values to the container
        void assign(size_type count, const_reference value)
        {
            MOROS_INFO("MorOS::vector::assign(size_type count, const_reference value)\n");
            clear();
            for(size_type i = 0; i < count; i++)
                push_back(value);
        }
        // assigns values to the container
        template <class InputIterator>
        void assign(InputIterator first, InputIterator last)
        {
            MOROS_INFO("MorOS::vector::assign(InputIterator first, InputIterator last)\n");
            clear();
            for(; first != last; ++first)
            {
                push_back(*first);
            }
        }
        // returns the allocator associated with the container
        allocator_type get_allocator() const
        {
            return allocator;
        }

    //--------------------------------------------------------------
    // ELEMENT ACCESS
    //--------------------------------------------------------------
    public:
        // access specified element wiht bounds checking
        reference at(size_type pos)
        {
            _range_check(pos);
            return _elements[pos];
        }
        // access specified element wiht bounds checking
        const_reference at(size_type pos) const
        {
            _range_check(pos);
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
        
        iterator begin(void)
        {
            return iterator(_elements + 0);
        }
        // returns an iterator to the beginning
        const_iterator cbegin(void) const
        {
            return const_iterator(begin());
        }
        iterator end(void)
        {
            return iterator(_elements + _size);
        }
        // returns an iterator to the end
        const_iterator cend(void) const
        {
            return const_iterator(end());
        }
        reverse_iterator rbegin(void)
        {
            return reverse_iterator(_elements + _size - 1);
        }
        const_reverse_iterator crbegin(void)
        {
            return const_reverse_iterator(rbegin());
        }
        reverse_iterator rend(void)
        {
            return reverse_iterator(_elements - 1);
        }
        const_reverse_iterator crend(void)
        {
            return const_reverse_iterator(rend());
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
        // insert items
        iterator insert(const_iterator pos, const_reference value)
        {
            iterator insertIndex = pos - begin();
            resize(size() + 1);
            insertIndex = begin() + insertIndex;

            // shift all elements after the insertIndex by 1 place
            for(auto it = end()-1; it != insertIndex-1; it--)
                *(it+1) = *it;
            
            *insertIndex = value;
            
            return insertIndex;
        }
        // insert items
        iterator insert(const_iterator pos, T&& value)
        {
            iterator insertIndex = pos - begin();
            resize(size() + 1);
            insertIndex = begin() + insertIndex;

            // shift all elements after the insertIndex by 1 place
            for(auto it = end()-1; it != insertIndex-1; it--)
                *(it+1) = *it;
            
            *insertIndex = value;
            
            return insertIndex;
        }
        // constructs an element in-place
        template <typename... Args>
        void emplace(const_iterator pos, Args&&... args)
        {
            iterator insertIndex = pos - begin();
            resize(size() + 1);
            insertIndex = begin() + insertIndex;

            // shift all elements after the insertIndex by 1 place
            for(auto it = end()-1; it != insertIndex-1; it--)
                *(it+1) = *it;
            
            *insertIndex = value_type(__MOROS_NAMESPACE__::forward<Args>(args)...);
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
        // adds an element to the end
        void push_back(const_reference val)
        {
            MOROS_INFO("MorOS::vector::push_back(const_reference val)\n");
            resize(size() + 1);
            *(end()-1) = val;
            
        }
        // constructs an element in-place at the end
        template <typename... Args>
        void emplace_back(Args&&... args)
        {
            MOROS_INFO("MorOS::vector::emplace_back(Args&&... args)\n");
            resize(size() + 1);
            *(end()-1) = value_type(__MOROS_NAMESPACE__::forward<Args>(args)...);
        }
        // removes last element of the container
        void pop_back()
        {
            if(!empty())
                _size--;
        }
        // resizes the container to contain count elements
        void resize(size_type count)
        {
            MOROS_INFO("MorOS::vector::resize(size_type count)\n");
            if(count < 1)
                return;
            
            if(count > capacity())
            {
                size_type n = (count > recommended_size()) ? count : recommended_size();
                
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
        // swaps the contents
        void swap(vector& x)
        {
            __swap(_elements, x._elements);
            __swap(_capacity, x._capacity);
            __swap(_size, x._size);
        }
        
    private:
        void _init()
        {
            _size = 0;
            _capacity = 0;
            _elements = 0;
        }
        void _range_check(size_type n)
        {
            assert(!(n >= size()));
        }
        // internal swap function, see (swap)
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

        allocator_type allocator; // allocator
        size_type _capacity; // capacity
        pointer _elements;   // the actual data
        size_type _size;     // size
    };

} // __MOROS_NAMESPACE__


#endif // _CXX_MOROS_VECTOR_H