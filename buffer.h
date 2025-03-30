#ifndef _BUFFER_H_
#define _BUFFER_H_
#include <vector>
#include <string>

class Buffer {
    private:
        std::vector<std::string> m_lines;
    public:
        Buffer();
        void insertChar(char ch, int row, int col);
        void insertString(const std::string& str, int row, int col);
        void removeChar(int row, int col);
        void insertNewLine(int row, int col);
        std::string getLine(int row);
        int getLineCount();
};

#endif