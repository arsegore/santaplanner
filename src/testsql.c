#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/sqlite3.h"

/* Pour vérifier les erreurs d'alloc */
int err_alloc(void *p){
    if (p == NULL){
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return 1; 
    }
    return 0;
}

/* Pour copier une reqûete à partir d'un fichier dans un buffer */ 
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
    char *requetetxt; /* Un buffer pour stocker une requete sql */
    sqlite3_stmt *ppreq;
    const char *lecture;
    int res_requete; 

    /* Ouverture de la BDD */
    res = sqlite3_open_v2("data/santadata.db", &db, SQLITE_OPEN_READWRITE, NULL); /* ouverture */

    if (res != SQLITE_OK){ /* cas d'erreur */
        fprintf(stderr, "Impossible d'accéder aux données.\n");
        sqlite3_close(db);

        exit(EXIT_FAILURE);
    }

    /* Lecture de la requete depuis un fichier */
    f = fopen("data/reqtest.sql", "r"); /* ouverture du fichier */
    if (f == NULL){ /* cas d'erreur */
        fprintf(stderr, "Erreur lecture requete\n");
        exit(EXIT_FAILURE);
    }
    requetetxt = lit_requete(f); /* copie du fichier dans la chaine */
    printf("requete =\n %s\n", requetetxt); /* affichage de verif */

    /* Compilation de la requete */
    res = sqlite3_prepare_v2(db, requetetxt, strlen(requetetxt) + 1, &ppreq, &lecture);
    if (res != SQLITE_OK){ /* cas d'erreur */
        fprintf(stderr, "Erreur à la compilation de la requête\n");
        sqlite3_close(db);

        exit(EXIT_FAILURE);
    }
    

    /* Execution de la requete */
    res = sqlite3_step(ppreq);

    while (res != SQLITE_DONE){ /* On continue l'éxécution de la requete jusqu'à sa fin */
        /* Si résultat obtenu */
        if (res == SQLITE_ROW){
            res_requete = sqlite3_column_int(ppreq, 0);
            printf("Résultat de la requête : %d\n", res_requete);
        }
        
        res = sqlite3_step(ppreq);
         
    }
    /* Si résultat obtenu */
    if (res == SQLITE_ROW){
        res_requete = sqlite3_column_int(ppreq, 1);
        printf("Résultat de la requête : %d\n", res_requete);
    }

    
    fclose(f);
    free(requetetxt);
    requetetxt = NULL;
    sqlite3_close(db);
    printf("Succès, fermeture de la BDD.\n");
    exit(EXIT_SUCCESS);
}
