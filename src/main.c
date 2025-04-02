#include "../include/sqlite3.h"
#include "../include/requete.h"
#include "../include/ui_term.h"
#include "../include/generation.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>



int main(){
  int statut = 1;
  sqlite3 *db = NULL;
  FILE *logs;
  edt e;
  int i, j ;

  /* Acces aux logs */
  logs = fopen("logs", "a+");
  if (logs == NULL) fprintf(stderr, "impossible d'accéder aux logs.\n");

  /* Ouverture de la BDD */
  ouvrir_bdd("data/santadata2.db", &db, logs);

 /*nb_lignes_ouvrables(db, logs, semaine2id(db, logs, 1, 1, 2025), 1);*/
  while (statut == 1){
    statut = menu_principal(db, logs);
  }
  
 
  fermer_bdd(db, logs);
  fclose(logs);
  exit(EXIT_SUCCESS);
}
