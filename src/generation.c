#include "../include/generation.h"
#include "../include/requete.h"
#include "../include/liste_chainee.h"

#include <stdio.h>
#include <stdlib.h>

int semaine2id(sqlite3 *db, FILE *logs, int num_semaine, int mois, int annee){
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  table_resultat *t;
  int res;

  fichier_rq = fopen("data/semaine2id.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);
  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  sqlite3_bind_int(rq, 1, num_semaine);
  sqlite3_bind_int(rq, 2, mois);
  sqlite3_bind_int(rq, 3, annee);
  t = executer_requete(rq, logs);
  afficher_resultats(t);
  res = atoi(t->valeurs[0][0]);

  printf("id_semaine = %d\n", res);

  liberer_resultats(t);
  fclose(fichier_rq);
  free(requete_txt);
  return res;
}

int nb_lignes_ouvrables(sqlite3 *db, FILE *logs, int id_semaine, int id_jour, int id_creneau){
  int res;
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  table_resultat *t;

  fichier_rq = fopen("data/lignes_ouvrables.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  sqlite3_bind_int(rq, 1, id_creneau);
  sqlite3_bind_int(rq, 2, id_semaine);
  sqlite3_bind_int(rq, 3, id_jour);
  t = executer_requete(rq, logs);
  res = atoi(t->valeurs[0][0]);

  liberer_resultats(t);
  fclose(fichier_rq);
  free(requete_txt);
  return res / 3;
}

void ouvrir_ligne(sqlite3 *db, FILE *logs, int id_semaine, int id_jour, int id_creneau, int id_ligne){
  table_resultat *lutins_dispo, *t;
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  int i, id_dispo;

  /* On recupere l'ensemble des lutins disponibles */
  fichier_rq = fopen("data/dispos_a_inserer.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  sqlite3_bind_int(rq, 1, id_jour);
  sqlite3_bind_int(rq, 2, id_semaine);
  sqlite3_bind_int(rq, 3, id_creneau);
  lutins_dispo = executer_requete(rq, logs);
  fclose(fichier_rq);

  /* On ouvre une ligne */
  fichier_rq = fopen("data/ouvrir_ligne.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  for (i = 0; i < 3; i++){
    compiler_requete(db, requete_txt, &rq, &lecture, logs);
    id_dispo = atoi(lutins_dispo->valeurs[i][0]);
    sqlite3_bind_int(rq, 1, id_dispo);
    sqlite3_bind_int(rq, 2, id_ligne);
    t = executer_requete(rq, logs);
    liberer_resultats(t);
  }

  liberer_resultats(lutins_dispo);
  fclose(fichier_rq);
  free(requete_txt);
  return;
}

int ligne_libre(sqlite3 *db, FILE *logs, int id_semaine, int id_jour, int id_creneau){
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  table_resultat *t;
  int res;

  fichier_rq = fopen("data/ligne_libre.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  sqlite3_bind_int(rq, 1, id_creneau);
  sqlite3_bind_int(rq, 2, id_semaine);
  sqlite3_bind_int(rq, 3, id_jour);
  t = executer_requete(rq, logs);
  res = atoi(t->valeurs[0][0]);
  ajoute_log(logs, "Ligne ouverte.\n");

  liberer_resultats(t);
  fclose(fichier_rq);
  free(requete_txt);
  return res;
}

void generer_edt_jour(sqlite3 *db, FILE *logs, int id_semaine, int id_jour){
  int id_creneau, nbl, i, id_ligne;

  for (id_creneau = 1; id_creneau < 15; id_creneau++){
    nbl = nb_lignes_ouvrables(db, logs, id_semaine, id_jour, id_creneau);
    if (nbl > 0){
      for (i = 0; i < nbl; i++){
        id_ligne = ligne_libre(db, logs, id_semaine, id_jour, id_creneau);
        ouvrir_ligne(db, logs, id_semaine, id_jour, id_creneau, id_ligne);
      }
    }
  }
  return;
}

void generer_edt_semaine(sqlite3 *db, FILE *logs, int id_semaine){
  int id_jour;

  for (id_jour = 1; id_jour < 8; id_jour++){
    generer_edt_jour(db, logs, id_semaine, id_jour);
  }

  return;
}

void nettoyer_edt_semaine(sqlite3 *db, FILE *logs, int id_semaine){
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  table_resultat *t;

  fichier_rq = fopen("data/nettoyer_edt_semaine.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  sqlite3_bind_int(rq, 1, id_semaine);
  t = executer_requete(rq, logs);

  liberer_resultats(t);
  fclose(fichier_rq);
  free(requete_txt);
  return ;
}

void maj_edt_semaine(sqlite3 *db, FILE *logs, int id_semaine){
  nettoyer_edt_semaine(db, logs, id_semaine);
  generer_edt_semaine(db, logs, id_semaine);
}

void nettoyer_tous_edt(sqlite3 *db, FILE *logs){
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  table_resultat *t;

  fichier_rq = fopen("data/nettoyer_tous_edt.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  compiler_requete(db, requete_txt, &rq, &lecture, logs);

  t = executer_requete(rq, logs);

  liberer_resultats(t);
  fclose(fichier_rq);
  free(requete_txt);
  return ;
}

void generer_tous_edt(sqlite3 *db, FILE *logs){
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  table_resultat *t;
  int i;

  nettoyer_tous_edt(db, logs);

  fichier_rq = fopen("data/semaines.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  t = executer_requete(rq, logs);

  for (i = 0; i < t->nb_ligne; i++ ){
    generer_edt_semaine(db, logs, atoi(t->valeurs[i][0]));
  }

  liberer_resultats(t);
  fclose(fichier_rq);
  free(requete_txt);
  return;

}

void init_edt(edt e){
  int i, j;
  for (i = 0; i < 14; i++){
    for (j = 0; j < 7; j++){
       e[i][j] = liste_vide(); 
    } 
  }
}


/* PLANNING GLOBAL DES LIGNES (toutes les lignes pr chaque creneau) */ 
void creation_table_edt_ligne_semaine(sqlite3 *db, FILE *logs, edt e, int id_semaine, int mois){
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  table_resultat *t;
  int i;

  fichier_rq = fopen("data/edt_ligne_semaine.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);
  /* Execution de la requete */
  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  sqlite3_bind_int(rq, 1, id_semaine);
  sqlite3_bind_int(rq, 2, mois); 
  t = executer_requete(rq, logs);
  afficher_resultats(t);

  init_edt(e); /* Un edt est une matrice de listes */ 
  /* Premiere colonne : les id_creneau 
   * Deuxieme colonne : les id_jour 
   * Troisieme colonne : les id_ligne */ 
  for (i = 0; i < t->nb_ligne; i++){
     e[ atoi(t->valeurs[i][0]) - 1 ][ atoi(t->valeurs[i][1]) - 1] = inserer_fin(e[ atoi(t->valeurs[i][0]) - 1 ][ atoi(t->valeurs[i][1]) - 1], atoi(t->valeurs[i][2]));
  }

  return ;
}
/* PLANNING GLOBAL D'UNE LIGNE (affiche les lutins qui bossent dessus à un creneau donné */
void creation_table_edt_ligne_semaine_avec_id(sqlite3 *db, FILE *logs, edt e, int id_semaine, int mois, int id_ligne){
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  table_resultat *t;
  int i;

  fichier_rq = fopen("data/edt_ligne_semaine_avec_id.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);
  /* Execution de la requete */
  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  sqlite3_bind_int(rq, 1, id_semaine);
  sqlite3_bind_int(rq, 2, mois); 
  sqlite3_bind_int(rq, 3, id_ligne);
  t = executer_requete(rq, logs);
  afficher_resultats(t);

  init_edt(e); /* Un edt est une matrice de listes */ 
  /* Premiere colonne : les id_creneau 
   * Deuxieme colonne : les id_jour 
   * Troisieme colonne : les id_lutin */ 
  for (i = 0; i < t->nb_ligne; i++){
     e[ atoi(t->valeurs[i][0]) - 1 ][ atoi(t->valeurs[i][1]) - 1] = inserer_fin(e[ atoi(t->valeurs[i][0]) - 1 ][ atoi(t->valeurs[i][1]) - 1], atoi(t->valeurs[i][2]));
  }

  return ;
}




void creation_table_edt_lutin_semaine(sqlite3 *db, FILE *logs, edt e, int id_semaine, int mois, int id_lutin){
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  table_resultat *t;
  int i;

  fichier_rq = fopen("data/edt_ligne_semaine.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);
  
  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  sqlite3_bind_int(rq, 1, id_semaine);
  sqlite3_bind_int(rq, 2, mois); 
  sqlite3_bind_int(rq, 3, id_lutin);
  t = executer_requete(rq, logs);
  afficher_resultats(t);

  init_edt(e);
  for (i = 0; i < t->nb_ligne; i++){
     e[ atoi(t->valeurs[i][0])][ atoi(t->valeurs[i][1])] = inserer_fin(e[ atoi(t->valeurs[i][0])][ atoi(t->valeurs[i][1])], atoi(t->valeurs[i][2]));
  }

  return ;
}


