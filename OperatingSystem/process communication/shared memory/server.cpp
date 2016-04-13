#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys pes.h>
#include <unistd.h>

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
    shm_id = shmget(MYMSG_KEY,0, 0);   
    if(shm_id == -1)
    {
        perror("shmget error");
        return -1;
    }
    m = (message*)shmat(shm_id,NULL,0);
    //printf("%d\n",m->sign);
    //printf("%s\n",m->value);
    while(1)
    {
       if(strcmp(m->value,"exit")==0)  break;
       if(m->sign==0){
       	  printf("%s\n",m->value);
       	  m->sign=-1;
       	  printf("(Server) Received!\n");
}
    }
    if(shmdt(m) == -1)
    {
        perror("detach error");
        return -1;
    }
    if(shmctl(shm_id,IPC_RMID,NULL) < 0)
{
        perror("RM error");
        return -1;
    }
    return 0 ;
}