#include "../shared/common.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <time.h>

// ��־��¼������
void start_logger(int pipe_fd[2]) {
    ChatMessage msg;
    FILE *fp;
    char filename[128];

    while (1) {
        // �ӹܵ��ж�ȡ��Ϣ
        if (read(pipe_fd[0], &msg, sizeof(msg)) <= 0) continue;

        // ��ȡ��ǰʱ�䲢������־�ļ���
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        strftime(filename, sizeof(filename), "server/chat_log/%Y-%m-%d.log", tm_info);

        // ����־�ļ�������
        fp = fopen(filename, "a");
        if (!fp) continue;
        flock(fileno(fp), LOCK_EX);

        // д����־��Ϣ
        fprintf(fp, "[%s] %s: %s\n", msg.timestamp, msg.username, msg.message);
        fflush(fp);  // ˢ���ļ�������

        // �������ر��ļ�
        flock(fileno(fp), LOCK_UN);
        fclose(fp);
    }
}

