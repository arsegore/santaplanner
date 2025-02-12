#include <stdio.h>
#include <stdlib.h>
#include "../include/sqlite3.h"

/* Pour vérifier les erreurs d'alloc */
int err_alloc(void *p){
    if (p == NULL){
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return 1; 
    }
    return 0;
}
    
char *lit_requete(FILE * fichier_requete){
    int taille;
    char *req;
    
    /* On récupère la taille du fichier (le nb de caracteres)
       en bougeant la tete de lecture */
    fseek(fichier_requete, 0, SEEK_END); 
    taille = ftell(fichier_requete);
    fseek(fichier_requete, 0, SEEK_SET);
    
    req = (char *) malloc (taille * sizeof(char));
    if (err_alloc(req)) return NULL;

    /* Recopie du fichier */
    fread(req, 1, taille, fichier_requete);

    return req;
}

int main(){
    sqlite3 *db = NULL;  
    int res;
    char *err = NULL;
    FILE * f;
    char *requete; /* Un buffer pour stocker une requete sql */


    /* Ouverture de la BDD */
    res = sqlite3_open("data/santadata.db", &db);

    if (res != SQLITE_OK){
        fprintf(stderr, "Impossible d'accéder aux données.\n");
        sqlite3_close(db);

        exit(EXIT_FAILURE);
    }

    /* Lecture de la requete */
    f = fopen("data/reqtest.sql", "r");
    if (f == NULL){
        fprintf(stderr, "Erreur lecture requete\n");
        exit(EXIT_FAILURE);
    }
    requete = lit_requete(f);
    printf("requete =\n %s\n", requete);

    

    /* Execution de la requete */
    res = sqlite3_exec(db, requete, 0, 0, &err);

    if (res != SQLITE_OK){
        fprintf(stderr, "Erreur SQLite: %s\n", err);
        sqlite3_free(err);
        sqlite3_close(db);

        exit(EXIT_FAILURE);
    }
    
    fclose(f);
    free(requete);
    requete = NULL;
    sqlite3_close(db);

    exit(EXIT_SUCCESS);
}
