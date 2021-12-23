#include <stdio.h>

#define __GLIBCXX__
#define USE_MOROS_DEBUG 2
#include "moros_vector.h"

int main()
{
    MorOS::vector<int> test = { 1, 2, 3, 4, 5 };

    test.push_back(13);
    test.emplace_back(14);
    test.insert(test.begin()+2, 11);
    test.emplace(test.begin()+2, 12);

    for(auto it = test.begin(); it != test.end(); it++)
        printf("%08lx %d\n", it, *it);
    
    printf("\n");
    
 
    return 0;
}