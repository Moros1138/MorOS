#include "hardware/keyboard.h"
#include "event.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"

char* event_names[] = {
    "reserved     ",
    "reserved     ",
    "KeyPress     ",
    "KeyRelease   ",
    "ButtonPress  ",
    "ButtonRelease",
    "Motion       "
};

#define EVENT_MAX 10000

event_t* eventQueue;

static int front;
static int rear;
static int itemCount;

static bool isEmpty()
{
    return itemCount == 0;
}

static bool isFull()
{
    return itemCount == EVENT_MAX;
}

static void insert(event_t data)
{
    if(!isFull())
    {
        if(rear == EVENT_MAX-1)
        {
            rear = -1;            
        }       

        eventQueue[++rear] = data;
        itemCount++;
    }
}

static event_t removeData()
{
    event_t data = eventQueue[front++];

    if(front == EVENT_MAX)
    {
        front = 0;
    }

    itemCount--;
    return data;
}

void event_init()
{
    eventQueue = (event_t*)malloc(sizeof(event_t) * EVENT_MAX);
    assert(eventQueue != NULL);

    front = 0;
    rear = -1;
    itemCount = 0;
}

void print_event(event_t* e)
{
    printf("TYPE: %s ", event_names[e->type]);

    if(e->type == KeyPress || e->type == KeyRelease)
        printf(" | Scancode(0x%04x) Keycode(0x%02x) | ", e->key.scancode, e->key.keycode);
    
    if(e->type == ButtonPress || e->type == ButtonRelease)
        printf(" | Button(%d) | ", e->button.button);
    
    printf("\n");
}

void event_fire(event_t* event)
{
    insert(*event);
}

bool event_pending()
{
    return !isEmpty();
}

void event_next(event_t* event)
{
    assert(event != NULL);

    // zero out the event
    memset(event, 0, sizeof(event_t));
    if(!isEmpty())
        *event = removeData();
}

