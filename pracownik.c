#include "common.h"
#include "ipc_utils.h"

int main() {
    // Podłączenie do zasobów
    shmget(SHM_KEY, sizeof(SharedState), 0666);
    
    printf("[PRACOWNIK %d] Pilnuję bramek.\n", getpid());

    while(1) {
        // Pracownik techniczny czeka na sygnały od Kierownika
        // lub sprawdza stan bramek w pamięci dzielonej
        sleep(5);
    }
    return 0;
}