#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ipc.h>
#include <ctype.h>
#include<sys/shm.h>

#define buffer_size 60
int price[10];
int linesCount;

int isNum(char* num){
    for(int i = 0; i<strlen(num); i++){
        if(!isdigit(num[i]))
            return 0;
    }
    return 1;
}

void getprices(){
    FILE *fpt = fopen("menu.txt", "r");
    int j = 1;
    char menuOption[1000];
    while(fgets(menuOption, 100, fpt) != NULL){
        char* token = strtok(menuOption, " ");
        while(token != NULL){
            if(isNum(token)){
                price[j] = atoi(token);
            }
            token = strtok(NULL, " ");
        }
    j++;
    }
    linesCount = j;
    fclose(fpt);
}


int main(){
getprices();

//for table shared memory
int shmid; 
int *shmptr;
key_t key;
//for waiter shared memory
int shmid1; 
int *shmptr1;
key_t key1;

int wid;
printf("Enter Waiter ID:\n");
scanf("%d",&wid);
char o=(char)wid;
key = ftok("menu.txt", o);
key1 = ftok("earnings.txt", o);//waiter manager
int r=1;
shmid=shmget(key,buffer_size, 0644 |IPC_CREAT);
if(shmid==-1){
		perror("error in shmget()");
			exit(-1);
	}
shmid1=shmget(key1,buffer_size, 0644 |IPC_CREAT);
if(shmid1==-1){
		perror("error in shmget()");
			exit(-1);
	}
shmptr1 = (int *)shmat(shmid1, (void*)0, 0);
 if(shmptr1==(void*)-1)
	{
		perror("Error in shmPtr in attaching the memory segment\n");
		return 1;
	} 
shmptr =(int *) shmat(shmid, (void*)0,0); 	//attach to shared memory
		if(shmptr ==(void*)-1){
			perror("error in shmat");
			exit(-2);
		}
while(r){
while(shmptr[51]==0);

int sum=0;
for(int i=1;i<40;i++){
	sum=sum+price[shmptr[i]];
}
shmptr1[1]=sum;
shmptr[49]=sum;
shmptr[51]=0;

 printf("Bill Amount for Table X:%d\n",shmptr[49]);
 shmptr[49]=0;

while(shmptr[50]==0);

if(shmptr[50]==-1){
	shmptr[50]=0;
	shmdt(shmptr);
    break;
}
shmptr[50]=0;


}

	shmptr1[5]=1;
	sleep(1);
shmdt(shmptr1);
         // destroy the shared memory
shmctl(shmid1, IPC_RMID, NULL);
	



return 0;
}