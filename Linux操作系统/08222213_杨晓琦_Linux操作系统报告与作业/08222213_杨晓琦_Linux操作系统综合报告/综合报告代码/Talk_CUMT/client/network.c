#include "../shared/common.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// 外部变量声明
extern int sock;  // 连接服务器的 Socket 描述符
extern void update_chat(const ChatMessage *);  // 更新聊天内容的函数

// 在线用户数组和在线用户数量
UserInfo online_users[MAX_CLIENTS];
int online_user_count = 0;

// 接收消息的线程函数
void *receive_thread(void *arg) {
    ChatMessage msg;  // 存储接收到的消息
    while (1) {
        // 接收消息，阻塞直到收到数据
        ssize_t len = recv(sock, &msg, sizeof(msg), 0);
        
        // 如果接收到数据长度小于等于 0，则退出循环（连接断开或错误）
        if (len <= 0) break;
        
        // 如果收到的是用户列表消息（类型为 MSG_TYPE_USERLIST）
        if (msg.type == MSG_TYPE_USERLIST) {
            // 更新在线用户列表，确保不会超过最大数量
            online_user_count = (msg.user_count < MAX_CLIENTS) ? msg.user_count : MAX_CLIENTS;
            // 复制用户列表到在线用户数组
            memcpy(online_users, msg.userlist, sizeof(UserInfo) * online_user_count);
        }

        // 更新聊天界面，显示收到的消息
        update_chat(&msg);
    }
    return NULL;  // 线程退出
}

