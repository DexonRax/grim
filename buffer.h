#ifndef _BUFFER_H_
#define _BUFFER_H_
#include <vector>
#include <string>

class Buffer {
    private:
        std::vector<std::string> m_lines;
    public:
        Buffer();
        void insertChar(char, int, int);
        void insertString(const std::string&, int, int);
        void removeChar(int, int);
        void insertNewLine(int, int);
        std::string getLine(int);
        int getLineCount();
        void clear();
        void addLine(const std::string&);
        void removeLine(int);
};

#endif