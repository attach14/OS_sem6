// shared-memory-server.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "message.h"

void sys_err (char *msg) {
  puts (msg);
  exit (1);
}

int main () {
  int shmid;             // идентификатор разделяемой памяти
  message_t *msg_p;      // адрес сообщения в разделяемой памяти

  // создание сегмента разделяемой памяти
  if ((shmid = shmget (SHM_ID, sizeof (message_t), PERMS | IPC_CREAT)) < 0) {
    sys_err ("server: can not create shared memory segment");
  }
  printf("Shared memory %d created\n", SHM_ID);
  // подключение сегмента к адресному пространству процесса
  if ((msg_p = (message_t *) shmat (shmid, 0, 0)) == NULL) {
    sys_err ("server: shared memory attach error");
  }
  printf("Shared memory pointer = %p\n", msg_p);
  msg_p->type = 3; //сообщаем клиенту, что он может начинать генерацию чисел
  while (1) { 
    if (msg_p->type == 1) { //выводим новое число
    	printf("number is %d\n", msg_p->num);
    	msg_p->type = 3; //сообщаем клиенту, что вывели число
    } 
    if (msg_p->type == 2) {//завершаем цикл
        break;
    }
  }
  // удаление сегмента разделяемой памяти
  shmdt (msg_p);
  if (shmctl (shmid, IPC_RMID, (struct shmid_ds *) 0) < 0) {
    sys_err ("server: shared memory remove error");
  }
  exit (0);
}

