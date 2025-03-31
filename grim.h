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
        std::string m_filename;
        
    public:
        Grim();
        Grim(const std::string&);
        void run();
        void saveFile(const std::string&);
};

#endif