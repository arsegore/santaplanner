#include "../include/requete.h"
#include "../include/sqlite3.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int err_alloc(void *p){
    if (p == NULL){
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return 1;
    }
    return 0;
}

int info_temps(char *temps_buffer ){
  time_t tps;
  struct tm* info_tps;

  tps = time(NULL);
  info_tps = localtime(&tps);

  strftime(temps_buffer, 26, "%Y-%m-%d %H:%M:%S : ", info_tps);

  return 1;
}

int ajoute_log(FILE *logs, char * message){
  char temps_buffer[150];

  info_temps(temps_buffer);
  strcat(temps_buffer, message);
  fprintf(logs, "%s", temps_buffer);

  return 1;
}

int ouvrir_bdd(const char *chemin, sqlite3 **db, FILE *logs){
  int res;

  res = sqlite3_open_v2(chemin, db, SQLITE_OPEN_READWRITE, NULL);
  ajoute_log(logs, "Ouverture de la BDD.\n");

  return res;
}

int fermer_bdd(sqlite3 *db, FILE *logs){

  sqlite3_close(db);
  ajoute_log(logs, "Fermeture de la BDD.\n");

  return 1;
}

int compiler_requete(sqlite3 *db, char *requete_txt, sqlite3_stmt **rq, const char **lecture, FILE *logs){
  int res;

  res = sqlite3_prepare_v2(db, requete_txt, -1, rq, lecture);
  if ( res != SQLITE_OK ){
    ajoute_log(logs, "Erreur à la compilation de la requête.\n");
    fermer_bdd(db, logs);
  }

  return res;
}

table_resultat *creer_table_res(int nb_col){
  table_resultat *res;

  /* Allocation de la memoire pr la struct table */
  res = (table_resultat *) malloc (sizeof(table_resultat));
  if (err_alloc(res)) return NULL;

  /* Allocation du ptr ligne */
  res->valeurs = (requete_valeur **) malloc(sizeof(requete_valeur*));
  if (err_alloc(res->valeurs)) return NULL;

  /* Allocation des colonnes */
  res->valeurs[0] = (requete_valeur *) calloc(nb_col,sizeof(requete_valeur));
  if (err_alloc(res->valeurs[0])) return NULL;

  res->nb_ligne = 0;
  res->nb_col = nb_col;

  return res;

}

table_resultat *ajoute_ligne(table_resultat *t){

  t->valeurs = (requete_valeur **) realloc(t->valeurs, ++t->nb_ligne*sizeof(requete_valeur *));
  if (err_alloc(t)) return NULL;

  t->valeurs[t->nb_ligne - 1] = (requete_valeur *) malloc(t->nb_col*sizeof(requete_valeur));
  if (err_alloc(t->valeurs[t->nb_ligne - 1])) return NULL;

  return t;
}

void liberer_resultat(table_resultat *t){
  int i;

  for(i = 0; i < t->nb_ligne; i++){
    free(t->valeurs[i]);
  }

  free(t->valeurs);
  free(t);
}

table_resultat *remplir_table_res(sqlite3_stmt *rq, FILE *logs){
  int nb_col, i, j;
  table_resultat *res;

  /* Création de la table des resultats */
  ajoute_log(logs, "Création de la table des résultats...\n");
  nb_col = sqlite3_column_count(rq);
  res = creer_table_res(nb_col);

  if (err_alloc(res)) return NULL;

  /* Remplissage de la table */
  ajoute_log(logs, "Execution de la requete...\n");

  for(j = 0;  sqlite3_step(rq) != SQLITE_DONE; j++ ){
    res = ajoute_ligne(res);
    for (i = 0; i < nb_col; i++){

      switch(sqlite3_column_type(rq, i)){
      case (SQLITE3_TEXT):
        res->valeurs[res->nb_ligne-1][i].type = 0;
        strncpy(res->valeurs[res->nb_ligne-1][i].res_chaine, (const char*) sqlite3_column_text(rq, i), 100);
        break;
      case (SQLITE_INTEGER):
        res->valeurs[res->nb_ligne-1][i].type = 1;
        res->valeurs[res->nb_ligne-1][i].res_int = sqlite3_column_int(rq, i);
        break;
      case (SQLITE_FLOAT):
        res->valeurs[res->nb_ligne-1][i].type = 2;
        res->valeurs[res->nb_ligne-1][i].res_int = sqlite3_column_double(rq, i);
        break;
      default:
        break;

      }
    }
  }

  ajoute_log(logs, "Requete éxécutée avec succès.\n");
  ajoute_log(logs, "Résultats enregistrée dans une table.\n");
  sqlite3_finalize(rq);
  return res;

}

void afficher_resultats(table_resultat *t){
  int i,j;

  for (i = 0; i < t->nb_ligne; i++){
    for (j = 0; j < t->nb_col; j++){
      switch(t->valeurs[i][j].type){
      case 0:
        printf("%s / ", t->valeurs[i][j].res_chaine);
        break;
      case 1:
        printf("%d / ", t->valeurs[i][j].res_int);
        break;
      case 2:
        printf("%f / ", t->valeurs[i][j].res_float);
        break;
      default:
        break;
      }
    }
    printf("\n");
  }
}

int executer_afficher_requete(sqlite3_stmt *rq, FILE *logs){
  table_resultat *t;

  t = remplir_table_res(rq, logs);
  if (err_alloc(t)) return 0;
  ajoute_log(logs, "Affichage des résultats\n");
  afficher_resultats(t);
  liberer_resultat(t);

  return 1;
}


char *charger_requete(FILE * fichier_requete){
    int taille;
    char *req;

    /* On récupère la taille du fichier (le nb de caracteres)
       en bougeant la tete de lecture */
    fseek(fichier_requete, 0, SEEK_END);
    taille = ftell(fichier_requete);
    fseek(fichier_requete, 0, SEEK_SET);

    req = (char *) malloc (taille * sizeof(char));
    if (req == NULL) return NULL;

    /* Recopie du fichier */
    fread(req, sizeof(char), taille, fichier_requete);

    return req;
}
