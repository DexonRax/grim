#include <ncurses.h>
#include <iostream>

class Buffer{
    private:
        int m_size;
        char *m_buffer;
        int m_length;

    public:
        Buffer(){
            m_size = 1024;
            m_buffer = new char[1024];
            m_length = 0;
            
            for(int i = 0; i < m_size; i++){
                m_buffer[i] = '\0';
            }
        }
        void insertChar(char ch, int i){
            if(i >= 0 && i < m_size){
                for(int x = m_length; x > i; x--){
                    m_buffer[x] = m_buffer[x-1];
                }
                m_buffer[i] = ch;
                m_length++;
            }
        }

        void removeChar(int i){
            if(i >= 0 && i < m_size){
                int buffer_char_end = 0;
                for(int x = i; x < m_length; x++){
                    m_buffer[x] = m_buffer[x+1];
                }
                m_length--;  
            }   
        }
        int getSize(){
            return m_size;
        }
        char* getBuffer(){
            return m_buffer;
        }
        int getLength(){
            return m_length;
        }
};

class Grim{

    private:
        Buffer m_Buffer;
        int m_cursorPos;
        char m_mode; 

    public:
        Grim(){
            m_cursorPos = 0;
            m_mode = 'n';
        }

        void run(){
            initscr();
            initscr();
            set_escdelay(25);
            raw();
            keypad(stdscr, TRUE);
            noecho();

            int rows, cols;
            getmaxyx(stdscr, rows, cols);
            WINDOW* footer = newwin(1, cols, rows - 1, 0);

            mvprintw(0, 0, "GRIM Text Editor v0.1");
            mvprintw(1, 0, "Press Enter...");

            int ch;
            while ((ch = getch())){
                if(m_mode == 'n'){
                    if(ch == 27){
                        break;
                    }else if(ch == 'i'){
                        m_mode = 'i';
                    }
                }else if(m_mode == 'i'){
                    if(ch == 27){
                        m_mode = 'n';
                    }else if(ch == KEY_BACKSPACE){
                        if(m_cursorPos > 0){
                            m_cursorPos--;
                            m_Buffer.removeChar(m_cursorPos);
                        }
                    }else if(ch == KEY_RIGHT){
                        if(m_cursorPos < m_Buffer.getLength()){
                            m_cursorPos++;       
                        }
                    }else if(ch == KEY_LEFT){
                        if(m_cursorPos > 0){
                            m_cursorPos--;
                        }
                    }else if(ch == 10){
                        m_Buffer.insertChar('\n', m_cursorPos);
                    }else if(ch >= 32){
                        m_Buffer.insertChar(ch, m_cursorPos);
                        m_cursorPos++;
                    }
                }
                
                
                
                clear();
                mvprintw(0, 0, m_Buffer.getBuffer());
                move(0, m_cursorPos);
                refresh();
                wclear(footer);
                wattron(footer, A_REVERSE);
                mvwprintw(footer, 0, 0, "Mode: %c | Footer: Press ESC to exit | Last pressed char: %c", m_mode, ch);
                wattroff(footer, A_REVERSE);
                wrefresh(footer);  // Now update the footer on top
                
                refresh();
            }
            endwin();
            std::cout << m_cursorPos << "\n";
        }
};

int main() {
    Grim grim;
    grim.run();
    return 0;
}