CC=gcc
CFLAGS=-W -Wall 
GTK_CFLAGS=`pkg-config --cflags gtk4`
GTK_LFLAGS=`pkg-config --libs gtk4`


.PHONY: all all_but_sqlite doc build

all: build var.o ui_gtk.o sqlite3.o requete.o liste_chainee.o generation.o ui_term.o main.o santaplanner doc

all_but_sqlite: build var.o ui_gtk.o requete.o liste_chainee.o generation.o ui_term.o main.o santaplanner doc

gtk_only: build ui_gtk.o 
	

build:
	mkdir -p build

doc:
	doxygen ./Doxyfile

santaplanner:
	gcc build/main.o build/var.o build/ui_gtk.o build/liste_chainee.o build/ui_term.o build/generation.o build/requete.o build/sqlite3.o -o build/santaplanner $(GTK_LFLAGS)

main.o:
	gcc -c $(CFLAGS) $(GTK_CFLAGS) src/main.c -o build/main.o

var.o:
	gcc -c $(CFLAGS) $(GTK_CFLAGS) src/var.c -o build/var.o

ui_gtk.o: 
	gcc -c $(CFLAGS) $(GTK_CFLAGS) src/ui_gtk.c -o build/ui_gtk.o 

liste_chainee.o:
	gcc -c $(CFLAGS) $(GTK_CFLAGS) src/liste_chainee.c -o build/liste_chainee.o

ui_term.o:
	gcc -c $(CFLAGS) $(GTK_CFLAGS) src/ui_term.c -o build/ui_term.o

generation.o:
	gcc -c $(CFLAGS) $(GTK_CFLAGS) src/generation.c -o build/generation.o

requete.o:
	gcc -c $(CFLAGS) $(GTK_CFLAGS) src/requete.c -o build/requete.o

sqlite3.o: 
	gcc -c $(CFLAGS) src/sqlite3.c -o build/sqlite3.o

clean:
	rm -rf build doc
