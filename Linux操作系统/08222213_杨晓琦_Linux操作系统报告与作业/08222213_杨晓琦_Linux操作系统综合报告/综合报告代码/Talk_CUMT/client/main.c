#include "../shared/common.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <stdlib.h>

// 外部函数声明
extern void draw_ui();  // 绘制用户界面
extern void *receive_thread(void *);  // 处理接收服务器消息的线程函数

int sock;  // 客户端 socket 描述符
char username[USERNAME_LEN];  // 用户名

int main() {
    // 配置服务器地址信息
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(8888),  // 服务器端口号
        .sin_addr.s_addr = INADDR_ANY  // 服务器地址，INADDR_ANY表示绑定本地所有接口
    };

    // 创建 Socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket"); 
        return 1;  // 创建 Socket 失败，返回错误
    }

    // 连接到服务器
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        return 1;  // 连接服务器失败，返回错误
    }

    // 获取用户输入的用户名
    printf("Enter your username: ");
    fgets(username, sizeof(username), stdin);  // 读取用户名
    username[strcspn(username, "\n")] = 0;  // 去掉输入的换行符

    // 创建并发送初始化消息到服务器
    ChatMessage initmsg = {.type = MSG_TYPE_SYSTEM};  // 初始化消息类型
    strncpy(initmsg.username, username, USERNAME_LEN);  // 设置用户名
    send(sock, &initmsg, sizeof(initmsg), 0);  // 发送消息到服务器

    // 创建线程接收服务器消息
    pthread_t recv_tid;
    pthread_create(&recv_tid, NULL, receive_thread, NULL);  // 启动接收消息的线程

    // 绘制客户端用户界面
    draw_ui();

    // 退出时取消接收线程并关闭连接
    pthread_cancel(recv_tid);  // 取消接收线程
    close(sock);  // 关闭 Socket 连接
    return 0;
}

