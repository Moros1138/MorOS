#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "moros_allocator.h"
#include "moros_vector.h"

struct Test
{
    int x;
    int y;

    Test() : x(0), y(0)     { }
    Test(int x, int y) : x(x), y(y) { }
    Test(const Test& rhs)
    {
        x = rhs.x;
        y = rhs.y;
    }

    Test operator=(const Test& rhs)
    {
        if(this != &rhs)
        {
            x = rhs.x;
            y = rhs.y;
        }
        return *this;
    }
};

int main()
{
    MorOS::vector<Test> test{
        {1, 2},
        {3, 4}
    };

    std::vector<Test, MorOS::allocator<Test>> test2{
        {1, 2},
        {3, 4}
    };

    test.insert(test.begin()+1, {10, 11});
    test2.insert(test2.begin()+1, {10, 11});

    for(auto& t : test)
        std::cout << t.x << " " << t.y << std::endl;

    for(auto& t : test2)
        std::cout << t.x << " " << t.y << std::endl;

    return 0;
}