#include "../shared/common.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <stdlib.h>

// �ⲿ��������
extern void draw_ui();  // �����û�����
extern void *receive_thread(void *);  // ������շ�������Ϣ���̺߳���

int sock;  // �ͻ��� socket ������
char username[USERNAME_LEN];  // �û���

int main() {
    // ���÷�������ַ��Ϣ
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(8888),  // �������˿ں�
        .sin_addr.s_addr = INADDR_ANY  // ��������ַ��INADDR_ANY��ʾ�󶨱������нӿ�
    };

    // ���� Socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket"); 
        return 1;  // ���� Socket ʧ�ܣ����ش���
    }

    // ���ӵ�������
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        return 1;  // ���ӷ�����ʧ�ܣ����ش���
    }

    // ��ȡ�û�������û���
    printf("Enter your username: ");
    fgets(username, sizeof(username), stdin);  // ��ȡ�û���
    username[strcspn(username, "\n")] = 0;  // ȥ������Ļ��з�

    // ���������ͳ�ʼ����Ϣ��������
    ChatMessage initmsg = {.type = MSG_TYPE_SYSTEM};  // ��ʼ����Ϣ����
    strncpy(initmsg.username, username, USERNAME_LEN);  // �����û���
    send(sock, &initmsg, sizeof(initmsg), 0);  // ������Ϣ��������

    // �����߳̽��շ�������Ϣ
    pthread_t recv_tid;
    pthread_create(&recv_tid, NULL, receive_thread, NULL);  // ����������Ϣ���߳�

    // ���ƿͻ����û�����
    draw_ui();

    // �˳�ʱȡ�������̲߳��ر�����
    pthread_cancel(recv_tid);  // ȡ�������߳�
    close(sock);  // �ر� Socket ����
    return 0;
}

