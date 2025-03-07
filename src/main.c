#include "../include/sqlite3.h"
#include "../include/requete.h"
#include "../include/ui_term.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


int main(){
  int statut = 1;
  sqlite3 *db = NULL;
  FILE *logs;;

  logs = fopen("logs", "a+");
  if (logs == NULL) fprintf(stderr, "impossible d'accéder aux logs.\n");
  ouvrir_bdd("data/santadata2.db", &db, logs);

  while (statut == 1){
   statut = menu_principal(db, logs);

  }

  fermer_bdd(db, logs);
  fclose(logs);
  exit(EXIT_SUCCESS);
}
