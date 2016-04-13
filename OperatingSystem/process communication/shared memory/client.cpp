#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <syspes.h>
#include <unistd.h>
#include <string.h>

typedef struct{
    int sign;
    char value[8];
} message;

#define MYMSG_KEY  75
int main()
{
    int shm_id;
    message msg;
    message *m=&msg;
   
    shm_id=shmget(MYMSG_KEY,4096,IPC_CREAT|IPC_EXCL|0600); 
    if(shm_id==-1)
    {
        perror("shmget error");
        return -1;
    }
    printf("shm_id=%d\n", shm_id) ;
    m=(message*)shmat(shm_id,NULL,0);
//printf("%d\n",m->sign); 
     m->sign=-1;
     while(1)
    {
       
       //getchar();
       if(-1==(m->sign)){
 // printf("GGGGGGGGGGG");
  printf("enter a msg use to send :\n");
       	  scanf("%s",m->value);
       	  m->sign=0;
       	  printf("(Client) Send!\n");
}
       if(strcmp(m->value,"exit")==0)  break; 
    } 
    shmdt(m) ;
    return 0 ;
}