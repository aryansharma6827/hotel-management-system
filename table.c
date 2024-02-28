#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define buffer_size 60
#define READ_END 0
#define WRITE_END 1

int main(){
int tnum=0;
int cnum=0;
char final[buffer_size];
char writeMsg[buffer_size];
char readMsg[buffer_size];
pid_t p ;


//shared memory
key_t key;
// struct shmid_ds buf;

int shmid; 
int *shmptr;

printf("Enter Table Number:");
scanf("%d",&tnum);
char o =(char)tnum;
key = ftok("menu.txt", o);
int it=1;
while(it){
printf("Enter Number of Customers at Table (maximum no. of customers can be 5):");
scanf("%d",&cnum);

char order[cnum][10];
char *filename = "menu.txt";
FILE *fp = fopen(filename, "r");

if (fp == NULL)
    {
        printf("Error: could not open file %s", filename);
        
    }
const unsigned MAX_LENGTH = 256;
char buffer[MAX_LENGTH];

while (fgets(buffer, MAX_LENGTH, fp))
    printf("%s", buffer);
fclose(fp);



int fd[cnum][2];
for(int i=1;i<=cnum;i++){
pipe(fd[i]);

p=fork();
if(p<0){
    printf("error");
}

else if(p==0){
    //child process
    
    
    printf("Enter the serial number(s) of the item(s) to order from the menu. Enter -1 when done:\n");
    int n=0;
    scanf("%d",&n);
    int j=0;
    char ch[2];
    char ord[25];
    while(n!=-1){
        if(n>4||n<-1||n==0){
            printf("wrong input\n");
            continue;
        }
        sprintf(ch, "%d", n);
        ord[j]=ch[0];
        j++;
        scanf("%d",&n);
    }
    ord[j]='0';
    j++;
    close(fd[i][READ_END]);
    write(fd[i][WRITE_END],ord,j);
    close(fd[i][WRITE_END]);
     

    exit(0);
}

else {
    //parent process
    close(fd[i][WRITE_END]);
    read(fd[i][READ_END],readMsg,buffer_size);
    // printf("%s\n",readMsg);
    strcpy(order[i],readMsg);
    close(fd[i][READ_END]);
    
    
}

}
for(int i=1;i<=cnum;i++){
    wait(NULL);
}
 if(p>0){
 
 shmid = shmget(key, buffer_size, 0644 | IPC_CREAT);
 if(shmid==-1)
	{
		perror("Error in shmget in creating/ accessing shared memory\n");
		return 1;
	}
 shmptr = (int *)shmat(shmid, (void*)0, 0);
 if(shmptr==(void*)-1)
	{
		perror("Error in shmPtr in attaching the memory segment\n");
		return 1;
	} 
    
    char cha='0';
    int size=1;
    for(int i=1;i<=cnum;i++){
    int k=0;
    while(order[i][k]!=cha){

    shmptr[size]=order[i][k]-'0';
    size++;
        k++;
    }
//   printf("%s\n",order[i]);
 }
    shmptr[51]=1;
    while(shmptr[49]==0);
    
    printf("Bill Amount for Table X:%d\n",shmptr[49]);
    shmptr[49]=0;
    int g=0;
    printf("press -1 to exit waiter 1 to not\n");
    scanf("%d",&g);
    int yo =g;
    shmptr[50]=g;
    
    if(yo==-1){
     shmdt(shmptr);
         // destroy the shared memory
    shmctl(shmid, IPC_RMID, NULL);
    break;
    }
 

 }
 
}
return 0;
}
