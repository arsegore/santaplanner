CC=gcc
CFLAGS=-W -Wall

.PHONY: all all_but_sqlite doc build

all: build ui_gtk.o sqlite3.o requete.o generation.o ui_term.o main.o santaplanner doc

all_but_sqlite: build requete.o liste_chainee.o generation.o ui_term.o main.o santaplanner doc

gtk_only: build ui_gtk.o 
	

build:
	mkdir -p build

doc:
	doxygen ./Doxyfile

santaplanner:
	gcc build/main.o build/liste_chainee.o build/ui_term.o build/generation.o build/requete.o build/sqlite3.o -o build/santaplanner

main.o:
	gcc -c $(CFLAGS) src/main.c -o build/main.o

ui_gtk.o: 
	gcc -c $(CFLAGS) `pkg-config --cflags gtk+-3.0` -o build/ui_gtk.o src/ui_gtk.c `pkg-config --libs gtk+-3.0`

liste_chainee.o:
	gcc -c $(CFLAGS) src/liste_chainee.c -o build/liste_chainee.o

ui_term.o:
	gcc -c $(CFLAGS) src/ui_term.c -o build/ui_term.o

generation.o:
	gcc -c $(CFLAGS) src/generation.c -o build/generation.o

requete.o:
	gcc -c $(CFLAGS) src/requete.c -o build/requete.o

sqlite3.o: 
	gcc -c $(CFLAGS) src/sqlite3.c -o build/sqlite3.o

clean:
	rm -rf build doc
