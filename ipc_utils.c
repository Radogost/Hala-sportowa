#include "ipc_utils.h"

// Funkcja obsługi błędów
void check_error(int result, const char *msg) {
    if (result == -1) {
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

// Inicjalizacja semafora
void sem_init(int semid, int sem_num, int val) {
    // macOS ma wbudowaną unię semun, nie definiujemy jej ręcznie
    union semun arg;
    arg.val = val;
    int res = semctl(semid, sem_num, SETVAL, arg);
    check_error(res, "Błąd sem_init");
}

// Opuszczenie (Lock)
void sem_lock(int semid, int sem_num) {
    struct sembuf operacja;
    operacja.sem_num = sem_num;
    operacja.sem_op = -1;
    operacja.sem_flg = 0;
    
    // Pętla wznawia czekanie, jeśli przerwał je sygnał (np. przyjście nowego kibica)
    while (semop(semid, &operacja, 1) == -1) {
        if (errno != EINTR) {
            check_error(-1, "Błąd sem_lock");
        }
    }
}

// Podniesienie (Unlock)
void sem_unlock(int semid, int sem_num) {
    struct sembuf operacja;
    operacja.sem_num = sem_num;
    operacja.sem_op = 1;
    operacja.sem_flg = 0;
    
    check_error(semop(semid, &operacja, 1), "Błąd sem_unlock");
}