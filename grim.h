#ifndef _GRIM_H_
#define _GRIM_H_
#include "buffer.h"
#include <ncurses.h>
#include <fstream>
#include <algorithm>
#include <ctype.h>
#include <stdio.h>
#include <cmath>

class Grim {
    private:
        Buffer m_Buffer;
        Buffer m_CopyBuffer;
        int m_cursorRow;
        int m_cursorCol;
        char m_mode;
        std::string m_filename;
        std::string m_version;

    public:
        Grim(const std::string&);
        void run();
        void saveFile(const std::string&);
        void openFile(const std::string&);
        bool executeCommand(const std::string&);
};

#endif