#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <stdio.h>

#include "moros_allocator.h"
#include "moros_vector.h"
#include "moros_string.h"

int main()
{
    MorOS::vector<int> test = { 1, 2, 3, 4, 5 };
    std::vector<int> stdTest = { 1, 2, 3, 4, 5 };

    test.push_back(13);
    stdTest.push_back(13);

    test.emplace_back(14);
    stdTest.emplace_back(14);

    test.insert(test.begin()+2, 11);
    stdTest.insert(stdTest.begin()+2, 11);

    test.emplace(test.begin()+2, 12);
    stdTest.emplace(stdTest.begin()+2, 12);

    for(auto &i : test)
        std::cout << i << " ";
    
    std::cout << std::endl;


    for(auto &i : stdTest)
        std::cout << i << " ";
    
    std::cout << std::endl;

    return 0;
}