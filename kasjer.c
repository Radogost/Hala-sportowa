#include "common.h"
#include "ipc_utils.h"

int main() {
    int shm_id = shmget(SHM_KEY, sizeof(SharedState), 0666);
    int sem_id = semget(SEM_KEY, 0, 0666);
    SharedState *stan = (SharedState *)shmat(shm_id, NULL, 0);

    // Rejestracja
    sem_lock(sem_id, SEM_MUTEX_MAIN);
    stan->aktywne_kasy++;
    sem_unlock(sem_id, SEM_MUTEX_MAIN);

    printf("[KASJER %d] Gotowy do pracy.\n", getpid());

    while(1) {
        // Czekamy na klienta
        sleep(2);

        sem_lock(sem_id, SEM_MUTEX_MAIN);
        // Jeśli są ludzie w kolejce, obsłuż jednego
        if (stan->liczba_kibicow_w_kolejce > 0) {
            // Logika sprzedaży jest uproszczona - kibic sam kupuje w swoim kodzie,
            // ale kasjer tutaj "nadzoruje" lub symuluje czas obsługi.
            // W pełnej wersji komunikacja odbywałaby się przez semafory.
            // Tutaj tylko sygnalizujemy obecność.
        }
        sem_unlock(sem_id, SEM_MUTEX_MAIN);
    }
    return 0;
}