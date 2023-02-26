// message.h
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>

#define SHM_ID  0x776     // ключ разделяемой памяти
#define PERMS   0666      // права доступа
// структура сообщения, помещаемого в разделяемую память
typedef struct {
  int num;
  int type; //если type == 1, то клиент сгенерировал число и сервер может его выводить; если type == 2, то клиент сгенерировал все числа, сервер уже их вывел => можно завершать программу;
            //если type == 3, то клиенту можно генерировать новое число
} message_t;

