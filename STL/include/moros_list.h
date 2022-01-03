// -*- C++ -*-
#ifndef _CXX_MOROS_LIST_H
#define _CXX_MOROS_LIST_H

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
    template<class T>
    struct _list_node
    {
        _list_node() : _prev(nullptr), _next(nullptr), _value(0) { }
        _list_node<T>* _prev;
        _list_node<T>* _next;
        T _value;
    };

    template<class T, class Allocator = __MOROS_NAMESPACE__::allocator<_list_node<T>>>
    class list
    {
    public:
        typedef Allocator allocator_type;
        typedef size_t size_type;
        typedef _list_node<T>* node_pointer;
        
        
        list() : _size(0), _head(nullptr), _tail(nullptr)
        {
        }

        void push_back(const T& value)
        {
            _list_node<T>* temp = allocator.allocate(1);
            temp->_value = value;

            if(_head == nullptr)
            {
                temp->_prev = nullptr;
                temp->_next = nullptr;
                _head = temp;
                _tail = temp;
                return;
            }

            _tail->_next = temp;
            temp->_prev = _tail;
            _tail = temp;
        }

        size_type size() const
        {
            return _size;
        }

        node_pointer front() const
        {
            return _head;
        }

        node_pointer back() const
        {
            return _tail;
        }

    private:
        void _init()
        {
            _size = 0;
            _head = nullptr;
            _tail = nullptr;
        }
        
        allocator_type allocator;
        size_type _size;
        node_pointer _head;
        node_pointer _tail;

    };

} // __MOROS_NAMESPACE__


#endif // _CXX_MOROS_LIST_H