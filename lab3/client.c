#include <stdio.h>
#include <string.h>
#include "message.h"

int main ()
{
  int semid;                    
  int shmid; 
  struct sembuf sops;
  sops.sem_num=0;
  sops.sem_op=0;
  message_t *msg_p;            
                                
  char s[MAX_STRING];

  if ((semid = semget (SEM_ID, 1, 0)) < 0)
    sys_err ("client: can not get semaphore");

  if ((shmid = shmget (SHM_ID, sizeof (message_t), 0)) < 0)
    sys_err ("client: can not get shared memory segment");

  if ((msg_p = (message_t *) shmat (shmid, 0, 0)) == NULL)
    sys_err ("client: shared memory attach error");

  semctl (semid, 0, SETVAL, 0); 
  msg_p->type = MSG_TYPE_EMPTY;

  while (1)
    {
      if(semctl(semid,0,GETVAL,1)) continue;
      if (msg_p->type != MSG_TYPE_EMPTY)
        {
          semctl (semid, 0, SETVAL, 1); 

          if (msg_p->type == MSG_TYPE_STRING)
            printf ("%s\n", msg_p->string);
          if (msg_p->type == MSG_TYPE_FINISH) {
            printf ("SERVER CLOSED!\n");
            break;
        }

          msg_p->type = MSG_TYPE_EMPTY;
          semctl (semid, 0, SETVAL, 0);
        }
    }

  shmdt (msg_p);    

  exit (0);
}
