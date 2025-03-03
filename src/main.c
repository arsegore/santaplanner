#include "../include/sqlite3.h"
#include "../include/requete.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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
  FILE *fichier_rq;
  char * requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  FILE *logs;

  logs = fopen("logs", "a+");
  if (logs == NULL) fprintf(stderr, "impossible d'accéder aux logs.\n");

  ouvrir_bdd("data/santadata.db", &db, logs);

  /* juste pour tester, sera modifié par la suite */
  fichier_rq = fopen("data/reqtest.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = lit_requete(fichier_rq);

  compiler_requete(db, requete_txt, &rq, &lecture, logs);

  executer_requete(rq, logs);

  fermer_bdd(db, logs);
  fclose(logs);
  fclose(fichier_rq);
  free(requete_txt);

  exit(EXIT_SUCCESS);

}
