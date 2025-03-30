#include "grim.h"

Grim::Grim() 
    : m_cursorRow(0), m_cursorCol(0), m_mode('n') 
{}

Grim::Grim(const std::string &filename)
    : m_cursorRow(0), m_cursorCol(0), m_mode('n')
{
    // If a filename was passed, attempt to open and load it.
    
    if (!filename.empty() && filename != "") {
        m_filename = filename;
        std::ifstream file(filename);
        if (file.is_open()) {
            // Clear the buffer before loading new content.
            m_Buffer.clear();
            std::string line;
            // Read each line from the file and append it.
            while (std::getline(file, line)) {
                m_Buffer.addLine(line);
            }
        }
        // If file fails to open, you might want to show an error message here.
    }
}
void Grim::run(){
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

    mvwprintw(mainWin, 0, 0, "GRIM Text Editor v0.3");
    mvwprintw(mainWin, 1, 0, "Press Enter...");

    int ch;
    while ((ch = wgetch(mainWin))) {
        if(m_mode == 'n'){
            if(ch == 27){ // ESC exits
                break;
            } else if(ch == 'i'){
                m_mode = 'i';
            } else if(ch == 's'){
                if(m_filename != ""){
                    std::ofstream file(m_filename);
                    for(int row = 0; row < m_Buffer.getLineCount(); row++){
                        file << m_Buffer.getLine(row) << "\n";
                    }
                    file.close();
                }
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