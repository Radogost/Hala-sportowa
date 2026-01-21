#ifndef IPC_UTILS_H
#define IPC_UTILS_H

#include "common.h"

void check_error(int result, const char *msg);
void sem_init(int semid, int sem_num, int val);
void sem_lock(int semid, int sem_num);
void sem_unlock(int semid, int sem_num);

#endif