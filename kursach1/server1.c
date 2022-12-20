// Текущее положение курсора - комплете
// Дескриптор раскладки клавиатуры по умолчанию. - комплете
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include <sys/stat.h>
#include <fcntl.h>

// порт для работы сервера
#define PORT 4444

// создание функций
void Cursor(char *s);
void KeyboardLayouts(char *s);

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
    printf("[+]Bind to port %d\n", 4444);

    if (listen(sockfd, 10) == 0)
    {
        printf("[+]Listening....\n"); // начинаем слушать клиентов
    }
    else
    {
        printf("[-]Error in binding.\n");
    }

    // Допка
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
    forLog.server = 1; // говорим структуре, а значит и клиенту, и логсерверу что мы - первый сервер.

    while (1) // вечно
    {
        newSocket = accept(sockfd, (struct sockaddr *)&newAddr, &addr_size);// принимаем клиента
        if (newSocket < 0)
        {
            exit(1);//если с клиентом возникли проблемы то... выключаемся...
        }
        char connection[64] = "Connection accepted from "; // делаем строку с указанием на то
        strcat(connection, inet_ntoa(newAddr.sin_addr)); // кто к нам все таки подключился
        strcat(connection, ":");
        sprintf(&connection[strlen(connection)], "%d\n", ntohs(newAddr.sin_port));
        printf("%s", connection); // принтим ее и

        memcpy(forLog.log, connection, strlen(connection));
        write(fd, &forLog, sizeof(struct Logs));// отправляем на лог сервер
        bzero(forLog.log, sizeof(forLog.log)); // очищаем переменную в структуре

        if ((childpid = fork()) == 0)//запускаем дочерний процесс
        {
            close(sockfd); // закрываем сокет

            while (1) // вечно для дочернего процесса
            {
                recv(newSocket, buffer, 1024, 0); // получаем данные от сервера
                if (strcmp(buffer, ":exit") == 0) // если встретили :exit, то
                {
                    char disconnect[64] = "Disconnected from "; // создаем строку, которая 
                    strcat(disconnect, inet_ntoa(newAddr.sin_addr)); // будет говорить нам о том
                    strcat(disconnect, ":"); // от кого мы отключились
                    sprintf(&disconnect[strlen(disconnect)], "%d\n", ntohs(newAddr.sin_port));
                    printf("%s", disconnect);//выводим ее и собственно, отправляем на логсервер
                    memcpy(forLog.log, disconnect, strlen(disconnect));
                    memcpy(forLog.client, buffer, strlen(buffer));
                    write(fd, &forLog, sizeof(struct Logs));
                    bzero(forLog.log, sizeof(forLog.log)); // очищаем переменную в структуре
                    bzero(forLog.client, sizeof(forLog.client));
                    break;
                }
                else if (strcmp(buffer, ">choice") == 0) // если встертили >choice
                {
                    printf("client switch servers"); // просто принтим, что клиент хочет свичнутся
                }
                else
                {
                    char cursor[20]; // создаем переменные для взятия системной информации
                    char keyboardlayout[2]; 
                    KeyboardLayout(keyboardlayout); // забираем текущую раскладку клавиатуры
                    //printf("%s\n", keyboardlayout); // выводим ее, можно закомментить - нужно было для дебага
                    Cursor(cursor); // забираем текущее расположение курсора 
                    memcpy(forLog.ans1, cursor, strlen(cursor)); // записываем в структуры
                    memcpy(forLog.ans2, keyboardlayout, strlen(keyboardlayout));
                    memcpy(forLog.client, buffer, strlen(buffer));
                    //printf("%s\n", forLog.ans2); // осталось от дебага
                    sleep(1);//усыпляем сервер на секунду
                    printf("Client: %s\n", buffer); // выводим то, что написал пользователь
                    send(newSocket, &forLog, sizeof(struct Logs), 0); // отправляем пользователю ответы
                    write(fd, &forLog, sizeof(struct Logs)); // записываем структуру в фифо
                    bzero(forLog.ans1, sizeof(forLog.ans1)); // очищаем переменные структуры
                    bzero(forLog.ans2, sizeof(forLog.ans2)); // очищаем переменные структуры
                    bzero(forLog.client, sizeof(forLog.client));
                }
                bzero(buffer, sizeof(buffer)); // Очищаем буффер ввода пользователя
            }
        }
    }

    close(newSocket); // как конечный факт пользования программой - выключаем сервер.

    return 0;
}

void Cursor(char *s)
{
    system("xdotool getmouselocation >> a.txt"); // получения данных о курсоре
    FILE *fp;
    fp = fopen("a.txt", "r");
    char string[20];
    if (!fp)
    {
        perror("open file");
    }
    fgets(string, 20, fp);//чтение данных о курсоре
    fclose(fp);
    system("rm a.txt");
    char newstring[20];
    int i;
    for (i = 0; (newstring[i] = string[i]) != 's'; i++)//фильтрация данных о курсоре
        ;
    newstring[i] = '\0';
    newstring[i - 1] = '\0';
    strcpy(s, newstring);//отдаем данные туда, откуда вызывали функцию
}
void KeyboardLayout(char *s)
{
    FILE *fp;
    system("setxkbmap -query | grep layout >> b.txt");//получение информации о раскладке клавиатуры
    fp = fopen("b.txt", "r");
    char string[20];
    if (!fp)
    {
        perror("open file");
    }
    fgets(string, 20, fp);
    system("rm b.txt");
    char newstring[5];
    newstring[0] = string[12];//фильтрация данных о клавиатуре
    newstring[1] = string[13];
    newstring[2] = '\0';
    strcpy(s, newstring);//отдаем данные туда, откуда вызывали функцию
}
