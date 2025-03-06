CC=gcc
CFLAGS=-W -Wall

all: sqlite3.o requete.o ui_term.o main.o santaplanner

all_but_sqlite: requete.o ui_term.o main.o santaplanner

santaplanner:
	gcc build/main.o build/ui_term.o build/requete.o build/sqlite3.o -o build/santaplanner

main.o:
	gcc -c $(CFLAGS) src/main.c -o build/main.o

ui_term.o:
	gcc -c $(CFLAGS) src/ui_term.c -o build/ui_term.o

requete.o:
	gcc -c $(CFLAGS) src/requete.c -o build/requete.o

sqlite3.o: 
	gcc -c $(CFLAGS) src/sqlite3.c -o build/sqlite3.o
