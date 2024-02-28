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
int totalamt=0;
key_t key;
key_t key1;
int shmid; 
int shmid1; 
int *shmptr1;
int n;
int r=1;
while(r){
printf("Enter the Total Number of Tables at the Hotel:\n");
scanf("%d",&n);
int *shmptr[n+1];
key1 = ftok("earnings.txt", 'A');
shmid1=shmget(key1,buffer_size, 0644 |IPC_CREAT);
    if(shmid1==-1){
		perror("error in shmget()");
			exit(-1);
	}
  shmptr1 =(int *) shmat(shmid1, (void*)0,0); 	//attach to shared memory
	if(shmptr ==(void*)-1){
			perror("error in shmat");
			exit(-2);
	}
if(n==0&&shmptr1[0]==-1){
break;
}
for(int i=1;i<=n;i++){
    char o=(char)i;
    key = ftok("earnings.txt", o);
    shmid=shmget(key,buffer_size, 0644 |IPC_CREAT);
    if(shmid==-1){
		perror("error in shmget()");
			exit(-1);
	}
    shmptr[i] =(int *) shmat(shmid, (void*)0,0); 	//attach to shared memory
	if(shmptr[i] ==(void*)-1){
			perror("error in shmat");
			exit(-2);
	}
  
}
int ter=0;
int er=1;
int ad=0;
while(er){
for(int i=1;i<=n;i++){
  if(shmptr[i][1]!=0){
  FILE *fptr;
  fptr = fopen("earnings.txt", "a");
  fprintf(fptr, "Earning from Table %d:%d\n",i,shmptr[i][1]);
  fclose(fptr);
  totalamt+=shmptr[i][1];
  shmptr[i][1]=0;
}
}

for(int j=1;j<=n;j++){
  if(shmptr[j][5]==1){
    ad+=1;
  }
}
if(ad==n){
  ter=1;
break;
}
}


for(int i=0;i<=n;i++){
    shmdt(shmptr[i]);
}
if(ter==1&&shmptr1[0]==-1)
break;
}

shmdt(shmptr1);
         // destroy the shared memory
shmctl(shmid1, IPC_RMID, NULL);

  FILE *fptr;
  fptr = fopen("earnings.txt", "a");
  fprintf(fptr, "Total Earnings of Hotel:  %d\n",totalamt);
  int twages=(40*totalamt)/100;
  fprintf(fptr, "Total Wages of Waiters:  %d\n",twages);
  int totalp=totalamt-twages;
  fprintf(fptr, "Total Profit:  %d\n",totalp);
  fclose(fptr);
  printf("Thank you for visiting the Hotel!\n");
return 0;
}

