// Текущее положение курсора - комплете
// Дескриптор раскладки клавиатуры по умолчанию. - комплете
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

#define PORT 4444

void Cursor(char *s);
void KeyboardLayouts(char *s);
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
    printf("[+]Bind to port %d\n", 4444);

    if (listen(sockfd, 10) == 0)
    {
        printf("[+]Listening....\n");
    }
    else
    {
        printf("[-]Error in binding.\n");
    }
    while (1)
    {
        newSocket = accept(sockfd, (struct sockaddr *)&newAddr, &addr_size);
        if (newSocket < 0)
        {
            exit(1);
        }
        printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

        if ((childpid = fork()) == 0)
        {
            close(sockfd);

            while (1)
            {
                recv(newSocket, buffer, 1024, 0);
                if (strcmp(buffer, ":exit") == 0)
                {
                    printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
                    break;
                }
                else
                {
                    char cursor[20];
                    char keyboardlayout[5];
                    KeyboardLayout(keyboardlayout);
                    Cursor(cursor);
                    //printf("%s\n", cursor);
                    //printf("%s\n", keyboardlayout);
                    char ans[30] = {0};
                    strcat(ans, cursor);
                    strcat(ans, "___");
                    strcat(ans, keyboardlayout);
                    sleep(1);
                    printf("Client: %s\n", buffer);
	  	    //printf(*ans);
		    //printf(strlen(ans));
                    send(newSocket, ans, strlen(ans), 0);
                    bzero(ans, sizeof(ans));
                }
            }
        }
    }

    close(newSocket);

    return 0;
}

void Cursor(char *s)
{
    system("xdotool getmouselocation >> a.txt");
    FILE *fp;
    fp = fopen("a.txt", "r");
    char string[20];
    if (!fp)
    {
        perror("open file");
    }
    fgets(string, 20, fp);
    fclose(fp);
    system("rm a.txt");
    char newstring[20];
    int i;
    for (i = 0; (newstring[i] = string[i]) != 's'; i++)
        ;
    newstring[i] = '\0';
    newstring[i - 1] = '\0';
    strcpy(s, newstring);
}
void KeyboardLayout(char *s)
{
    FILE *fp;
    system("setxkbmap -query | grep layout >> b.txt");
    fp = fopen("b.txt", "r");
    char string[20];
    if (!fp)
    {
        perror("open file");
    }
    fgets(string, 20, fp);
    system("rm b.txt");
    char newstring[5];
    newstring[0] = string[12];
    newstring[1] = string[13];
    newstring[2] = '\0';
    strcpy(s, newstring);
}