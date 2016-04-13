#include<stdio.h>
#include<stdlib.h>
#include<sys/msg.h>
#include<string.h>
 
#define MYMSG_KEY  75
 
struct mymesg {
    long  mtype;      /* positive message type */
    char  mtext[512]; /* message data, of length nbytes */
};
 
int main()
{
    int msgid,msgsend;
    struct  mymesg   mymsgsend;
    msgid=msgget(MYMSG_KEY,IPC_CREAT|0644);
    printf("msgid=%d\n",msgid);
    if(msgid==-1)
    {
       perror("msgget error: ");
       exit(EXIT_FAILURE);
    }
 
    mymsgsend.mtype=1;
    while(1)
    {
       printf("enter a  type  and a msg use to send :\n");
       scanf("%ld",&mymsgsend.mtype);
       //printf("%d",mymsgsend.mtype);
getchar();
       scanf("%s",mymsgsend.mtext);
//getchar();
       printf("(Client) Send!\n");
       msgsend=msgsnd(msgid,&mymsgsend,strlen(mymsgsend.mtext)+1,0);
       if(msgsend==-1)
       {
           perror("msgget error: ");
           exit(EXIT_FAILURE);
       }
              if(strcmp(mymsgsend.mtext,"exit")==0)  break;
 
    }
    return 0;
}