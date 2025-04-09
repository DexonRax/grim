#include "grim.h"

Grim::Grim(const std::string &filename)
    : m_cursorRow(0), m_cursorCol(0), m_mode('n'), m_version("v0.9.7.0")
{
    m_filename.clear();
    // If a filename was passed, attempt to open and load it.
    
    openFile(filename);
}

void Grim::openFile(const std::string &filename){
    if (!filename.empty() && filename != "") {
        std::ifstream file(filename);
        if (file.is_open()) {
            // Clear the buffer before loading new content.
            m_Buffer.clear();
            std::string line;
            // Read each line from the file and append it.
            while (std::getline(file, line)) {
                m_Buffer.addLine(line);
            }
            m_filename = filename;
        }else{
            m_filename.clear();
            //TODO: If file fails to open, you might want to show an error message here.
        }
    }else{
        m_Buffer.clear();
        m_Buffer.addLine("");
        m_filename.clear();
    }
}

void Grim::saveFile(const std::string &filename){
    std::ofstream file(filename);
    for(int row = 0; row < m_Buffer.getLineCount(); row++){
        file << m_Buffer.getLine(row) << "\n";
    }
    file.close();
    m_filename = filename;
}

bool Grim::executeCommand(const std::string &command){
    int len = command.size();
    
    if (len > 1) {
        int num = 0;
        int numLen = 0;
    
        while (numLen < len && isdigit(command[numLen])) {
            num = num * 10 + (command[numLen] - '0');
            numLen++;
        }

        if (len == numLen + 1) {
            if(command[numLen] == 'd'){
                for (int i = 0; i < num; i++) {
                    m_Buffer.removeLine(m_cursorRow);
                }
                if (m_Buffer.getLineCount() == 0) {
                    m_Buffer.insertNewLine(0, 0);
                }
            }
            return true;
        }
    
        if (len == 2 && numLen == 0) {
            if(command == "dd"){
                m_Buffer.removeLine(m_cursorRow);
                if (m_Buffer.getLineCount() == 0) {
                    m_Buffer.insertNewLine(0, 0);
                }
            }
            return true;
        }
    }
    return false;
    
}

void Grim::run(){
    initscr();
    //raw();
    cbreak();
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

    mvwprintw(mainWin, 0, 0, ("GRIM Text Editor "+m_version).c_str());
    mvwprintw(mainWin, 1, 0, "Press Enter...");

    std::string saveFilename = "";
    std::string openFilename = "";

    std::string command = "";
    char ch;
    while ((ch = wgetch(mainWin))) {
        if(m_mode != 'n'){
            command = "";
        }


        if(ch == -102){ //Window resize
            getmaxyx(stdscr, rows, cols);
            wresize(mainWin, rows - 1, cols);
            werase(footer);
            wresize(footer, 1, cols);
            mvwin(footer, rows - 1, 0);
            wrefresh(mainWin);
            wrefresh(footer);
        }


        if(ch == 27){ //Escape
            if(m_mode == 'n'){
                m_mode = 'q';
            }else if(m_mode == 'q'){
                break;
            }else if(m_mode == 'o' || m_mode == 's'){
                openFilename.clear();
                m_mode = 'n';
            }else if(m_mode == 'i'){
                m_mode = 'n';
            }
        }


        if(m_mode == 'n'){
            if(ch == 'i'){
                m_mode = 'i';
            }else if(ch == 's'){
                m_mode = 's';
                saveFilename = m_filename;
            }else if(ch == 'o'){
                m_mode = 'o';
            }
        }


        if(ch == 5){ //RIGHT
            if(m_cursorCol < (int)m_Buffer.getLine(m_cursorRow).length()){
                m_cursorCol++;
            }else if(m_Buffer.getLineCount()-1 > m_cursorRow){
                m_cursorRow++;
                m_cursorCol = 0;
            }
        }else if(ch == 4){ //LEFT
            if(m_cursorCol > 0){
                m_cursorCol--;
            }
        }else if(ch == 3){ //UP
            if(m_cursorRow > 0){
                m_cursorRow--;
                m_cursorCol = std::min((int)m_Buffer.getLine(m_cursorRow).length(), m_cursorCol);
            }
        }else if(ch == 2){ //DOWN
            if(m_cursorRow < m_Buffer.getLineCount() - 1){
                m_cursorRow++;
                m_cursorCol = std::min((int)m_Buffer.getLine(m_cursorRow).length(), m_cursorCol);
            }


        }else if(ch == 7 || ch == 8){ //Backspace
            if(m_mode == 'n'){
                if(command.size() > 0)
                    command.erase(command.size()-1, 1);
            }else if(m_mode == 'o'){
                if(openFilename.size() > 0)
                    openFilename = openFilename.substr(0, openFilename.size()-1);
            }else if(m_mode == 's'){
                if(saveFilename.size() > 0)
                    saveFilename = saveFilename.substr(0, saveFilename.size()-1);
            }else if(m_mode == 'i'){
                if(m_cursorCol > 0){
                    m_Buffer.removeChar(m_cursorRow, m_cursorCol);
                    m_cursorCol--;
                }else{
                    if(m_cursorRow > 0){
                        if(m_Buffer.getLine(m_cursorRow).length() > 0){
                            int m_col =  m_Buffer.getLine(m_cursorRow-1).length();
                            m_Buffer.insertString(m_Buffer.getLine(m_cursorRow).c_str(), m_cursorRow-1, m_col);
                            m_Buffer.removeLine(m_cursorRow);
                            m_cursorRow--;
                            m_cursorCol = m_col;
                        }else{
                            m_Buffer.removeLine(m_cursorRow);
                            m_cursorRow--; 
                            m_cursorCol = m_Buffer.getLine(m_cursorRow).length();
                        }
                    }
                }
            }


        }else if(ch == 10){ // Enter key
            if(m_mode == 'q'){
                m_mode = 'n';
            }else if(m_mode == 'o'){
                openFile(openFilename);
                openFilename.clear();
                m_mode = 'n';
            }else if(m_mode == 's'){
                saveFile(saveFilename);
                m_filename = saveFilename;
                m_mode = 'n';
            }else if(m_mode == 'i'){
                m_Buffer.insertNewLine(m_cursorRow, m_cursorCol);
                m_cursorRow++;
                m_cursorCol = 0;
            }

        }else if(ch == 9){ // Tab key
            if(m_mode == 'i'){
                std::string tab = "    ";
                m_Buffer.insertString(tab, m_cursorRow, m_cursorCol);
                m_cursorCol += tab.length();
            }


        }else if(ch >= 32){
            if(m_mode == 'n'){
                command += ch;
                if(executeCommand(command)){
                    command.clear();
                }
            }else if(m_mode == 'o'){
                openFilename += ch;
            }else if(m_mode == 's'){
                saveFilename += ch;
            }else if(m_mode == 'i'){
                m_Buffer.insertChar(ch, m_cursorRow, m_cursorCol);
                m_cursorCol++;
            }

        }

        // Clear and redraw the main window with the text buffer
        wclear(mainWin);
        if(m_mode != 'q'){
            for(int row = 0; row < m_Buffer.getLineCount(); row++){
                mvwprintw(mainWin, row, 0, "%s", m_Buffer.getLine(row).c_str());
            }
        }else{
            mvwprintw(mainWin, 0, 0, "Press Escape to Quit...");
            mvwprintw(mainWin, 1, 0, "Press Enter to Return...");
            mvwprintw(mainWin, 3, 0, ("GRIM Text Editor "+m_version).c_str());
        }

        // Update the footer window with status info and refresh it
        wclear(footer);
        wattron(footer, A_REVERSE);

        if(m_mode == 's'){
            mvwprintw(footer, 0, 0, "Filename: %s", saveFilename.c_str());
        }else if(m_mode == 'o'){
            mvwprintw(footer, 0, 0, "Filename: %s", openFilename.c_str());
        }else if(m_mode == 'q'){
            mvwprintw(footer, 0, 0, "(c) GRM 2020-2025");
            m_cursorCol = 0;
            m_cursorRow = 0;
        }else{
            std::string fn = "";
            if(m_filename == ""){
                fn = "[new file]";
            }else{
                fn = m_filename;
            }

            mvwprintw(footer, 0, 0, "Filename: %s | Mode: %c | Cursor: %d, %d | Key code: %d | Command: %s" , fn.c_str(), m_mode, m_cursorRow, m_cursorCol, ch, command.c_str());
        }
        
        wattroff(footer, A_REVERSE);
        wrefresh(footer);

        // Position the cursor in the main window and refresh it
        wmove(mainWin, m_cursorRow, m_cursorCol);
        wrefresh(mainWin);

        // This is unnecessary - already handled by wmove() and wrefresh() above
        // move(m_cursorRow, m_cursorCol);
        // refresh();
    }
    endwin();
}