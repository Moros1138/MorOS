#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "moros_allocator.h"
#include "moros_vector.h"

// #include "string"

int main()
{
    // std::vector<int, MorOS::allocator<int>> test;
    MorOS::vector<int> test = {1, 2, 3, 4, 5};
    // test.reserve(128);
    // for(int i = 0; i < 128; i++)
    // {
    //     test.push_back(i);
    // }
    
    std::cout << test.size() << " " << test.capacity() << std::endl;
    
    // test.resize(64);
    // std::cout << test.size() << " " << test.capacity() << std::endl;
    
    // for(int i = 0; i < 128; i++)
    // {
    //     test.push_back(i);
    // }

    // for(auto &i : test)
    //     std::cout << i << " ";
    
    // std::cout << std::endl;
        
    // std::cout << test.size() << " " << test.capacity() << std::endl;
    
    // test.shrink_to_fit();

    // std::cout << test.size() << " " << test.capacity() << std::endl;

    return 0;
}