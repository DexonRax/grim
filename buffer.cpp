#include "buffer.h"

Buffer::Buffer(){
    m_lines.push_back(""); // Start with one empty line
}
void Buffer::insertChar(char ch, int row, int col){
    if(row >= 0 && row < m_lines.size()){
        if(col >= 0 && col <= (int)m_lines[row].length()){
            m_lines[row].insert(col, 1, ch);
        }
    }
}
void Buffer::insertString(const std::string& str, int row, int col){
    if(row >= 0 && row < m_lines.size()){
        if(col >= 0 && col <= (int)m_lines[row].length()){
            m_lines[row].insert(col, str);
        }
    }
}
void Buffer::removeChar(int row, int col){
    if(row >= 0 && row < m_lines.size() && col > 0 && col <= (int)m_lines[row].length()){
        m_lines[row].erase(col - 1, 1);
    }
}
void Buffer::insertNewLine(int row, int col){
    if(row >= 0 && row < m_lines.size()){
        std::string newLine = m_lines[row].substr(col);
        m_lines[row] = m_lines[row].substr(0, col);
        m_lines.insert(m_lines.begin() + row + 1, newLine);
    }
}
std::string Buffer::getLine(int row){
    if(row >= 0 && row < m_lines.size()){
        return m_lines[row];
    }
    return "";
}
int Buffer::getLineCount(){
    return m_lines.size();
}

void Buffer::clear() {
    m_lines.clear();
}

void Buffer::addLine(const std::string& line) {
    m_lines.push_back(line);
}

void Buffer::removeLine(int i){
    if (i >= 0 && i < m_lines.size()) {
        m_lines.erase(m_lines.begin() + i);
    }
}