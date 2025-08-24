#include "../shared/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>

// 外部函数声明
extern void *handle_client(void *);  // 处理客户端的线程函数
extern void start_logger(int pipe_fd[2]);  // 启动日志记录器

// 全局变量
int client_sockets[MAX_CLIENTS] = {0};  // 存储客户端socket
char client_usernames[MAX_CLIENTS][USERNAME_LEN] = {{0}};  // 存储客户端用户名
pthread_mutex_t clients_lock = PTHREAD_MUTEX_INITIALIZER;  // 保护client_sockets的锁
int logger_pipe[2];  // 用于日志的管道

int main() {
    // 忽略SIGPIPE信号，避免在写入已断开的连接时程序崩溃
    signal(SIGPIPE, SIG_IGN);

    // 创建服务器socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 设置服务器地址和端口
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(8888),
        .sin_addr.s_addr = INADDR_ANY
    };

    // 绑定服务器地址
    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));

    // 开始监听
    listen(server_fd, 10);

    // 创建管道并启动日志记录进程
    pipe(logger_pipe);
    if (fork() == 0) {
        close(logger_pipe[1]);  // 子进程关闭写端
        start_logger(logger_pipe);  // 启动日志记录器
        exit(0);  // 结束子进程
    }
    close(logger_pipe[0]);  // 父进程关闭读端

    printf("TermiTalk Server started on port 8888...\n");

    // 服务器主循环：等待并接受客户端连接
    while (1) {
        // 接受客户端连接
        int client_fd = accept(server_fd, NULL, NULL);
        
        // 获取锁并为每个客户端分配一个空的socket槽
        pthread_mutex_lock(&clients_lock);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] == 0) {
                client_sockets[i] = client_fd;  // 将客户端socket添加到数组
                pthread_t tid;
                // 为每个客户端创建线程
                pthread_create(&tid, NULL, handle_client, &client_fd);
                pthread_detach(tid);  // 分离线程
                break;
            }
        }
        pthread_mutex_unlock(&clients_lock);
    }
    return 0;
}

