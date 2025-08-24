#include "../shared/common.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

extern int client_sockets[];  // 客户端套接字数组
extern char client_usernames[][USERNAME_LEN];  // 客户端用户名数组
extern pthread_mutex_t clients_lock;  // 互斥锁
extern int logger_pipe[];  // 日志管道

// 广播消息给所有客户端，排除指定的客户端
void broadcast_message(const ChatMessage *msg, int exclude_fd) {
    pthread_mutex_lock(&clients_lock);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] > 0 && client_sockets[i] != exclude_fd) {
            send(client_sockets[i], msg, sizeof(*msg), 0);
        }
    }
    pthread_mutex_unlock(&clients_lock);
}

// 广播当前在线用户列表
void broadcast_userlist() {
    ChatMessage msg = {0};
    msg.type = MSG_TYPE_USERLIST;
    
    pthread_mutex_lock(&clients_lock);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] > 0) {
            strncpy(msg.userlist[msg.user_count].username, client_usernames[i], USERNAME_LEN);
            msg.userlist[msg.user_count].status = USER_ONLINE;
            msg.user_count++;
        }
    }
    pthread_mutex_unlock(&clients_lock);
    
    broadcast_message(&msg, -1);  // 广播用户列表
}

// 处理单个客户端的连接和消息
void *handle_client(void *arg) {
    int client_fd = *(int *)arg;
    ChatMessage msg;

    // 接收用户名
    if (recv(client_fd, &msg, sizeof(msg), 0) <= 0) {
        close(client_fd);
        return NULL;
    }
    
    // 存储用户名
    pthread_mutex_lock(&clients_lock);
    for (int i = 0; i < MAX_CLIENTS; i++)_

