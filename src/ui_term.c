#include "../include/ui_term.h"
#include "../include/requete.h"
#include "../include/generation.h"
#include <stdio.h>
#include <stdlib.h>


void viderBuffer(){
  int c = 0;
  while (c != '\n' && c != EOF)
    {
      c = getchar();
    }
}

int quelle_semaine(sqlite3 *db, FILE *logs){
  int mois, annee, num_semaine;

  printf("Quel numéro de semaine ? \n");
  viderBuffer();
  scanf("%d", &num_semaine);

  printf("Quel mois ? \n");
  viderBuffer();
  scanf("%d", &mois);

  printf("Quelle année ? \n");
  viderBuffer();
  scanf("%d", &annee);

  return semaine2id(db, logs, num_semaine, mois, annee);
}

void consulter_edt_ligne_terminal(sqlite3 *db, FILE *logs){
  table_resultat *t;
  int id;

  printf("De quelle ligne souhaitez-vous consulter l'emploi du temps ? : \n");
  viderBuffer();
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
  viderBuffer();
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
  viderBuffer();
  scanf("%s", nom);
  printf("Quelle est sa spécialité ? : \n 1 : Bricoleur \n 2 : Controleur \n 3 : Empaqueteur \n");
  viderBuffer();
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

  liberer_resultats(t);
  return;
}

void inscrire_absence_terminal(sqlite3 *db, FILE *logs){
  table_resultat *t;
  int id_lutin, id_jour, id_semaine, id_creneau;

  afficher_lutins(db, logs);
  printf("Pour quel lutin voulez-vous inscrire une absence ? : \n");
  scanf("%d", &id_lutin);

  printf("Quel jour ? \n");
  printf("1 : Lundi \n2 : Mardi \n3 : Mercredi \n4 : Jeudi \n5 : Vendredi \n6 : Samedi \n7 : Dimanche \n\n");
  viderBuffer();
  scanf("%d", &id_jour);

  id_semaine = quelle_semaine(db, logs);

  printf("Quel créneau ?\n");
  viderBuffer();
  for (int i = 8; i < 22; i++) {
    printf("%d : %dh-%dh\n", i - 7, i, i + 1);
  }
  scanf("%d", &id_creneau);

  t = requete_inscrire_absence(db, logs, id_lutin, id_jour, id_semaine, id_creneau);
  printf("***************************************\n");
  if (t == NULL){
    printf("L'inscription a échoué\n");
    return ;
  }
  printf("Inscription réussie\n");
  printf("***************************************\n");
  printf("Mise à jour des emploi du temps concernés...");
  maj_edt_semaine(db, logs, id_semaine);

  liberer_resultats(t);
  return;
}

int menu_principal(sqlite3 *db, FILE *logs){
  int touche;

  printf("1: Consulter l'emploi du temps d'une ligne \n");
  printf("2: Consulter l'emploi du temps d'un lutin \n");
  printf("3: Inscrire un nouveau lutin \n");
  printf("4: Inscrire une absence \n");
  printf("5: Générer les emplois du temps \n");
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
  case 5:
    generer_tous_edt(db, logs);
    break;
  default:
    break;
  }

  return 1;
}
