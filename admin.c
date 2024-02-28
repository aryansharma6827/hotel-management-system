#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define buffer_size 60

int main(){
    int key;
    int shmid; 
    int *shmptr;
    key = ftok("earnings.txt", 'A');
    shmid=shmget(key,buffer_size, 0644 |IPC_CREAT);
    if(shmid==-1){
		perror("error in shmget()");
			exit(-1);
	}
  shmptr =(int *) shmat(shmid, (void*)0,0); 	//attach to shared memory
	if(shmptr ==(void*)-1){
			perror("error in shmat");
			exit(-2);
	}
    char ch;
    while(1){
        printf("Do you want to close the hotel? Enter Y for Yes and N for No.\n");

        scanf("%c",&ch);
        if (ch=='y')
        break;
    
    }
    shmptr[0]=-1;

    shmdt(shmptr);
    return 0;
}