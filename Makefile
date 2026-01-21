CC = gcc
CFLAGS = -Wall -Wextra -pthread -g

TARGETS = kierownik kasjer pracownik kibic

all: $(TARGETS)

ipc_utils.o: ipc_utils.c ipc_utils.h common.h
	$(CC) $(CFLAGS) -c ipc_utils.c

kierownik: kierownik.c ipc_utils.o common.h
	$(CC) $(CFLAGS) kierownik.c ipc_utils.o -o kierownik

kasjer: kasjer.c ipc_utils.o common.h
	$(CC) $(CFLAGS) kasjer.c ipc_utils.o -o kasjer

pracownik: pracownik.c ipc_utils.o common.h
	$(CC) $(CFLAGS) pracownik.c ipc_utils.o -o pracownik

kibic: kibic.c ipc_utils.o common.h
	$(CC) $(CFLAGS) kibic.c ipc_utils.o -o kibic

clean:
	rm -f *.o $(TARGETS)

ipc-clean:
	ipcrm -a