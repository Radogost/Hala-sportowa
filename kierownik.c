#include "common.h"
#include "ipc_utils.h"
#include <sys/wait.h>

int shm_id = -1;
int sem_id = -1;
SharedState *stan = NULL;

// Sprzątanie procesów zombie
void handle_zombie(int sig) {
    (void)sig;
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

// Sprzątanie zasobów IPC przy wyjściu
void clean_exit(int sig) {
    printf("\n[KIEROWNIK] Otrzymano sygnał %d. Sprzątanie...\n", sig);

    // Jeśli to błąd krytyczny (SegFault), wypisz komunikat
    if (sig == SIGSEGV) {
        printf("!!! BŁĄD KRYTYCZNY (Segmentation Fault) !!!\n");
    }

    // Zabijamy wszystkie procesy potomne (kasjerów, kibiców)
    kill(0, SIGTERM);
    
    // Usuwamy pamięć
    if (shm_id != -1) {
        shmctl(shm_id, IPC_RMID, NULL);
        printf("-> Pamięć usunięta.\n");
    }

    // Usuwamy semafory
    if (sem_id != -1) {
        semctl(sem_id, 0, IPC_RMID);
        printf("-> Semafory usunięte.\n");
    }

    exit(0);
}

int main() {
    // 1. Obsługa normalnego wyjścia (Ctrl+C)
    signal(SIGINT, clean_exit); 
    
    // 2. Obsługa standardowego "kill" (np. z terminala)
    signal(SIGTERM, clean_exit);

    // 3. Obsługa błędów programu (SegFault) - żeby posprzątał nawet jak się zepsuje
    signal(SIGSEGV, clean_exit);
    signal(SIGABRT, clean_exit);
    
    // Obsługa zombie
    struct sigaction sa;
    sa.sa_handler = handle_zombie;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &sa, NULL);

    printf("[KIEROWNIK] Start systemu...\n");

    // 1. Tworzenie zasobów
    shm_id = shmget(SHM_KEY, sizeof(SharedState), IPC_CREAT | 0666);
    check_error(shm_id, "shmget");
    
    stan = (SharedState *)shmat(shm_id, NULL, 0);
    if (stan == (void *)-1) check_error(-1, "shmat");

    // Inicjalizacja pamięci
    stan->aktywne_kasy = 0;
    stan->liczba_kibicow_w_kolejce = 0;
    stan->ewakuacja = 0;
    for(int i=0; i<LICZBA_SEKTOROW; i++) stan->bilety_dostepne[i] = POJEMNOSC_SEKTORA;

    // 2. Tworzenie semaforów (Mutex + zapas)
    sem_id = semget(SEM_KEY, 20, IPC_CREAT | 0666);
    check_error(sem_id, "semget");
    sem_init(sem_id, SEM_MUTEX_MAIN, 1);

    // 3. Uruchamianie pracowników
    // Kasjerzy (2 sztuki)
    for(int i=0; i<2; i++) {
        if(fork() == 0) {
            execl("./kasjer", "kasjer", NULL);
            exit(1);
        }
    }
    // Pracownicy techniczni (2 sztuki dla testu)
    for(int i=0; i<2; i++) {
        if(fork() == 0) {
            execl("./pracownik", "pracownik", NULL);
            exit(1);
        }
    }

    printf("[KIEROWNIK] System działa. Generuję kibiców...\n");

    // 4. Pętla główna - generator kibiców
    while(1) {
        sleep(1);
        
        // 60% szans na nowego kibica co sekundę
        if (rand() % 100 < 60) {
            if (fork() == 0) {
                execl("./kibic", "kibic", NULL);
                exit(1);
            }
        }
    }
    return 0;
}