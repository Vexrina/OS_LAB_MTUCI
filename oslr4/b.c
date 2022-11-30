#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <error.h>
int main(){
    int shm_id;
    int *share;
    shm_id = shmget(1234,getpagesize(), IPC_CREAT);
    if(shm_id == -1){
        perror("shmget()")
    }
    share = (int *)shmat(shm_id, 0, 0);
    while(1){
        sleep(1);
        printf("%d\n", *share);
    }
    return 0;
}