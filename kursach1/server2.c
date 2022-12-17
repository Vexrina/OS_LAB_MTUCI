// PID сервака getpid()
// перечень собственных потоков с указанием их приоритета
// Сервер из этого говна
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

#define PORT 5555

int main()
{
    int sockfd, ret;
    struct sockaddr_in serverAddr;

    int newSocket;
    struct sockaddr_in newAddr;

    socklen_t addr_size;

    char buffer[1024];
    pid_t childpid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("[-]Error in connection.\n");
        exit(1);
    }
    printf("[+]Server Socket is created.\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (ret < 0)
    {
        printf("[-]Error in binding.\n");
        exit(1);
    }
    printf("[+]Bind to port %d\n", PORT);

    if (listen(sockfd, 10) == 0)
    {
        printf("[+]Listening....\n");
    }
    else
    {
        printf("[-]Error in binding.\n");
    }

     // Additinal quest
    int fd = open("myfifo",O_WRONLY);
    if(fd<0){
        perror("open");
        return 1;
    }

    struct Logs {
        char ans[1024];
        int server;
    };
    struct Logs forLog;
    forLog.server = 2;

    while (1)
    {
        newSocket = accept(sockfd, (struct sockaddr *)&newAddr, &addr_size);
        if (newSocket < 0)
        {
            exit(1);
        }
        
        char connection [64] = "Connection accepted from ";
        strcat(connection, inet_ntoa(newAddr.sin_addr));
        strcat(connection, ":");
        sprintf(&connection[strlen(connection)],"%d\n",ntohs(newAddr.sin_port));
        printf("%s", connection);


        memcpy(forLog.ans, connection, strlen(connection));
        write(fd, &forLog, sizeof(struct Logs));
	    bzero(forLog.ans, sizeof(forLog.ans));

        if ((childpid = fork()) == 0)
        {
            close(sockfd);

            while (1)
            {
                recv(newSocket, buffer, 1024, 0);
                if (strcmp(buffer, ":exit") == 0)
                {
                    char disconnect [64] = "Disconnected from ";
                    strcat(disconnect, inet_ntoa(newAddr.sin_addr));
                    strcat(disconnect, ":");
                    sprintf(&disconnect[strlen(disconnect)],"%d\n",ntohs(newAddr.sin_port));
                    printf("%s", disconnect);                 
                    memcpy(forLog.ans, disconnect, strlen(disconnect));
                    write(fd, &forLog, sizeof(struct Logs));
                    bzero(forLog.ans, sizeof(forLog.ans));
                    break;
                }
                else
                {
                    int pid = getpid();
                    int tid = syscall(SYS_gettid);
                    errno = 0;
                    int prio = getpriority(PRIO_PROCESS, pid);
                    if (errno != 0)
                    {
                        printf("oops, smth went wrong");
                    }
                    char ans[1024] = {0};
                    snprintf(ans, 1024, "PID is %d, TID is %d, his priority is %d",
                             pid, tid, prio);
                    
                    sleep(1);
                    printf("Client: %s\n", buffer);
                    send(newSocket, ans, strlen(ans), 0);
                    printf("%s", ans);
                    memcpy(forLog.ans, ans, strlen(ans));
                    write(fd, &forLog, sizeof(struct Logs));
                    bzero(forLog.ans, sizeof(forLog.ans));
                    bzero(ans, sizeof(ans));
                }
                bzero(buffer, sizeof(buffer));
            }
        }
    }

    close(newSocket);

    return 0;
}
