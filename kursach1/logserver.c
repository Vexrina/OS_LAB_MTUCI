#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    system("rm myfifo");
    FILE *file1;
    FILE *file2;
    if ((file1 = fopen("LogFile1.txt", "a")) == NULL)
    {
        printf("smth went wrong");
        return 0;
    }
    if ((file2 = fopen("LogFile2.txt", "a")) == NULL)
    {
        printf("smth went wrong");
        return 0;
    }
    int lines_count1 = 0;
    for (char c = getc(file1); c != EOF; c = getc(file1))
    {
        if (c == '\n')
        { // Increment count if this character is newline
            lines_count1 = lines_count1 + 1;
        }
    }

    int lines_count2 = 0;
    for (char c = getc(file2); c != EOF; c = getc(file2))
    {
        if (c == '\n')
        { // Increment count if this character is newline
            lines_count2 = lines_count2 + 1;
        }
    }

    umask(0);
    int ret = mkfifo("myfifo", 0666);
    if (ret < 0)
    {
        perror("mkfifo");
        return 1;
    }

    int fd = open("myfifo", O_RDONLY);

    struct Logs
    {
        char ans1[32];
        char ans2[512];
        char log[1024];
        char client[1024];
        int server;
    };
    struct Logs forLog;
    while (1)
    {
        int len = read(fd, &forLog, sizeof(struct Logs));
        if (forLog.server == 1)
        {
            if (strlen(forLog.ans1) == 0)
            {
                printf("Log1 %s\n", forLog.log);
                fprintf(file1, "Log #%d: %s", lines_count1, forLog.log);
                lines_count1++;
            }
            printf("Log1 %s\n", forLog.ans1);
            printf("Log1 %s\n", forLog.ans2);
            fprintf(file1, "Log #%d: %s\n", lines_count1, forLog.ans1);
            lines_count1++;
            fprintf(file1, "Log #%d: %s\n", lines_count1, forLog.ans2);
            lines_count1++;
        }
        if (forLog.server == 2)
        {
            if (strlen(forLog.ans1) == 0)
            {
                printf("Log2 %s\n", forLog.log);
                fprintf(file2, "Log #%d: %s", lines_count2, forLog.log);
                lines_count2++;
            }
            printf("Log2 %s\n", forLog.ans1);
            printf("Log2 %s\n", forLog.ans2);
            fprintf(file2, "Log #%d: %s\n", lines_count2, forLog.ans1);
            lines_count2++;
            fprintf(file2, "Log #%d: %s\n", lines_count2, forLog.ans2);
            lines_count2++;
        }

        if (len > 0)
        {
        }
        else if (len == 0)
        {
            printf("client quit!\n");
            break;
        }
        else
        {
            perror("read");
            return 1;
        }
    }
    fclose(file1);
    fclose(file2);
    close(fd);
    return 0;
}
