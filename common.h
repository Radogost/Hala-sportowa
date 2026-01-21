#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>
#include <time.h>
#include <signal.h>

// --- KONFIGURACJA ---
#define SHM_KEY 0x18001
#define SEM_KEY 0x18002

#define LICZBA_SEKTOROW 8
#define POJEMNOSC_SEKTORA 100
#define MAX_KAS 10

// Limity z tematu
#define MAX_NA_BRAMCE 3

// --- STRUKTURA PAMIĘCI DZIELONEJ ---
typedef struct {
    // Kasy
    int liczba_kibicow_w_kolejce;
    int aktywne_kasy;

    // Bilety
    int bilety_dostepne[LICZBA_SEKTOROW];

    // Bramki (8 sektorów, 2 stanowiska na sektor)
    struct {
        int zajete_miejsca;       // 0-3
        int obslugiwana_druzyna;  // 0=Puste, 1=A, 2=B
    } bramki[LICZBA_SEKTOROW][2];

    // Flaga końca (ewakuacja)
    int ewakuacja;

} SharedState;

// --- SEMAFORY ---
#define SEM_MUTEX_MAIN 0      // Chroni pamięć dzieloną
#define SEM_GATE_BASE 1       // Indeks początkowy semaforów dla bramek (rezerwa)

#endif