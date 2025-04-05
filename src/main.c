#include "../include/sqlite3.h"
#include "../include/requete.h"
#include "../include/ui_term.h"
#include "../include/generation.h"
#include "../include/liste_chainee.h" 
#include "../include/ui_gtk.h" 
#include "../include/var.h" 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

sqlite3 *db = NULL;
FILE *logs = NULL;

int main(int argc, char **argv){
  int statut = 1; 
  edt e;
  int i, j ;
  liste_chainee test;

  /* Acces aux logs */
  logs = fopen("logs", "a+");
  if (logs == NULL) fprintf(stderr, "impossible d'accéder aux logs.\n");

  /* Ouverture de la BDD */
  ouvrir_bdd("data/santadata2.db", &db, logs);

 /*nb_lignes_ouvrables(db, logs, semaine2id(db, logs, 1, 1, 2025), 1);*/
  /*
  while (statut == 1){
    statut = menu_principal(db, logs);
  }
  */

  creation_table_edt_ligne_semaine(db, logs, e, 2, 1);
  demarrage_appli(argc, argv);



 
  fermer_bdd(db, logs);
  fclose(logs);
  exit(EXIT_SUCCESS);
}
