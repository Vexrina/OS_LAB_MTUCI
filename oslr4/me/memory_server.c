#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>
int main()
{
	int shm_id;
	char* share[255];
	int order;
	srand(time(NULL));
	shm_id = shmget(1234, getpagesize(), IPC_CREAT);
	if (shm_id == -1)
	{
		perror("shmget()");
	}
	*share = (char*)shmat(shm_id, 0, 0);
	
	while (1)
	{
		order = random() % 7 + 2;
		int mtx[order][order];
		char string[255] = {0};

		strcpy(string, "[");
		for (int i = 0; i < order; i++)
		{
			for (int j = 0; j < order; j++)
			{
				mtx[i][j] = random() % 10 + 1;
				printf("%d  ", mtx[i][j]);
			}
			printf("\n");
		}
		strcpy(string, "[");
		for (size_t i = 0; i < order; i++)
		{
			strcat(string, "[");
			for (size_t j = 0; j < order; j++)
			{
				if (j == order - 1)
					sprintf(&string[strlen(string)], "%d", mtx[i][j]);
				else
					sprintf(&string[strlen(string)], "%d,", mtx[i][j]);
			}
			if (i == order - 1)
				strcat(string, "]");
			else
				strcat(string, "],");
		}
		strcat(string, "] ");
		sprintf(&string[strlen(string)], "%d", order);
		printf("%s\n", string);
		
		strcpy(*share, string);
		//printf("here2");
		//printf("TEXT %s\n",*share);
		sleep(5);
	}
	return 0;
}
