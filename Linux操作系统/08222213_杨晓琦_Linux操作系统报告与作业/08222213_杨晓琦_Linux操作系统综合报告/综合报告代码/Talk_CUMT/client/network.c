#include "../shared/common.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// �ⲿ��������
extern int sock;  // ���ӷ������� Socket ������
extern void update_chat(const ChatMessage *);  // �����������ݵĺ���

// �����û�����������û�����
UserInfo online_users[MAX_CLIENTS];
int online_user_count = 0;

// ������Ϣ���̺߳���
void *receive_thread(void *arg) {
    ChatMessage msg;  // �洢���յ�����Ϣ
    while (1) {
        // ������Ϣ������ֱ���յ�����
        ssize_t len = recv(sock, &msg, sizeof(msg), 0);
        
        // ������յ����ݳ���С�ڵ��� 0�����˳�ѭ�������ӶϿ������
        if (len <= 0) break;
        
        // ����յ������û��б���Ϣ������Ϊ MSG_TYPE_USERLIST��
        if (msg.type == MSG_TYPE_USERLIST) {
            // ���������û��б�ȷ�����ᳬ���������
            online_user_count = (msg.user_count < MAX_CLIENTS) ? msg.user_count : MAX_CLIENTS;
            // �����û��б������û�����
            memcpy(online_users, msg.userlist, sizeof(UserInfo) * online_user_count);
        }

        // ����������棬��ʾ�յ�����Ϣ
        update_chat(&msg);
    }
    return NULL;  // �߳��˳�
}

