#include "../include/requete.h"
#include "../include/sqlite3.h"
#include <time.h>
#include <stdio.h>
#include <string.h>


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

  res = sqlite3_prepare_v2(db, requete_txt, strlen(requete_txt) + 1, rq, lecture);
  if ( res != SQLITE_OK ){
    ajoute_log(logs, "Erreur à la compilation de la requête.\n");
    fermer_bdd(db, logs);
  }

  return res;
}

int executer_requete(sqlite3_stmt *rq, FILE *logs){
  int res, i, nb_col;

  ajoute_log(logs, "Execution de la requete...\n");

  while( (res = sqlite3_step(rq)) != SQLITE_DONE ){
    nb_col = sqlite3_column_count(rq);

    for (i = 0; i < nb_col; i++){

      switch(sqlite3_column_type(rq, i)){
      case (SQLITE3_TEXT):
        printf("%s | ", sqlite3_column_text(rq, i));
        break;
      case (SQLITE_INTEGER):
        printf("%d | ", sqlite3_column_int(rq, i));
        break;
      case (SQLITE_FLOAT):
        printf("%g | ", sqlite3_column_double(rq, i));
        break;
      default:
        break;

      }
    }
    printf("\n");
  }

  ajoute_log(logs, "Requete éxécutée avec succès.\n");
  sqlite3_finalize(rq);

  return res;
}
