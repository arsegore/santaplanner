#include "../include/ui_term.h"
#include "../include/requete.h"
#include <stdio.h>
#include <stdlib.h>

void consulter_edt_ligne(){
  sqlite3 *db = NULL;
  FILE *fichier_rq, *logs;
  char * requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  int id;

  logs = fopen("logs", "a+");
  if (logs == NULL) fprintf(stderr, "impossible d'accéder aux logs.\n");

  ouvrir_bdd("data/santadata.db", &db, logs);


  fichier_rq = fopen("data/edt_ligne.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  compiler_requete(db, requete_txt, &rq, &lecture, logs);

  printf("De quelle ligne souhaitez-vous consulter l'emploi du temps ? : \n");
  scanf("%d", &id);
  inserer_id(rq, id);

  printf("+-----------------------------------------------------------+\n");
  printf("| Jour      | Semaine   | Début     | Fin       | Ligne     |\n");
  executer_afficher_requete(rq, logs);

  fermer_bdd(db, logs);
  fclose(logs);
  fclose(fichier_rq);
  free(requete_txt);
  return;
}

void consulter_edt_lutin(){
  sqlite3 *db = NULL;
  FILE *fichier_rq, *logs;;
  char * requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  int id;

  logs = fopen("logs", "a+");
  if (logs == NULL) fprintf(stderr, "impossible d'accéder aux logs.\n");

  ouvrir_bdd("data/santadata.db", &db, logs);


  fichier_rq = fopen("data/edt_lutin.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  compiler_requete(db, requete_txt, &rq, &lecture, logs);

  printf("De qui souhaitez-vous consulter l'emploi du temps ? : \n");
  scanf("%d", &id);
  inserer_id(rq, id);

  printf("+-----------------------------------------------------------+\n");
  printf("| Jour      | Semaine   | Début     | Fin       | Ligne     |\n");
  executer_afficher_requete(rq, logs);

  fermer_bdd(db, logs);
  fclose(logs);
  fclose(fichier_rq);
  free(requete_txt);

  return;
}

void inscrire_lutin(){
  printf("test\n");
  return;
}

void inscrire_absence(){
  printf("test\n");
  return;
}

int menu_principal(){
  int touche;

  printf("1: Consulter l'emploi du temps d'une ligne \n");
  printf("2: Consulter l'emploi du temps d'un lutin \n");
  printf("3: Inscrire un nouveau lutin \n");
  printf("4: Inscrire une absence \n");
  printf("Que choisissez vous ? : \n");

  if (scanf("%d", &touche) != 1) return 0;

  switch(touche){
  case 1:
    consulter_edt_ligne();
    break;
  case 2:
    consulter_edt_lutin();
    break;
  case 3:
    inscrire_lutin();
    break;
  case 4:
    inscrire_absence();
    break;
  default:
    break;
  }
  return 1;
}
