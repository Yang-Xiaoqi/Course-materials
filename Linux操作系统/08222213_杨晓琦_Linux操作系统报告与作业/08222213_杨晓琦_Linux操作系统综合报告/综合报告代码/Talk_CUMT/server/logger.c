#include "../shared/common.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <time.h>

// 日志记录器函数
void start_logger(int pipe_fd[2]) {
    ChatMessage msg;
    FILE *fp;
    char filename[128];

    while (1) {
        // 从管道中读取消息
        if (read(pipe_fd[0], &msg, sizeof(msg)) <= 0) continue;

        // 获取当前时间并生成日志文件名
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        strftime(filename, sizeof(filename), "server/chat_log/%Y-%m-%d.log", tm_info);

        // 打开日志文件并锁定
        fp = fopen(filename, "a");
        if (!fp) continue;
        flock(fileno(fp), LOCK_EX);

        // 写入日志信息
        fprintf(fp, "[%s] %s: %s\n", msg.timestamp, msg.username, msg.message);
        fflush(fp);  // 刷新文件缓冲区

        // 解锁并关闭文件
        flock(fileno(fp), LOCK_UN);
        fclose(fp);
    }
}

