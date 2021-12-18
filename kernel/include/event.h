#ifndef EVENT_H
#define EVENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

#define KeyPress        2
#define KeyRelease      3
#define ButtonPress     4
#define ButtonRelease   5
#define Motion          6

extern char* event_names[];

// KEY EVENT
typedef struct
{
    int type;
    uint8_t  keycode;
    uint16_t scancode;
} key_event_t;

// BUTTON EVENT
typedef struct
{
    int type;
    uint32_t button;
} button_event_t;

// MOTION EVENT
typedef struct
{
    int type;
    int x;
    int y;
    int z;
} motion_event_t;

// comprehensive event union, contains all event types
typedef union _event
{
    int type; // do not change, must be first
    key_event_t key;
    button_event_t button;
    motion_event_t motion;
} event_t;

// initalize the event queue
void event_init();
// print the details of an event for debug purposes
void print_event(event_t* e);
// true; if there are events in the queue
bool event_pending();
// retrieves the next available event, or empty event if queue is empty
void event_next(event_t* event);
// add an event into the queue
void event_fire(event_t* event);

#ifdef __cplusplus
}
#endif

#endif // EVENT_H
