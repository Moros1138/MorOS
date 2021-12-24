// -*- C++ -*-
#ifndef _CXX_MOROS_ITERATOR_H
#define _CXX_MOROS_ITERATOR_H

#include "moros_config.h"

namespace __MOROS_NAMESPACE__
{
    struct input_iterator_tag { };
    struct output_iterator_tag { };
    struct forward_iterator_tag : public input_iterator_tag { };
    struct bidirectional_iterator_tag : public forward_iterator_tag { };
    struct random_access_iterator_tag : public bidirectional_iterator_tag { };

    template<class T>
    class iterator
    {
        public:
            typedef iterator _self;
            typedef T value_type;
            typedef T& reference;
            typedef T* pointer;
            typedef __MOROS_NAMESPACE__::bidirectional_iterator_tag iterator_category;
            typedef int difference_type;

            iterator(pointer ptr)
                : ptr_(ptr)
            { }
            
            _self& operator=(const _self& rhs) = default;

            _self operator--()
            {
                _self temp = *this;
                ptr_--;
                return temp;
            }
            
            _self operator--(int)
            {
                _self temp = *this;
                ptr_--;
                return temp;
            }


            _self operator++()
            {
                _self temp = *this;
                ptr_++;
                return temp;
            }
            
            _self operator++(int)
            {
                _self temp = *this;
                ptr_++;
                return temp;
            }
            
            reference operator*()
            { return *ptr_; }
            
            pointer operator->()
            { return ptr_; }
            
            _self operator+(const int& rhs) const
            {
                return _self((pointer)((size_t)ptr_ + (size_t)rhs * sizeof(T)));
            }
            
            _self operator+(const _self& rhs) const
            {
                return _self((pointer)((size_t)ptr_ + (size_t)rhs.ptr_));
            }
            
            _self operator-(const int& rhs) const
            {
                return _self((pointer)((size_t)ptr_ - (size_t)rhs * sizeof(T)));
            }
            
            _self operator-(const _self& rhs) const
            {
                return _self((pointer)((size_t)ptr_ - (size_t)rhs.ptr_));
            }

            bool operator==(const _self& rhs)
            { return ptr_ == rhs.ptr_; }
            
            bool operator!=(const _self& rhs)
            { return ptr_ != rhs.ptr_; }

        private:
            pointer ptr_;
    };

    template<class T>
    class reverse_iterator
    {
        public:
            typedef T value_type;
            typedef T& reference;
            typedef T* pointer;
            typedef __MOROS_NAMESPACE__::bidirectional_iterator_tag iterator_category;
            typedef int difference_type;

            reverse_iterator(pointer ptr)
                : ptr_(ptr)
            { }
            
            reverse_iterator& operator=(const reverse_iterator& rhs) = default;
            
            reverse_iterator operator++()
            {
                reverse_iterator temp = *this;
                ptr_--;
                return temp;
            }
            
            reverse_iterator operator++(int)
            {
                reverse_iterator temp = *this;
                ptr_--;
                return temp;
            }
            
            reverse_iterator operator--()
            {
                reverse_iterator temp = *this;
                ptr_++;
                return temp;
            }
            
            reverse_iterator operator--(int)
            {
                reverse_iterator temp = *this;
                ptr_++;
                return temp;
            }


            reference operator*()
            { return *ptr_; }
            
            pointer operator->()
            { return ptr_; }
            
            reverse_iterator operator+(const int& rhs) const
            {
                return reverse_iterator((pointer)((size_t)ptr_ - (size_t)rhs * sizeof(T)));
            }
            
            reverse_iterator operator+(const reverse_iterator& rhs) const
            {
                return reverse_iterator((pointer)((size_t)ptr_ - (size_t)rhs.ptr_));
            }
            
            reverse_iterator operator-(const int& rhs) const
            {
                return reverse_iterator((pointer)((size_t)ptr_ + (size_t)rhs * sizeof(T)));
            }
            
            reverse_iterator operator-(const reverse_iterator& rhs) const
            {
                return reverse_iterator((pointer)((size_t)ptr_ + (size_t)rhs.ptr_));
            }

            bool operator==(const reverse_iterator& rhs)
            { return ptr_ == rhs.ptr_; }
            
            bool operator!=(const reverse_iterator& rhs)
            { return ptr_ != rhs.ptr_; }

        private:
            pointer ptr_;
    };


} // __MOROS_NAMESPACE__

#endif // _CXX_MOROS_ITERATOR_H
