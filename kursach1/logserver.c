#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    system("rm myfifo"); // удаляется файл канала, если он был в каталоге
    FILE *file1; // переменные для логфайлов
    FILE *file2;
    if ((file1 = fopen("LogFile1.txt", "a")) == NULL)//попытка открыть файлы и их контроль ошибок
    {
        printf("smth went wrong");
        return 0;
    }
    if ((file2 = fopen("LogFile2.txt", "a")) == NULL)
    {
        printf("smth went wrong");
        return 0;
    }
    int lines_count1 = 0;//подсчёт количества строк в первом и во втором (почему-то не работает)
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

    umask(0); // начало работы с каналами
    int ret = mkfifo("myfifo", 0666); // создание файла фифо
    if (ret < 0)
    {
        perror("mkfifo"); // контроль ошибки при создании файла
        return 1;
    }

    int fd = open("myfifo", O_RDONLY); // открывается только для чтения

    struct Logs // создается структура, принимающая логи
    {
        char ans1[32];
        char ans2[512];
        char log[1024];
        char client[1024];
        int server;
    };
    struct Logs forLog;// инициализируется
    while (1) //вечно
    {
        int len = read(fd, &forLog, sizeof(struct Logs));//читается структура из файла фифо
        if (forLog.server == 1)// если одна из переменных в структуре равна 1, то
        {
            if (strlen(forLog.ans1) == 0) // если длина ответа = 0, то
            { // значит что-то в логах. 
                if(strcmp(forLog.log, "Connection")==0){
                    printf("Log1 %s\n", forLog.log); // просматриваем их и
                    fprintf(file1, "Log #%d: %s", lines_count1, forLog.log); // записываем в файл
                    lines_count1++; // увеличиваем счётчик количества строк.
                }
                else if(strcmp(forLog.log, "Disconnected")==0){
                    printf("Log1 %s\n", forLog.log); // просматриваем их и
                    fprintf(file1, "Log #%d: %s", lines_count1, forLog.log); // записываем в файл
                    lines_count1++; // увеличиваем счётчик количества строк.
                }
            }
            else{
                if(strcmp(forLog.client, "1")==0){
                    printf("ClientLog %s\n", forLog.client);
                    fprintf(file1, "ClientLog #%d: %s\n", lines_count1, forLog.client);
                    lines_count1++;
                    printf("Log1 %s\n", forLog.ans1);// все тоже самое, но с ответами
                    fprintf(file1, "Log #%d: %s\n", lines_count1, forLog.ans1);
                    lines_count1++;
                }
                else if(strcmp(forLog.client, "2")==0){
                    printf("ClientLog %s\n", forLog.client);
                    fprintf(file1, "ClientLog #%d: %s\n", lines_count1, forLog.client);
                    lines_count1++;
                    printf("Log1 %s\n", forLog.ans2);// все тоже самое, но с ответами
                    fprintf(file1, "Log #%d: %s\n", lines_count1, forLog.ans2);
                    lines_count1++;
                }
                else{
                    printf("ClientLog %s\n", forLog.client);
                    fprintf(file1, "ClientLog #%d: %s\n", lines_count1, forLog.client);
                    lines_count1++;
                    printf("Log1 %s\n", forLog.ans1);// все тоже самое, но с ответами
                    printf("Log1 %s\n", forLog.ans2);
                    fprintf(file1, "Log #%d: %s\n", lines_count1, forLog.ans1);
                    lines_count1++;
                    fprintf(file1, "Log #%d: %s\n", lines_count1, forLog.ans2);
                    lines_count1++;
                }
            }
        }
        if (forLog.server == 2) // если одна из переменных в структуре равна 2, то
       {
            if (strlen(forLog.ans1) == 0) // если длина ответа = 0, то
            { // значит что-то в логах. 
                if(strcmp(forLog.log, "Connection")==0){
                    printf("Log2 %s\n", forLog.log); // просматриваем их и
                    fprintf(file1, "Log #%d: %s", lines_count2, forLog.log); // записываем в файл
                    lines_count2++; // увеличиваем счётчик количества строк.
                }
                else if(strcmp(forLog.log, "Disconnected")==0){
                    printf("Log2 %s\n", forLog.log); // просматриваем их и
                    fprintf(file2, "Log #%d: %s", lines_count2, forLog.log); // записываем в файл
                    lines_count2++; // увеличиваем счётчик количества строк.
                }
            }
            else{
                if(strcmp(forLog.client, "1")==0){
                    printf("ClientLog %s\n", forLog.client);
                    fprintf(file2, "ClientLog #%d: %s\n", lines_count2, forLog.client);
                    lines_count2++;
                    printf("Log2 %s\n", forLog.ans1);// все тоже самое, но с ответами
                    fprintf(file2, "Log #%d: %s\n", lines_count2, forLog.ans1);
                    lines_count2++;
                }
                else if(strcmp(forLog.client, "2")==0){
                    printf("ClientLog %s\n", forLog.client);
                    fprintf(file2, "ClientLog #%d: %s\n", lines_count2, forLog.client);
                    printf("Log2 %s\n", forLog.ans2);// все тоже самое, но с ответами
                    fprintf(file2, "Log #%d: %s\n", lines_count2, forLog.ans2);
                    lines_count2++;
                }
                else{
                    printf("ClientLog %s\n", forLog.client);
                    fprintf(file2, "ClientLog #%d: %s\n", lines_count2, forLog.client);
                    lines_count2++;
                    printf("Log2 %s\n", forLog.ans1);// все тоже самое, но с ответами
                    printf("Log2 %s\n", forLog.ans2);
                    fprintf(file2, "Log #%d: %s\n", lines_count2, forLog.ans1);
                    lines_count2++;
                    fprintf(file2, "Log #%d: %s\n", lines_count2, forLog.ans2);
                    lines_count2++;
                }
            }
        }
        // контроли ошибок и выход из фифо
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
    // закрываются файлы и фифо
    fclose(file1);
    fclose(file2);
    close(fd);
    return 0;
}
