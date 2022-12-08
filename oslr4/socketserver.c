#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main()
{
    int server_sockfd = -1;
    int client_sockfd = -1;
    int client_len = 0;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    server_sockfd = socket(AF_INET, SOCK_STREAM,0);
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl (INADDR_ANY);
    server_addr.sin_port = htons (9736);

    bind(server_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    listen(server_sockfd,5);

    signal(SIGCHLD, SIG_IGN);

    while(1){
        char ch ='\0';
        client_len = sizeof(client_addr);
        printf("server is waiting\n");

        client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_addr, &client_len);

        if(fork()==0){

            read(client_sockfd, &ch,1);
            sleep(5);
            ch++;
            write(client_sockfd, &ch, 1);
            close(client_sockfd);
            exit(0);
        }
        else{
            close(client_sockfd);
        }
    }
    return 0;
}
