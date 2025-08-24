#include "../shared/common.h"
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>


extern int sock;  // ���ӷ������� Socket ������
extern char username[USERNAME_LEN];  // ��ǰ�û����û���
extern UserInfo online_users[MAX_CLIENTS];  // �����û��б�
extern int online_user_count;  // �����û�����

// ����ָ��
WINDOW *user_win, *chat_win, *input_win;
int screen_height, screen_width;  

// ������ɫ��
enum {
    PAIR_SELF = 1,
    PAIR_SYSTEM,
    PAIR_OTHER,
    PAIR_PRIVATE,
    PAIR_ONLINE,
    PAIR_AWAY,
    PAIR_BUSY
};

// �������촰������
void update_chat(const ChatMessage *msg) {
    switch (msg->type) {
        case MSG_TYPE_SYSTEM:
            wattron(chat_win, COLOR_PAIR(PAIR_SYSTEM));  
            break;
        case MSG_TYPE_PRIVATE:
            wattron(chat_win, COLOR_PAIR(PAIR_PRIVATE));  
            break;
        default:
            // �����Ƿ��ǵ�ǰ�û���ѡ��ͬ����ɫ
            wattron(chat_win, 
                   (strcmp(msg->username, username) == 0) ? 
                   COLOR_PAIR(PAIR_SELF) : COLOR_PAIR(PAIR_OTHER));  
    }
    
    // ��ӡ��Ϣ
    wprintw(chat_win, "[%s] %s: %s\n", msg->timestamp, msg->username, msg->message);
    wattroff(chat_win, COLOR_PAIR(PAIR_SELF));
    wattroff(chat_win, COLOR_PAIR(PAIR_SYSTEM));
    wattroff(chat_win, COLOR_PAIR(PAIR_OTHER));
    wattroff(chat_win, COLOR_PAIR(PAIR_PRIVATE));
    wrefresh(chat_win);  // ˢ�����촰����ʾ
}

// ���������û��б�
void draw_user_list() {
    werase(user_win);  
    box(user_win, 0, 0);  
    mvwprintw(user_win, 0, 2, " Online Users (%d) ", online_user_count);  
    
    // ��ӡÿ�������û�
    for (int i = 0; i < online_user_count && i < screen_height - 4; i++) {
        int color = PAIR_ONLINE;  
        if (online_users[i].status == USER_AWAY) color = PAIR_AWAY;  
        else if (online_users[i].status == USER_BUSY) color = PAIR_BUSY;  
        
        wattron(user_win, COLOR_PAIR(color));
        mvwprintw(user_win, i+1, 1, " %s ", online_users[i].username);  
        wattroff(user_win, COLOR_PAIR(color));  
    }
    wrefresh(user_win);  
}

// ��ʼ���������û�����
void draw_ui() {
    setlocale(LC_ALL, "");  
    initscr();  
    cbreak();  
    echo();  
    keypad(stdscr, TRUE);  
    start_color(); 
    
    // ��ʼ����ɫ��
    init_pair(PAIR_SELF, COLOR_CYAN, COLOR_BLACK);
    init_pair(PAIR_SYSTEM, COLOR_GREEN, COLOR_BLACK);
    init_pair(PAIR_OTHER, COLOR_YELLOW, COLOR_BLACK);
    init_pair(PAIR_PRIVATE, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(PAIR_ONLINE, COLOR_GREEN, COLOR_BLACK);
    init_pair(PAIR_AWAY, COLOR_YELLOW, COLOR_BLACK);
    init_pair(PAIR_BUSY, COLOR_RED, COLOR_BLACK);

    // ��ȡ�ն���Ļ�ߴ�
    getmaxyx(stdscr, screen_height, screen_width);
    
    int user_win_width = 20;  
    int input_height = 3;  
    
    // ��������
    user_win = newwin(screen_height - input_height, user_win_width, 0, 0);
    chat_win = newwin(screen_height - input_height, screen_width - user_win_width, 0, user_win_width);
    input_win = newwin(input_height, screen_width, screen_height - input_height, 0);
    
    scrollok(chat_win, TRUE);  
    keypad(input_win, TRUE);  
    
    // �����û��б�
    draw_user_list();
    box(input_win, 0, 0); 
    mvwprintw(input_win, 1, 1, "> "); 
    wrefresh(user_win);
    wrefresh(chat_win);
    wrefresh(input_win);

    ChatMessage msg;
    char input[MAX_MSG_LEN];
    int ch, pos = 0;
    
    while (1) {
        wmove(input_win, 1, 3 + pos);  
        wrefresh(input_win);
        ch = wgetch(input_win);  
        
        if (ch == '\n') {
            input[pos] = '\0';  
            if (pos > 0) {
                // ������Ϣ
                memset(&msg, 0, sizeof(msg));
                strncpy(msg.username, username, USERNAME_LEN);
                get_timestamp(msg.timestamp, sizeof(msg.timestamp));  // ��ȡ��ǰʱ���
                
                // ����Ƿ�Ϊ˽������� "/w " ��ͷ��
                if (strncmp(input, "/w ", 3) == 0) {
                    char *target = input + 3;
                    char *text = strchr(target, ' ');
                    if (text) {
                        *text++ = '\0';  
                        msg.type = MSG_TYPE_PRIVATE;
                        strncpy(msg.target, target, USERNAME_LEN);
                        strncpy(msg.message, text, MAX_MSG_LEN);
                    }
                } else {
                    msg.type = MSG_TYPE_TEXT;  
                    strncpy(msg.message, input, MAX_MSG_LEN);
                }
                
                // ������ʾ���͵���Ϣ
                update_chat(&msg);
                
                // ����Ϣ���͵�������
                send(sock, &msg, sizeof(msg), 0);
            }
            
            // ������봰�ڣ����»���
            werase(input_win);
            box(input_win, 0, 0);
            mvwprintw(input_win, 1, 1, "> ");
            pos = 0;
            draw_user_list();  // �����û��б�
        } else if (ch == KEY_BACKSPACE || ch == 127) {
            if (pos > 0) {
                pos--;
                mvwaddch(input_win, 1, 3 + pos, ' ');  
            }
        } else if (pos < MAX_MSG_LEN - 1 && ch >= 32 && ch <= 126) {
            input[pos++] = ch;  // ����һ���ַ�
            mvwaddch(input_win, 1, 3 + pos - 1, ch);  // �����봰����ʾ�ַ�
        }
    }
    
    endwin();  // �˳� ncurses
}

