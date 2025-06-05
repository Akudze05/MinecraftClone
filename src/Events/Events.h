
#ifndef EVENTS_H
#define EVENTS_H

typedef unsigned int uint;

class Events {
public:
    static bool* _keys;
    static uint* _frames;
    static uint* _current;
    static float deltaX;
    static float deltaY;
    static float x;
    static float y;

    static int initialize();
};



#endif //EVENTS_H
