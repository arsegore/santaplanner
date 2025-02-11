CC=gcc
CFLAGS=-W -Wall

santaplanner: testsql.o sqlite3.o
	gcc build/testsql.o build/sqlite3.o -o build/santaplanner

sqlite3.o: 
	gcc -c src/sqlite3.c -o build/sqlite3.o

testsql.o:
	gcc -c src/testsql.c -o build/testsql.o