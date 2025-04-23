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

/*!
 * \brief Compile une requete
 *
 * Appelle la fonction sqlite3_prepare_v2, qui prépare une requête à partir d'un code SQL
 * pour son éxécution
 *
 * \param db La bdd sur laquelle interagir
 * \param requete_txt Le code de la requête (code SQL)
 * \param rq Un pointeur vers l'objet sqlite3_stmt qui correspondra à la requête
 * \param lecture Un pointeur vers la portion restante de requete_txt
 * \param logs Le fichier où écrire les logs
*/
int compiler_requete(sqlite3 *db, char *requete_txt, sqlite3_stmt **rq, const char **lecture, FILE *logs){
  int res;

  res = sqlite3_prepare_v2(db, requete_txt, -1, rq, lecture);
  if ( res != SQLITE_OK ){
    ajoute_log(logs, "Erreur à la compilation de la requête.\n");
    fermer_bdd(db, logs);
  }

  return res;
}

/*!
 * \brief Crée une nouvelle table de résultats
 *
 * Alloue une zone mémoire pour contenir les résultats d'une requête
 *
 * \param nb_col Le nombre de colonnes renvoyées par la requête
 */
table_resultat *creer_table_res(int nb_col){
  table_resultat *res = NULL;

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

void liberer_resultats(table_resultat *t){
  int i;

  for(i = 0; i < t->nb_ligne; i++){
    free(t->valeurs[i]);
  }

  free(t->valeurs);
  free(t);
}

table_resultat *executer_requete(sqlite3_stmt *rq, FILE *logs){
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
          strncpy(res->valeurs[res->nb_ligne-1][i], (const char*) sqlite3_column_text(rq, i), strlen( (const char *) sqlite3_column_text(rq, i))+1);
          break;
        case (SQLITE_INTEGER):
          snprintf(res->valeurs[res->nb_ligne-1][i], 50, "%d",sqlite3_column_int(rq, i));
          break;
        case (SQLITE_FLOAT):
          snprintf(res->valeurs[res->nb_ligne-1][i], 50, "%f",sqlite3_column_double(rq, i));
          break;
        default:
          break;

      }
    }
  }

  ajoute_log(logs, "Requete éxécutée avec succès.\n");
  ajoute_log(logs, "Résultats enregistrés dans une table.\n");
  sqlite3_finalize(rq);
  return res;

}

void combler_espace(int longueur, int max){
  int i;

  if (longueur < max){
    for (i = 0; i < max - longueur; i++){
      printf(" ");
    }
  }
  return;
}

void afficher_resultats(table_resultat *t){
  int i,j;

  for (i = 0; i < t->nb_ligne; i++){
    printf("+-----------------------------------------------------------+\n");
    printf("|");
    for (j = 0; j < t->nb_col; j++){
      combler_espace(strlen(t->valeurs[i][j]) + 2, 57 / t->nb_col);
      printf(" %s |", t->valeurs[i][j]);
    }
    printf("\n");
  }

  printf("+-----------------------------------------------------------+\n\n\n");
  return;
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

/*!
 * \brief Affiche tous les lutins inscrits dans la base de données
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier où écrire les logs
*/
void afficher_lutins(sqlite3 *db, FILE *logs){
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  table_resultat *t;

  fichier_rq = fopen("data/afficher_lutins.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  t = executer_requete(rq, logs);
  printf("+-----------------------------------------------------------+\n");
  printf("| Nom               | Numéro            | Spécialité        |\n");
  afficher_resultats(t);
  liberer_resultats(t);

  fclose(fichier_rq);
  free(requete_txt);
}

table_resultat * requete_edt_ligne(sqlite3 *db, FILE *logs, int id){
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  table_resultat *t;

  fichier_rq = fopen("data/edt_ligne.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  sqlite3_bind_int(rq, 1, id);
  t = executer_requete(rq, logs);

  fclose(fichier_rq);
  free(requete_txt);

  return t;
}

table_resultat *requete_edt_lutin(sqlite3 *db, FILE *logs, int id){
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  table_resultat *t;

  fichier_rq = fopen("data/edt_lutin.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  sqlite3_bind_int(rq, 1,id);
  t = executer_requete(rq, logs);

  fclose(fichier_rq);
  free(requete_txt);

  return t;
}

table_resultat *requete_inscrire_lutin(sqlite3 *db, FILE *logs, char *nom, char *specialite){
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  table_resultat *t;

  fichier_rq = fopen("data/inscrire_lutin.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  sqlite3_bind_text(rq, 1, nom, -1, NULL);
  sqlite3_bind_text(rq, 2, specialite, -1, NULL);

  t = executer_requete(rq, logs);

  fclose(fichier_rq);
  free(requete_txt);

  return t;
}

table_resultat *requete_inscrire_absence(sqlite3 *db, FILE *logs, int id_lutin, int id_jour, int id_semaine, int id_creneau){
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  table_resultat *t;

  fichier_rq = fopen("data/inscrire_absence.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  sqlite3_bind_int(rq, 1, id_lutin);
  sqlite3_bind_int(rq, 2, id_jour);
  sqlite3_bind_int(rq, 3, id_semaine);
  sqlite3_bind_int(rq, 4, id_creneau);

  t = executer_requete(rq, logs);
  if (t != NULL){
    printf("supp: ltn = %d, smn = %d; crn = %d\n", id_lutin, id_semaine, id_creneau);
  }
  fclose(fichier_rq);
  free(requete_txt);

  return t;
}

table_resultat *requete_inscrire_dispo(sqlite3 *db, FILE *logs, int id_lutin, int id_jour, int id_semaine, int id_creneau){
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  table_resultat *t;

  fichier_rq = fopen("data/inscrire_dispo.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  sqlite3_bind_int(rq, 1, id_lutin);
  sqlite3_bind_int(rq, 2, id_jour);
  sqlite3_bind_int(rq, 3, id_semaine);
  sqlite3_bind_int(rq, 4, id_creneau);

  t = executer_requete(rq, logs);
  if (t != NULL){
    printf("ajt: ltn = %d, smn = %d; crn = %d\n", id_lutin, id_semaine, id_creneau);
  }
  fclose(fichier_rq);
  free(requete_txt);

  return t;
}

table_resultat *requete_supprimer_lutin(sqlite3 *db, FILE *logs, int id_lutin){
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  table_resultat *t;

  fichier_rq = fopen("data/supprimer_lutin.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  sqlite3_bind_int(rq, 1, id_lutin);
  t = executer_requete(rq,logs);

  fclose(fichier_rq);
  free(requete_txt);
  return t;
}

int edt_est_vide(sqlite3 *db, FILE *logs){
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  table_resultat *t;

  fichier_rq = fopen("data/edt_est_vide.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  t = executer_requete(rq,logs);
  if (atoi(t->valeurs[0][0]) == 0) return 1; 

  fclose(fichier_rq);
  free(requete_txt);
  return 0;
}



