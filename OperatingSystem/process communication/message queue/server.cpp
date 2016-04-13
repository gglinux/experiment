#include<stdio.h>
#include<stdlib.h>
#include<sys/msg.h>
#define MYMSG_KEY 75
 
struct mymesg {
    long  mtype;      /* positive message type */
    char  mtext[512]; /* message data, of length nbytes */
};
 
 
int main()
{
    int msgid,msgrcv_ret,msgctl_ret;
    struct mymesg  mymsgrece;
    msgid=msgget(MYMSG_KEY ,IPC_CREAT|0644);
    printf("msgid=%d\n",msgid);
    if(msgid==-1)
    {
       perror("msgget error:");
       exit(EXIT_FAILURE);
    }
    while(1)
    {
       scanf("%ld",&mymsgrece.mtype);
       msgrcv_ret=msgrcv(msgid,&mymsgrece,512,mymsgrece.mtype,0);
       if(msgrcv_ret==-1)
       {
           perror("msgrcv  error:");
           exit(EXIT_FAILURE);
       }
               if(strcmp(mymsgrece.mtext,"exit")==0)  break;
          printf("received msg : %s\n",mymsgrece.mtext);
  printf("(Server) Received!\n");
    }
    msgctl_ret=msgctl(msgid,IPC_RMID,0);
    if(msgctl_ret==-1)
    {
       perror("msgrcv  error:");
       exit(EXIT_FAILURE);
    }
    return 0;
}