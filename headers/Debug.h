#ifndef _DEBUG_INCLUDE
#define _DEBUG_INCLUDE

#include <iostream>
#include "TileMapLoader.h"

class Debug {
public:
    static void print(const TileRowModels& t) {
        for (auto s : t) {
            std::cout << s << " ";
        }
        std::cout << std::endl;
    }

    static void print(const TileMapModels& t) {
        for (auto r : t) {
            print(r);
        }
    }
};

#endif // ifndef _DEBUG_INCLUDE
