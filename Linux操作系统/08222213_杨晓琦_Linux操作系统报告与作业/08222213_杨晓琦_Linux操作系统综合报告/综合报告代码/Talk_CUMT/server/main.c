#include "../shared/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>

// �ⲿ��������
extern void *handle_client(void *);  // ����ͻ��˵��̺߳���
extern void start_logger(int pipe_fd[2]);  // ������־��¼��

// ȫ�ֱ���
int client_sockets[MAX_CLIENTS] = {0};  // �洢�ͻ���socket
char client_usernames[MAX_CLIENTS][USERNAME_LEN] = {{0}};  // �洢�ͻ����û���
pthread_mutex_t clients_lock = PTHREAD_MUTEX_INITIALIZER;  // ����client_sockets����
int logger_pipe[2];  // ������־�Ĺܵ�

int main() {
    // ����SIGPIPE�źţ�������д���ѶϿ�������ʱ�������
    signal(SIGPIPE, SIG_IGN);

    // ����������socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // ���÷�������ַ�Ͷ˿�
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(8888),
        .sin_addr.s_addr = INADDR_ANY
    };

    // �󶨷�������ַ
    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));

    // ��ʼ����
    listen(server_fd, 10);

    // �����ܵ���������־��¼����
    pipe(logger_pipe);
    if (fork() == 0) {
        close(logger_pipe[1]);  // �ӽ��̹ر�д��
        start_logger(logger_pipe);  // ������־��¼��
        exit(0);  // �����ӽ���
    }
    close(logger_pipe[0]);  // �����̹رն���

    printf("TermiTalk Server started on port 8888...\n");

    // ��������ѭ�����ȴ������ܿͻ�������
    while (1) {
        // ���ܿͻ�������
        int client_fd = accept(server_fd, NULL, NULL);
        
        // ��ȡ����Ϊÿ���ͻ��˷���һ���յ�socket��
        pthread_mutex_lock(&clients_lock);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] == 0) {
                client_sockets[i] = client_fd;  // ���ͻ���socket��ӵ�����
                pthread_t tid;
                // Ϊÿ���ͻ��˴����߳�
                pthread_create(&tid, NULL, handle_client, &client_fd);
                pthread_detach(tid);  // �����߳�
                break;
            }
        }
        pthread_mutex_unlock(&clients_lock);
    }
    return 0;
}

