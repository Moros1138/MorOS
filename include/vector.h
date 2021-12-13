#ifndef _CPP_VECTOR_H
#define _CPP_VECTOR_H

#include "assert.h"
#include "stdlib.h"

namespace std
{

    template <typename T>
    class vector
    {
    //--------------------------------------------------------------
    // MEMBER FUNCTIONS
    //--------------------------------------------------------------
    public:
        // constructs the vector
        vector()
        {
            _data = (T*)realloc(NULL, sizeof(T) * 1);
            _size = 0;
            _capacity = 1;
            
            assert(_data != NULL && _size == 0 && _capacity == 1);
        }

        // destructs the vector
        ~vector()
        {
            _size = 0;
            _capacity = 0;
            free(_data);
            
            assert(_size == 0 && _capacity == 0);
        }
        
        // assigns values to the container
        vector<T>& operator=(const vector<T>& other)
        {
            return other;
        }
        
        // assigns values to the container
        vector<T>& assign(const vector<T>& other)
        {
            return other;
        }
        

    //--------------------------------------------------------------
    // ELEMENT ACCESS
    //--------------------------------------------------------------
    public:
        // access specified element with bounds checking
        T& at(size_t pos)
        {
            // TODO: some bounds checking
            return _data[pos];
        }
        
        // access specified element
        T& operator[](size_t pos)
        {
            return _data[pos];
        }
        
        // access the first element
        T& front()
        {
            return *begin();
        }
        
        // access the last element
        T& back()
        {
            return *(end()-1);
        }

        // direct access to the underlying array
        T* data()
        {
            return _data;
        }

    //--------------------------------------------------------------
    // ITERATORS
    //--------------------------------------------------------------
    public:
        // returns an iterator to the beginning
        T* begin()
        {
            return (T*)_data;
        }
        
        // returns an iterator to the end
        T* end()
        {
            return (T*)(_data + _size);
        }

    //--------------------------------------------------------------
    // CAPACITY
    //--------------------------------------------------------------
    public:        
        // checks whether the container is empty
        bool empty()
        {
            return _size == 0;
        }

        // returns the number of elements
        size_t size()
        {
            return _size;
        }
        
        // returns the maximum possible number of elements
        size_t max_size()
        {
            return (size_t)0xffffffff / sizeof(T);
        }

        // reserves storage
        void reserve(size_t capacity)
        {
            T* allocation = (T*)realloc(_data, sizeof(T) * capacity);
            if(allocation)
            {
                _data = allocation;
                _capacity = capacity;
            }

            assert(_data == allocation);
        }

        // returns the number of elements that can be held in currently allocated storage
        size_t capacity()
        {
            return _capacity;
        }
        
        // requests removal of unused capacity.
        void shrink_to_fit()
        {
            reserve(_size);
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

        // void insert(size_t pos, T& value);
        // emplace?
        // T* erase(T* pos);

        // adds an element to the end
        void push_back(const T& value)
        {
            if(_size + 1 > _capacity)
                reserve(_capacity * 2);
            
            *end() = value;
            _size++;
        }

        void push_back(const T&& value)
        {
            if(_size + 1 > _capacity)
                reserve(_capacity * 2);
            
            *end() = value;
            _size++;
        }

        // void emplace_back(const T&& value)
        // {
        //     if(_size + 1 > _capacity)
        //         reserve(_capacity * 2);

        //     end()
        //     push_back(value)
        // }

        // void resize(size_t size)
        // void swap(vector& other)

        // removes last element of the container
        void pop_back()
        {
            if(_size > 0)
                _size--;
        }

    private:
        // the actual array
        T* _data;
        // current capacity
        size_t _capacity;
        // current number of elements in the array
        size_t _size;
    };

} // std

#endif // _CPP_VECTOR_H