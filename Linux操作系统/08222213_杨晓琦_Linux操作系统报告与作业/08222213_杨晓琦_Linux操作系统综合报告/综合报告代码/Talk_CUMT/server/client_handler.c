#include "../shared/common.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

extern int client_sockets[];  // �ͻ����׽�������
extern char client_usernames[][USERNAME_LEN];  // �ͻ����û�������
extern pthread_mutex_t clients_lock;  // ������
extern int logger_pipe[];  // ��־�ܵ�

// �㲥��Ϣ�����пͻ��ˣ��ų�ָ���Ŀͻ���
void broadcast_message(const ChatMessage *msg, int exclude_fd) {
    pthread_mutex_lock(&clients_lock);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] > 0 && client_sockets[i] != exclude_fd) {
            send(client_sockets[i], msg, sizeof(*msg), 0);
        }
    }
    pthread_mutex_unlock(&clients_lock);
}

// �㲥��ǰ�����û��б�
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
    
    broadcast_message(&msg, -1);  // �㲥�û��б�
}

// �������ͻ��˵����Ӻ���Ϣ
void *handle_client(void *arg) {
    int client_fd = *(int *)arg;
    ChatMessage msg;

    // �����û���
    if (recv(client_fd, &msg, sizeof(msg), 0) <= 0) {
        close(client_fd);
        return NULL;
    }
    
    // �洢�û���
    pthread_mutex_lock(&clients_lock);
    for (int i = 0; i < MAX_CLIENTS; i++)_

