// -*- C++ -*-

#include "moros_vector.h"

#if defined(__GLIBCXX__) || defined(__GLIBCPP__)
#   include <stdio.h>
#   include <assert.h>
#   include <initializer_list>
#else
#   include "assert.h"
#   include "stdlib.h"
#   include "moros_initializer_list.h"
#endif

namespace MorOS
{
    template <class T, class Allocator = MorOS::allocator<T>>
    class basic_string
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
        basic_string()
        {
        }
        
        basic_string(const basic_string<T>& rhs)
        {
            MOROS_DEBUG("COPY\n");
            _container.clear();
            _container.~vector();

            _container = rhs._container;
        }

        basic_string(pointer value)
        {
            pointer i = value;
            for(; *i != '\0'; i++)
                _container.push_back(*i);
            
            _container.push_back(0);
        }

        basic_string(const_pointer value)
        {
            pointer i = (pointer)value;

            for(; *i != '\0'; i++)
                _container.push_back(*i);
            
            _container.push_back(0);
        }
        
        // operator=
        // assign
        // get_allocator

    //--------------------------------------------------------------
    // ELEMENT ACCESS
    //--------------------------------------------------------------
    public:
        // at
        // operator[]
        // front
        // back
        
        pointer data()
        {
            return _container.data();
        }

        const_pointer c_str()
        {
            return (const_pointer)_container.data();
        }
        

        // operator basic_string_view

    //--------------------------------------------------------------
    // ITERATORS
    //--------------------------------------------------------------
    public:
        // begin
        // end
        // rbegin
        // rend

    //--------------------------------------------------------------
    // CAPACITY
    //--------------------------------------------------------------
    public:
        // empty
        // size
        // length
        // max_size
        // reserve
        // capacity
        // shrink_to_fit

    //--------------------------------------------------------------
    // OPERATIONS
    //--------------------------------------------------------------
    public:

    //--------------------------------------------------------------
    // SEARCH
    //--------------------------------------------------------------
    public:
    
    
    private:
        MorOS::vector<value_type> _container;

    };

    typedef basic_string<char> string;
} // MorOS
