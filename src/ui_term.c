#include "../include/ui_term.h"
#include "../include/requete.h"
#include <stdio.h>
#include <stdlib.h>

void consulter_edt_ligne_terminal(sqlite3 *db, FILE *logs){
  table_resultat *t;
  int id;

  printf("De quelle ligne souhaitez-vous consulter l'emploi du temps ? : \n");
  scanf("%d", &id);
  t = requete_edt_ligne(db, logs, id);

  printf("+-----------------------------------------------------------+\n");
  printf("| Jour      | Semaine   | Début     | Fin       | Ligne     |\n");

  afficher_resultats(t);
  liberer_resultats(t);
  return;
}

void consulter_edt_lutin_terminal(sqlite3 *db, FILE *logs){
  table_resultat *t;
  int id;

  afficher_lutins(db, logs);
  printf("De quel lutin souhaitez-vous consulter l'emploi du temps ? : \n");
  scanf("%d", &id);
  t = requete_edt_lutin(db, logs, id);

  printf("+-----------------------------------------------------------+\n");
  printf("| Jour      | Semaine   | Début     | Fin       | Ligne     |\n");

  afficher_resultats(t);
  liberer_resultats(t);
  return;
}

void inscrire_lutin_terminal(sqlite3 *db, FILE *logs){
  char nom[50];
  int i;
  table_resultat *t = NULL;
  printf("Qui souhaitez-vous inscrire ? : \n");
  scanf("%s", nom);
  printf("Quelle est sa spécialité ? : \n 1 : Bricoleur \n 2 : Controleur \n 3 : Empaqueteur \n");
  scanf("%d", &i);

  switch(i){
  case 1:
    t = requete_inscrire_lutin(db, logs, nom, "Bricoleur");
    break;
  case 2:
    t = requete_inscrire_lutin(db, logs, nom, "Controleur");
    break;
  case 3:
    t = requete_inscrire_lutin(db, logs, nom, "Empaqueteur");
    break;
  }
  printf("***************************************\n");
  (t == NULL) ? printf("L'inscription a échoué\n") : printf("Inscription réussie\n");
  printf("***************************************\n");
  return;
}

void inscrire_absence_terminal(sqlite3 *db, FILE *logs){
  table_resultat *t;
  int id_lutin, id_jour, id_semaine, id_creneau;

    afficher_lutins(db, logs);
  printf("Pour quel lutin voulez-vous inscrire une absence ? : \n");
  scanf("%d", &id_lutin);

  printf("Quel jour ? \n");
  printf("1 : Lundi \n 2 : Mardi \n 3 : Mercredi \n 4 : Jeudi \n 5 : Vendredi \n 6 : Samedi \n 7 : Dimanche \n\n");
  scanf("%d", &id_jour);

  printf("Quelle semaine ? \n");
  scanf("%d", &id_semaine);

  printf("Quel créneau ?\n");
  for (int i = 8; i < 22; i++) {

        printf("%d : %dh-%dh\n", i - 7, i, i + 1);
    }
  scanf("%d", &id_creneau);

  t = requete_inscrire_absence(db, logs, id_lutin, id_jour, id_semaine, id_creneau);
  printf("***************************************\n");
  (t == NULL) ? printf("L'inscription a échoué\n") : printf("Inscription réussie\n");
  printf("***************************************\n");
  return;
}

int menu_principal(sqlite3 *db, FILE *logs){
  int touche;

  printf("1: Consulter l'emploi du temps d'une ligne \n");
  printf("2: Consulter l'emploi du temps d'un lutin \n");
  printf("3: Inscrire un nouveau lutin \n");
  printf("4: Inscrire une absence \n");
  printf("q: Quitter \n");
  printf("Que choisissez vous ? : \n");

  if (scanf("%d", &touche) != 1) return 0;

  switch(touche){
  case 1:
    consulter_edt_ligne_terminal(db, logs);
    break;
  case 2:
    consulter_edt_lutin_terminal(db, logs);
    break;
  case 3:
    inscrire_lutin_terminal(db, logs);
    break;
  case 4:
    inscrire_absence_terminal(db, logs);
    break;
  default:
    break;
  }

  return 1;
}
