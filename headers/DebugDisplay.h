#ifndef _DEBUGDISPLAY_INCLUDE
#define _DEBUGDISPLAY_INCLUDE

#include "TextRender.h"

class DebugDisplay {
public:
    DebugDisplay();

    void init();
    bool update(int dt);
    bool render();

private:
    int deltaTime;
    int qtty;
    TextRender textRender;
};

#endif // _DEBUGDISPLAY_INCLUDE
