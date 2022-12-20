// PID сервака getpid()
// перечень собственных потоков с указанием их приоритета
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/resource.h>
#include <sys/syscall.h>

#include <sys/stat.h>
#include <fcntl.h>

// порт для работы сервера
#define PORT 5555

int main()
{
    //настройка сервера и клиента
    int sockfd, ret;
    struct sockaddr_in serverAddr;

    int newSocket;
    struct sockaddr_in newAddr;

    socklen_t addr_size;

    char buffer[1024];
    pid_t childpid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // создание сервера
    if (sockfd < 0)
    {
        printf("[-]Error in connection.\n"); // контроль ошибки подключения к сокету
        exit(1);
    }
    printf("[+]Server Socket is created.\n");

    memset(&serverAddr, '\0', sizeof(serverAddr)); // настраивается сервер
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)); // пытаемся его запустить
    if (ret < 0)
    {
        printf("[-]Error in binding.\n"); // контроль ошибки запуска
        exit(1);
    }
    printf("[+]Bind to port %d\n", PORT);

    if (listen(sockfd, 10) == 0)
    {
        printf("[+]Listening....\n"); // начинаем слушать клиентов
    }
    else
    {
        printf("[-]Error in binding.\n");
    }

    // Additinal quest
    int fd = open("myfifo", O_WRONLY); // открываем канал для записи
    if (fd < 0)
    {
        perror("open"); // контроль ошибки для допки
        return 1;
    }

    // создания структуры для посылки ответов клиенту и логов... серверу с логами
    struct Logs
    {
        char ans1[32];
        char ans2[512];
        char log[1024];
        char client[1024];
        int server;
    };
    struct Logs forLog; // инициализируется структура
    forLog.server = 2; // говорим структуре, а значит и клиенту, и логсерверу что мы - второй сервер.

    while (1)
    {
        // блок до  131 строки такой же, как и на первом сервере
        newSocket = accept(sockfd, (struct sockaddr *)&newAddr, &addr_size);
        if (newSocket < 0)
        {
            exit(1);
        }

        char connection[64] = "Connection accepted from ";
        strcat(connection, inet_ntoa(newAddr.sin_addr));
        strcat(connection, ":");
        sprintf(&connection[strlen(connection)], "%d\n", ntohs(newAddr.sin_port));
        printf("%s", connection);

        memcpy(forLog.log, connection, strlen(connection));
        write(fd, &forLog, sizeof(struct Logs));
        bzero(forLog.log, sizeof(forLog.log));

        if ((childpid = fork()) == 0)
        {
            close(sockfd);

            while (1)
            {
                recv(newSocket, buffer, 1024, 0);
                memcpy(forLog.client, buffer, 1024);
                if (strcmp(buffer, ":exit") == 0)
                {
                    char disconnect[64] = "Disconnected from ";
                    strcat(disconnect, inet_ntoa(newAddr.sin_addr));
                    strcat(disconnect, ":");
                    sprintf(&disconnect[strlen(disconnect)], "%d\n", ntohs(newAddr.sin_port));
                    printf("%s", disconnect);
                    memcpy(forLog.client, buffer, strlen(buffer));
                    memcpy(forLog.log, disconnect, strlen(disconnect));
                    write(fd, &forLog, sizeof(struct Logs));
                    bzero(forLog.log, sizeof(forLog.log));
                    bzero(forLog.client, sizeof(forLog.client));
                    break;
                }
                else if (strcmp(buffer, ">choice") == 0)
                {
                    printf("client switch servers\n");
                }
                else
                {
                    int pid = getpid(); // получение ID процесса
                    int tid = syscall(SYS_gettid); // получение ID потока
                    errno = 0;
                    int prio = getpriority(PRIO_PROCESS, pid); // Получение приоритета потока
                    if (errno != 0)
                    {
                        printf("oops, smth went wrong"); // контроль ошибки
                    }
                    char ans1[32] = {0}; // создание строк под ответ
                    snprintf(ans1, 32, "PID is %d", pid); // запись в первую
                    char ans2[512] = {0};
                    snprintf(ans2, 512, "TID is %d, his priority is %d", tid, prio);//запись во вторую

                    sleep(1); // усыпляем на секунду сервер
                    memcpy(forLog.ans1, ans1, strlen(ans1)); // запись в структуры для логов и сервера
                    memcpy(forLog.ans2, ans2, strlen(ans2));
                    memcpy(forLog.client, buffer, strlen(buffer));
                    printf("Client: %s\n", buffer); // чтоже нам написал клиент
                    send(newSocket, &forLog, sizeof(struct Logs), 0); // отправляем клиенту результат
                    // printf("%s", ans);
                    write(fd, &forLog, sizeof(struct Logs)); // пишем в ФИФО файл
                    bzero(forLog.ans1, sizeof(forLog.ans1)); // очищаем структуру
                    bzero(forLog.ans2, sizeof(forLog.ans2));
                    bzero(forLog.client, sizeof(forLog.client));
                }
                bzero(buffer, sizeof(buffer)); // очищаем ввод пользователя
            }
        }
    }

    close(newSocket); // закрываем сокет

    return 0;
}
