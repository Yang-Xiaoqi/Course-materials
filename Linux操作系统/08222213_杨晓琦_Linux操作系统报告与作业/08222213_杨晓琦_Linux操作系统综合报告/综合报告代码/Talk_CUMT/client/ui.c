#include "../shared/common.h"
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>


extern int sock;  // 连接服务器的 Socket 描述符
extern char username[USERNAME_LEN];  // 当前用户的用户名
extern UserInfo online_users[MAX_CLIENTS];  // 在线用户列表
extern int online_user_count;  // 在线用户数量

// 窗口指针
WINDOW *user_win, *chat_win, *input_win;
int screen_height, screen_width;  

// 定义颜色对
enum {
    PAIR_SELF = 1,
    PAIR_SYSTEM,
    PAIR_OTHER,
    PAIR_PRIVATE,
    PAIR_ONLINE,
    PAIR_AWAY,
    PAIR_BUSY
};

// 更新聊天窗口内容
void update_chat(const ChatMessage *msg) {
    switch (msg->type) {
        case MSG_TYPE_SYSTEM:
            wattron(chat_win, COLOR_PAIR(PAIR_SYSTEM));  
            break;
        case MSG_TYPE_PRIVATE:
            wattron(chat_win, COLOR_PAIR(PAIR_PRIVATE));  
            break;
        default:
            // 根据是否是当前用户，选择不同的颜色
            wattron(chat_win, 
                   (strcmp(msg->username, username) == 0) ? 
                   COLOR_PAIR(PAIR_SELF) : COLOR_PAIR(PAIR_OTHER));  
    }
    
    // 打印消息
    wprintw(chat_win, "[%s] %s: %s\n", msg->timestamp, msg->username, msg->message);
    wattroff(chat_win, COLOR_PAIR(PAIR_SELF));
    wattroff(chat_win, COLOR_PAIR(PAIR_SYSTEM));
    wattroff(chat_win, COLOR_PAIR(PAIR_OTHER));
    wattroff(chat_win, COLOR_PAIR(PAIR_PRIVATE));
    wrefresh(chat_win);  // 刷新聊天窗口显示
}

// 绘制在线用户列表
void draw_user_list() {
    werase(user_win);  
    box(user_win, 0, 0);  
    mvwprintw(user_win, 0, 2, " Online Users (%d) ", online_user_count);  
    
    // 打印每个在线用户
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

// 初始化并绘制用户界面
void draw_ui() {
    setlocale(LC_ALL, "");  
    initscr();  
    cbreak();  
    echo();  
    keypad(stdscr, TRUE);  
    start_color(); 
    
    // 初始化颜色对
    init_pair(PAIR_SELF, COLOR_CYAN, COLOR_BLACK);
    init_pair(PAIR_SYSTEM, COLOR_GREEN, COLOR_BLACK);
    init_pair(PAIR_OTHER, COLOR_YELLOW, COLOR_BLACK);
    init_pair(PAIR_PRIVATE, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(PAIR_ONLINE, COLOR_GREEN, COLOR_BLACK);
    init_pair(PAIR_AWAY, COLOR_YELLOW, COLOR_BLACK);
    init_pair(PAIR_BUSY, COLOR_RED, COLOR_BLACK);

    // 获取终端屏幕尺寸
    getmaxyx(stdscr, screen_height, screen_width);
    
    int user_win_width = 20;  
    int input_height = 3;  
    
    // 创建窗口
    user_win = newwin(screen_height - input_height, user_win_width, 0, 0);
    chat_win = newwin(screen_height - input_height, screen_width - user_win_width, 0, user_win_width);
    input_win = newwin(input_height, screen_width, screen_height - input_height, 0);
    
    scrollok(chat_win, TRUE);  
    keypad(input_win, TRUE);  
    
    // 绘制用户列表
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
                // 构造消息
                memset(&msg, 0, sizeof(msg));
                strncpy(msg.username, username, USERNAME_LEN);
                get_timestamp(msg.timestamp, sizeof(msg.timestamp));  // 获取当前时间戳
                
                // 检查是否为私聊命令（以 "/w " 开头）
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
                
                // 本地显示发送的消息
                update_chat(&msg);
                
                // 将消息发送到服务器
                send(sock, &msg, sizeof(msg), 0);
            }
            
            // 清空输入窗口，重新绘制
            werase(input_win);
            box(input_win, 0, 0);
            mvwprintw(input_win, 1, 1, "> ");
            pos = 0;
            draw_user_list();  // 更新用户列表
        } else if (ch == KEY_BACKSPACE || ch == 127) {
            if (pos > 0) {
                pos--;
                mvwaddch(input_win, 1, 3 + pos, ' ');  
            }
        } else if (pos < MAX_MSG_LEN - 1 && ch >= 32 && ch <= 126) {
            input[pos++] = ch;  // 输入一个字符
            mvwaddch(input_win, 1, 3 + pos - 1, ch);  // 在输入窗口显示字符
        }
    }
    
    endwin();  // 退出 ncurses
}

