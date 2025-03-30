#ifndef _GRIM_H_
#define _GRIM_H_
#include "buffer.h"
#include <ncurses.h>
#include <fstream>
#include <algorithm>

class Grim {
    private:
        Buffer m_Buffer;
        int m_cursorRow;
        int m_cursorCol;
        char m_mode;
    public:
        Grim();
        void run();
};

#endif