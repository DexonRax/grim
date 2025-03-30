#include <ncurses.h>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

class Buffer {
private:
    std::vector<std::string> m_lines;
public:
    Buffer(){
        m_lines.push_back(""); // Start with one empty line
    }
    void insertChar(char ch, int row, int col){
        if(row >= 0 && row < m_lines.size()){
            if(col >= 0 && col <= (int)m_lines[row].length()){
                m_lines[row].insert(col, 1, ch);
            }
        }
    }
    void insertString(const std::string& str, int row, int col){
        if(row >= 0 && row < m_lines.size()){
            if(col >= 0 && col <= (int)m_lines[row].length()){
                m_lines[row].insert(col, str);
            }
        }
    }
    void removeChar(int row, int col){
        if(row >= 0 && row < m_lines.size() && col > 0 && col <= (int)m_lines[row].length()){
            m_lines[row].erase(col - 1, 1);
        }
    }
    void insertNewLine(int row, int col){
        if(row >= 0 && row < m_lines.size()){
            std::string newLine = m_lines[row].substr(col);
            m_lines[row] = m_lines[row].substr(0, col);
            m_lines.insert(m_lines.begin() + row + 1, newLine);
        }
    }
    std::string getLine(int row){
        if(row >= 0 && row < m_lines.size()){
            return m_lines[row];
        }
        return "";
    }
    int getLineCount(){
        return m_lines.size();
    }
};

class Grim {
private:
    Buffer m_Buffer;
    int m_cursorRow;
    int m_cursorCol;
    char m_mode;
public:
    Grim(): m_cursorRow(0), m_cursorCol(0), m_mode('n') {}
    void run(){
        initscr();
        raw();
        set_escdelay(25);
        noecho();
        curs_set(1);  // Ensure the cursor is enabled

        int rows, cols;
        getmaxyx(stdscr, rows, cols);
        // Create subwindows which share stdscr's memory:
        WINDOW* mainWin = subwin(stdscr, rows - 1, cols, 0, 0);
        WINDOW* footer = subwin(stdscr, 1, cols, rows - 1, 0);

        // Enable keypad on mainWin so it can properly interpret special keys.
        keypad(mainWin, TRUE);

        int ch;
        while ((ch = wgetch(mainWin))) {
            if(m_mode == 'n'){
                if(ch == 27){ // ESC exits
                    break;
                } else if(ch == 'i'){
                    m_mode = 'i';
                } else if(ch == 's'){
                    std::ofstream file("text.txt");
                    for(int row = 0; row < m_Buffer.getLineCount(); row++){
                        file << m_Buffer.getLine(row) << "\n";
                    }
                    file.close();
                }
            } else if(m_mode == 'i'){
                if(ch == 27){ // ESC to exit insert mode
                    m_mode = 'n';
                } else if(ch == KEY_BACKSPACE){
                    if(m_cursorCol > 0){
                        m_Buffer.removeChar(m_cursorRow, m_cursorCol);
                        m_cursorCol--;
                    }
                } else if(ch == KEY_RIGHT){
                    if(m_cursorCol < (int)m_Buffer.getLine(m_cursorRow).length()){
                        m_cursorCol++;
                    }
                } else if(ch == KEY_LEFT){
                    if(m_cursorCol > 0){
                        m_cursorCol--;
                    }
                } else if(ch == KEY_UP){
                    if(m_cursorRow > 0){
                        m_cursorRow--;
                        m_cursorCol = std::min((int)m_Buffer.getLine(m_cursorRow).length(), m_cursorCol);
                    }
                } else if(ch == KEY_DOWN){
                    if(m_cursorRow < m_Buffer.getLineCount() - 1){
                        m_cursorRow++;
                        m_cursorCol = std::min((int)m_Buffer.getLine(m_cursorRow).length(), m_cursorCol);
                    }
                } else if(ch == 10){ // Enter key
                    m_Buffer.insertNewLine(m_cursorRow, m_cursorCol);
                    m_cursorRow++;
                    m_cursorCol = 0;
                } else if(ch == 9){ // Tab key
                    std::string tab = "    ";
                    m_Buffer.insertString(tab, m_cursorRow, m_cursorCol);
                    m_cursorCol += tab.length();
                } else if(ch >= 32){
                    m_Buffer.insertChar(ch, m_cursorRow, m_cursorCol);
                    m_cursorCol++;
                }
            }

            // Clear and redraw the main window with the text buffer
            wclear(mainWin);
            for(int row = 0; row < m_Buffer.getLineCount(); row++){
                mvwprintw(mainWin, row, 0, m_Buffer.getLine(row).c_str());
            }
            // Position the cursor in the main window and refresh it
            wmove(mainWin, m_cursorRow, m_cursorCol);
            wrefresh(mainWin);

            // Update the footer window with status info and refresh it
            wclear(footer);
            wattron(footer, A_REVERSE);
            mvwprintw(footer, 0, 0, "Mode: %c | Cursor: %d, %d", m_mode, m_cursorRow, m_cursorCol);
            wattroff(footer, A_REVERSE);
            wrefresh(footer);

            // Update the hardware cursor position on stdscr
            move(m_cursorRow, m_cursorCol);
            refresh();

        }
        endwin();
    }
};

int main() {
    Grim grim;
    grim.run();
    return 0;
}
