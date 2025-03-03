#ifndef REQUETE_H_
#define REQUETE_H_

#include "../include/sqlite3.h"
#include <stdio.h>

typedef struct {
  int type; /* 0 pour int, 1 pour chaine, ... */
  union {
    int res_int;
    char res_chaine[50];
  };
}resultat_requete;

/*
 * Récupère la date & heure.
 * temps_buffer : chaîne où est écrite la date récupérée
 */
int info_temps(char *temps_buffer );

/*
 * Ajoute au fichier de logs le message passé en parametre.
 * logs : fichier où sont écrits les logs
 * message : le message de log à écrire
 */
int ajoute_log(FILE *logs, char * message);

/*
 * Ouvre une BDD en lecture & écriture.
 * chemin : adresse du fichier sqlite
 * db : pointeur qui pointera sur la bdd ouverte
 */
int ouvrir_bdd(const char *chemin, sqlite3 **db, FILE *logs);

/*
 * Ferme une BDD.
 * db : l'objet bdd à fermer
 * logs : fichier où sont écrits les logs
 */
int fermer_bdd(sqlite3 *db, FILE *logs);

/*
 * Compilation d'une requete SQL.
 * db : l'objet bdd sur lequel travailler
 * requete_txt : le code de la requete
 * rq : pointeur vers un "objet" requete
 * logs : le fichier de logs
 */
int compiler_requete(sqlite3 *db, char *requete_txt, sqlite3_stmt **rq, const char **lecture, FILE *logs);

/*
 * Execution d'une requete SQL
 * rq : la requete compilée à executer
 * logs : le fichier de logs
 */
int executer_requete(sqlite3_stmt *rq, FILE *logs);



#endif // REQUETE_H_
