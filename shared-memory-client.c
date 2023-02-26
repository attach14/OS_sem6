#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "message.h"
#include <stdlib.h>
#include <time.h>
void sys_err (char *msg) {
  puts (msg);
  exit (1);
}

int main (int argc, char* argv[]) {
  srand(time(NULL));
  if (argc != 2) { //проверка на правильное число аргументов, единственный аргумент должен быть требуемое число генерируемых чисел
  	printf("wrong number of arguments: %d instead of 1\n", argc - 1);
  }
  int x = atoi(argv[1]);
  if (x<=0 || x >= 1000) { //проверка на корректное число генерируемых чисел
  	printf("wrong number of generations: it must more than 0 and less than 1000\n");
  }	
  int shmid;            // идентификатор разделяемой памяти
  message_t *msg_p;     // адрес сообщения в разделяемой памяти

  // получение доступа к сегменту разделяемой памяти
  if ((shmid = shmget (SHM_ID, sizeof (message_t), 0)) < 0) {
    sys_err ("client: can not get shared memory segment");
  }

  // получение адреса сегмента
  if ((msg_p = (message_t *) shmat (shmid, 0, 0)) == NULL) {
    sys_err ("client: shared memory attach error");
  }

  // Организация передаци сообщений
  while (x) {
    if (msg_p->type == 3) { //если сервер закончил вывод или мы ещё не генерировали чисел, то можно генерировать новое число
    x--;
    int num = random() % 1000;
    msg_p->num = num;
    msg_p->type = 1; // сообщаем серверу, что мы сгенерировали новое число
    }
  }
  //мы сгенерировали все числа, теперь надо дождаться, пока сервер выведет последнее число и завершить работу
  while(1) {
  if (msg_p->type == 3) { // сервер вывел последнее число
  msg_p->type = 2; // сообщаем ему, что надо завершать программу
  }
    break;
  }
  shmdt (msg_p);  // отсоединить сегмент разделяемой памяти
  exit (0);
}

