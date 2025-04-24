CC = gcc
CFLAGS = -W -Wall
GTK_CFLAGS = `pkg-config --cflags gtk4`
GTK_LFLAGS = `pkg-config --libs gtk4`
BUILD_DIR = build
SRC_DIR = src
BIN_DIR = bin

# sources et objets
SRC_FILES = main.c var.c ui_gtk.c liste_chainee.c ui_term.c generation.c requete.c sqlite3.c export.c alerte.c
OBJ_FILES = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

# cibles
.PHONY: all all_but_sqlite gtk_only build bin doc clean

all: bin build santaplanner doc

all_but_sqlite: build $(filter-out $(BUILD_DIR)/sqlite3.o, $(OBJ_FILES)) santaplanner doc

gtk_only: build $(BUILD_DIR)/ui_gtk.o

build:
	mkdir -p $(BUILD_DIR)

bin:
	mkdir -p $(BIN_DIR)

doc:
	doxygen ./Doxyfile

santaplanner: $(OBJ_FILES)
	$(CC) $^ -o $(BIN_DIR)/santaplanner $(GTK_LFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $(GTK_CFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) doc
