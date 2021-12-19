#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "moros_allocator.h"
#include "moros_vector.h"

int main()
{
    MorOS::vector<int> test{1, 2, 3, 4, 5};
    std::vector<int, MorOS::allocator<int>> test2{1, 2, 3, 4, 5};

    std::cout << test[5] << std::endl;
    std::cout << test.at(5) << std::endl;

    return 0;
}