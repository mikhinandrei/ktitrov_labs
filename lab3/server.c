#include <stdio.h>
#include <string.h>
#include "message.h"

void sys_err (char *msg)
{
  puts (msg);
  exit (1);
}

int main ()
{
  int semid;                    
  int shmid;                    
  message_t *msg_p;          

  char s[MAX_STRING];

  if ((semid = semget (SEM_ID, 1, PERMS | IPC_CREAT)) < 0)
    sys_err ("server: can not create semaphore");

  if ((shmid = shmget (SHM_ID, sizeof (message_t), PERMS | IPC_CREAT)) < 0)
    sys_err ("server: can not create shared memory segment");

    if ((msg_p = (message_t *) shmat (shmid, 0, 0)) == NULL)
      sys_err ("server: shared memory attach error");

  while (1)
    {
      scanf ("%s", s);
      while (semctl (semid, 0, GETVAL, 0) || msg_p->type != MSG_TYPE_EMPTY);
      semctl (semid, 0, SETVAL, 1);
      if (strlen (s) != 1)
        {
          msg_p->type = MSG_TYPE_STRING;
          strncpy (msg_p->string, s, MAX_STRING);
        }
      else
        {
          msg_p->type = MSG_TYPE_FINISH;
          printf ("SHUTDOWN SERVER\n");
        };
      semctl (semid, 0, SETVAL, 0);
      if (strlen (s) == 1)
        break;
    }
  if (semctl (semid, 0, IPC_RMID, (struct semid_ds *) 0) < 0)
    sys_err ("server: semaphore remove error");

  shmdt (msg_p);
  if (shmctl (shmid, IPC_RMID, (struct shmid_ds *) 0) < 0)
    sys_err ("server: shared memory remove error");
  exit (0);
}
