#include <event.h>

int main()
{
    
    while(1)
    {
        while(event_pending())
        {
            event_t e;
            event_next(&e);
            print_event(&e);
        }
    }
    
    return 0;
}
