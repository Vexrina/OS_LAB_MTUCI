#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT1 4444
#define PORT2 5555

int findChar(char str[], char s){
	for (int i = 0; i<strlen(str); i++)
		if(str[i]==s)
			return i;
	return -1;
}

int main()
{

	int clientSocket1, clientSocket2, ret1, ret2; // Настройка клиента
	clientSocket1 = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket1 < 0)
	{
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket for server1 is created.\n");

	clientSocket2 = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket2 < 0)
	{
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket for server2 is created.\n");

	struct sockaddr_in serverAddr1; // настройка первого сервера.
	memset(&serverAddr1, '\0', sizeof(serverAddr1));
	serverAddr1.sin_family = AF_INET;
	serverAddr1.sin_port = htons(PORT1);
	serverAddr1.sin_addr.s_addr = inet_addr("127.0.0.1");

	struct sockaddr_in serverAddr2; // настройка первого сервера.
	memset(&serverAddr2, '\0', sizeof(serverAddr2));
	serverAddr2.sin_family = AF_INET;
	serverAddr2.sin_port = htons(PORT2);
	serverAddr2.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret1 = connect(clientSocket1, (struct sockaddr *)&serverAddr1, sizeof(serverAddr1));
	if (ret1 < 0)
	{
		printf("[-]Error in connection to server1.\n");
		exit(1);
	}
	else
		printf("[+]Connected to Server1.\n");

	ret2 = connect(clientSocket2, (struct sockaddr *)&serverAddr2, sizeof(serverAddr2));
	if (ret2 < 0)
	{
		printf("[-]Error in connection to server2.\n");
		exit(1);
	}
	else
		printf("[+]Connected to Server2.\n");

	char buffer[1024];
	while (1)
	{
		printf("Client: \t");
		scanf("%s", &buffer[0]);
		send(clientSocket1, buffer, strlen(buffer), 0);
		send(clientSocket2, buffer, strlen(buffer), 0);

		if (strcmp(buffer, ":exit") == 0)
		{
			close(clientSocket1);
			close(clientSocket2);
			printf("[-]Disconnected from servers.\n");
			exit(1);
		}

		char ans1[19];
		recv(clientSocket1, ans1, 19, 0);
		int U, S;
		U = findChar(ans1, 'u');
		S = findChar(ans1, 's');
		if (S > U)
		{
			while (strlen(ans1) - 1 > S)
			{
				ans1[strlen(ans1) - 1] = '\0';
			}
		}
		else if (U > S)
		{
			while (strlen(ans1) - 1 > U)
			{
				ans1[strlen(ans1) - 1] = '\0';
			}
		}
		char ans2[512];
		recv(clientSocket2, ans2, 512, 0);

		if (ans1 < 0 || ans2 < 0)
		{
			printf("[-]Error in receiving data.\n");
		}
		else
		{
			printf("Server1: \t%s\n", ans1);
			printf("Server2: \t%s\n", ans2);
		}
		bzero(buffer, sizeof(buffer));
	}

	return 0;
}
