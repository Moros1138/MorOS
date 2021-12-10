#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#define KEY_NONE             0
#define KEY_ESCAPE           1
#define KEY_K1               2
#define KEY_K2               3
#define KEY_K3               4
#define KEY_K4               5
#define KEY_K5               6
#define KEY_K6               7
#define KEY_K7               8
#define KEY_K8               9
#define KEY_K9               10
#define KEY_K0               11
#define KEY_MINUS            12
#define KEY_EQUALS           13
#define KEY_BACKSPACE        14
#define KEY_TAB              15
#define KEY_Q                16
#define KEY_W                17
#define KEY_E                18
#define KEY_R                19
#define KEY_T                20
#define KEY_Y                21
#define KEY_U                22
#define KEY_I                23
#define KEY_O                24
#define KEY_P                25
#define KEY_LEFT_BRACKET     26
#define KEY_RIGHT_BRACKET    27
#define KEY_ENTER            28
#define KEY_LEFT_CONTROL     29
#define KEY_A                30
#define KEY_S                31
#define KEY_D                32
#define KEY_F                33
#define KEY_G                34
#define KEY_H                35
#define KEY_J                36
#define KEY_K                37
#define KEY_L                38
#define KEY_SEMICOLON        39
#define KEY_SINGLEQUOTE      40
#define KEY_APOSTROPHE       41
#define KEY_LEFT_SHIFT       42
#define KEY_BACKSLASH        43
#define KEY_Z                44
#define KEY_X                45
#define KEY_C                46
#define KEY_V                47
#define KEY_B                48
#define KEY_N                49
#define KEY_M                50
#define KEY_COMMA            51
#define KEY_PERIOD           52
#define KEY_SLASH            53
#define KEY_RIGHT_SHIFT      54
#define KEY_NUMPAD_MULTIPLY  55
#define KEY_LEFT_ALT         56
#define KEY_SPACE            57
#define KEY_CAPS_LOCK        58
#define KEY_F1               59
#define KEY_F2               60
#define KEY_F3               61
#define KEY_F4               62
#define KEY_F5               63
#define KEY_F6               64
#define KEY_F7               65
#define KEY_F8               66
#define KEY_F9               67
#define KEY_F10              68
#define KEY_NUM_LOCK         69
#define KEY_SCROLL_LOCK      70
#define KEY_NUMPAD_7         71
#define KEY_NUMPAD_8         72
#define KEY_NUMPAD_9         73
#define KEY_NUMPAD_MINUS     74
#define KEY_NUMPAD_4         75
#define KEY_NUMPAD_5         76
#define KEY_NUMPAD_6         77
#define KEY_NUMPAD_PLUS      78
#define KEY_NUMPAD_1         79
#define KEY_NUMPAD_2         80
#define KEY_NUMPAD_3         81
#define KEY_NUMPAD_0         82
#define KEY_NUMPAD_PERIOD    83
#define KEY_UNUSED1          84
#define KEY_UNUSED2          85
#define KEY_UNUSED3          86
#define KEY_F11              87
#define KEY_F12              88
#define KEY_NUMPAD_ENTER    156
#define KEY_RIGHT_CONTROL   157
#define KEY_NUMPAD_DIVIDE   181
#define KEY_RIGHT_ALT       184
#define KEY_HOME            199
#define KEY_UP              200
#define KEY_PAGE_UP         201
#define KEY_LEFT            203
#define KEY_RIGHT           205
#define KEY_END             207
#define KEY_DOWN            208
#define KEY_PAGE_DOWN       209
#define KEY_INSERT          210
#define KEY_DELETE          211

void keyboard_init();

#ifdef __cplusplus
}
#endif

#endif // KEYBOARD_H