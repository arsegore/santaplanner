#include <stdio.h>
#include <stdlib.h>
#include "../include/sqlite3.h"


int main(){
    sqlite3 *db = NULL;  
    int res;
    char *err = NULL, *requete = "CREATE TABLE test( id INTEGER PRIMARY KEY );";
    
    res = sqlite3_open("data/test.db", &db);

    if (res != SQLITE_OK){
        fprintf(stderr, "Impossible d'accéder aux données.\n");
        sqlite3_close(db);

        exit(EXIT_FAILURE);
    }

    res = sqlite3_exec(db, requete, 0, 0, &err);

    if (res != SQLITE_OK){
        fprintf(stderr, "Erreur SQLite: %s\n", err);
        sqlite3_free(err);
        sqlite3_close(db);

        exit(EXIT_FAILURE);
    }
    
    
    sqlite3_close(db);

    exit(EXIT_SUCCESS);
}
