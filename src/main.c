#include "../include/sqlite3.h"
#include "../include/requete.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


int main(){
  sqlite3 *db = NULL;
  FILE *fichier_rq;
  char * requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  FILE *logs;
  table_resultat * t;

  logs = fopen("logs", "a+");
  if (logs == NULL) fprintf(stderr, "impossible d'accéder aux logs.\n");

  ouvrir_bdd("data/santadata.db", &db, logs);

  /* juste pour tester, sera modifié par la suite */
  fichier_rq = fopen("data/reqtest.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  executer_afficher_requete(rq, logs);

  fermer_bdd(db, logs);
  fclose(logs);
  fclose(fichier_rq);
  free(requete_txt);

  exit(EXIT_SUCCESS);

}
