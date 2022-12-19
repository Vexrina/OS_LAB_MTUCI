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

int server1(int clientSocket1, int clientSocket2, char *buffer);
int server2(int clientSocket1, int clientSocket2, char *buffer);
int bothServers(int clientSocket1, int clientSocket2, char *buffer);

struct answers
{
	char ans1[32];
	char ans2[512];
	char log[1024];
	char client[1024];
	int server;
};
struct answers answers;

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

	// настройка первого сервера.
	struct sockaddr_in serverAddr1;
	memset(&serverAddr1, '\0', sizeof(serverAddr1));
	serverAddr1.sin_family = AF_INET;
	serverAddr1.sin_port = htons(PORT1);
	serverAddr1.sin_addr.s_addr = inet_addr("127.0.0.1");

	// настройка второго сервера.
	struct sockaddr_in serverAddr2;
	memset(&serverAddr2, '\0', sizeof(serverAddr2));
	serverAddr2.sin_family = AF_INET;
	serverAddr2.sin_port = htons(PORT2);
	serverAddr2.sin_addr.s_addr = inet_addr("127.0.0.1");

	// попытка подключится к первому серверу
	ret1 = connect(clientSocket1, (struct sockaddr *)&serverAddr1, sizeof(serverAddr1));
	if (ret1 < 0)
	{
		printf("[-]Error in connection to server1.\n");
		exit(1);
	}
	else
		printf("[+]Connected to Server1.\n");

	// попытка подключится ко второму серверу
	ret2 = connect(clientSocket2, (struct sockaddr *)&serverAddr2, sizeof(serverAddr2));
	if (ret2 < 0)
	{
		printf("[-]Error in connection to server2.\n");
		exit(1);
	}
	else
		printf("[+]Connected to Server2.\n");

	// инициализация переменных для
	char buffer[1024]; // ввода client message
	int choice;		   // ввода выбора сервера
	// принт как работать в клиенте
	printf("What server do you want to connect to?\n1 - first, 2 - second, 3 - both.\nFor exit - choice server and print ':exit'.\nFor rechoice server - choice server and print '>choice':");
	// Забирается ввод "выбора сервера"
	scanf("%d", &choice);

	// начинается магия
	while (choice > 0 && choice < 4)
	{
		int flag = 1;
		switch (choice)
		{
		case 1:
			while (flag == 1)
				flag = server1(clientSocket1, clientSocket2, buffer); // Работа ТОЛЬКО с первым сервером
			break;
		case 2:
			while (flag == 1)
				flag = server2(clientSocket1, clientSocket2, buffer); // Работа ТОЛЬКО со вторым сервером
			break;
		case 3:
			while (flag == 1)
				flag = bothServers(clientSocket1, clientSocket2, buffer); // работа с двумя серверами
			break;
		default:
			printf("What server do you want to connect to?\n1 - first, 2 - second, 3 - both.\nFor exit - choice server and print ':exit'.\nFor rechoice server - choice server and print '>choice':");
			scanf("%d", &choice);
			break;
		}
		printf("What server do you want to connect to?\n1 - first, 2 - second, 3 - both.\nFor exit - choice server and print ':exit'.\nFor rechoice server - choice server and print '>choice':");
		scanf("%d", &choice);
	}
	return 0;
}

int server1(int clientSocket1, int clientSocket2, char *buffer)
{
	// принтится ввод клиента
	printf("Client: \t");
	scanf("%s", &buffer[0]);						// забирается запрос клиента
	send(clientSocket1, buffer, strlen(buffer), 0); // отправляется на сервер
	if (strcmp(buffer, ">choice") == 0)				// если есть >choice в начале, то переход к выбору сервера
	{
		printf("[-]Go to the choice from server1.\n");
		return -1;
	}
	if (strcmp(buffer, ":exit") == 0) // если есть :exit в начале, то выход из приложения
	{
		close(clientSocket1);
		close(clientSocket2);
		printf("[-]Disconnected from servers.\n");
		exit(1);
	}

	recv(clientSocket1, &answers, sizeof(struct answers), 0); // в нее записываются данные от сервера
	// обрезка полученной инфы от сервера
	// обрезка полученной инфы от сервера
	if (answers.ans1 < 0)
	{
		printf("[-]Error in receiving data.\n"); // контроль ошибки сервера
	}
	else
	{
		if (strcmp(buffer, "1") == 0)
		{
			printf("Server1: \t%s\n", answers.ans1); // вывод сервера
		}
		else if (strcmp(buffer, "2") == 0)
		{
			printf("Server1: \t%s\n", answers.ans2); // вывод сервера
		}
		else
		{
			printf("Server1: \t%s\n", answers.ans1);
			printf("Server1: \t%s\n", answers.ans2);
		}
	}
	bzero(buffer, sizeof(buffer)); // очищение буффера
	return 1;					   // продолжение работы с сервером
}

int server2(int clientSocket1, int clientSocket2, char *buffer)
{
	// принтится ввод клиента
	printf("Client: \t");
	scanf("%s", &buffer[0]);						// забирается запрос клиента
	send(clientSocket2, buffer, strlen(buffer), 0); // отправляется на сервер

	if (strcmp(buffer, ">choice") == 0) // если есть >choice в начале, то переход к выбору сервера
	{
		printf("[-]Go to the choice from server2.\n");
		return -1;
	}
	if (strcmp(buffer, ":exit") == 0) // если есть :exit в начале, то выход из приложения
	{
		close(clientSocket1);
		close(clientSocket2);
		printf("[-]Disconnected from servers.\n");
		exit(1);
	}

	recv(clientSocket2, &answers, sizeof(struct answers), 0); // получение данных

	if (answers.ans1 < 0)
	{
		printf("[-]Error in receiving data.\n"); // контроль ошибки
	}
	else
	{
		if (strcmp(buffer, "1") == 0)
		{
			printf("Server2: \t%s\n", answers.ans1); // вывод сервера
		}
		else if (strcmp(buffer, "2") == 0)
		{
			printf("Server2: \t%s\n", answers.ans2); // вывод сервера
		}
		else
		{
			printf("Server2: \t%s\n", answers.ans1);
			printf("Server2: \t%s\n", answers.ans2);
		}
	}
	bzero(buffer, sizeof(buffer)); // очистка буффера.
	return 1;
}

int bothServers(int clientSocket1, int clientSocket2, char *buffer)
{
	printf("Client: \t");
	scanf("%s", &buffer[0]);
	send(clientSocket1, buffer, strlen(buffer), 0);
	send(clientSocket2, buffer, strlen(buffer), 0);

	if (strcmp(buffer, ">choice") == 0)
	{
		printf("[-]Go to the choice from servers.\n");
		return -1;
	}

	if (strcmp(buffer, ":exit") == 0)
	{
		close(clientSocket1);
		close(clientSocket2);
		printf("[-]Disconnected from servers.\n");
		exit(1);
	}

	recv(clientSocket1, &answers, sizeof(struct answers), 0); // в нее записываются данные от сервера
	if (answers.ans1 < 0)
	{
		printf("[-]Error in receiving data.\n"); // контроль ошибки сервера
	}
	else
	{
		if (strcmp(buffer, "1") == 0)
		{
			printf("Server1: \t%s\n", answers.ans1); // вывод сервера
		}
		else if (strcmp(buffer, "2") == 0)
		{
			printf("Server1: \t%s\n", answers.ans2); // вывод сервера
		}
		else
		{
			printf("Server1: \t%s\n", answers.ans1);
			printf("Server1: \t%s\n", answers.ans2);
		}
	}
	recv(clientSocket2, &answers, sizeof(struct answers), 0); // получение данных

	if (answers.ans1 < 0)
	{
		printf("[-]Error in receiving data.\n"); // контроль ошибки
	}
	else
	{
		if (strcmp(buffer, "1") == 0)
		{
			printf("Server2: \t%s\n", answers.ans1); // вывод сервера
		}
		else if (strcmp(buffer, "2") == 0)
		{
			printf("Server2: \t%s\n", answers.ans2); // вывод сервера
		}
		else
		{
			printf("Server2: \t%s\n", answers.ans1);
			printf("Server2: \t%s\n", answers.ans2);
		}
	}

	bzero(buffer, sizeof(buffer));
	return 1;
}
