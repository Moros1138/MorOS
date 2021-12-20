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
    MorOS::string test1 = "MorOS - Hello, World";
    MorOS::string test2(test1);
    MorOS::string test3{"MorOS - Hello, World"};

    std::string stdTest1 = "MorOS - Hello, World";
    std::string stdTest2("MorOS - Hello, World");
    std::string stdTest3{"MorOS - Hello, World"};

    printf("%s\n", test1.data());
    printf("%s\n", test2.c_str());
    printf("%s\n", test3.data());



    return 0;
}