#ifndef REQUETE_H_
#define REQUETE_H_

#include "../include/sqlite3.h"
#include <stdio.h>

typedef struct {
  int type; /* 0 : chaine, 1 : int, 2 : float */
  union {
    int res_int;
    float res_float;
    char res_chaine[50];
  };
}requete_valeur;

typedef struct {
  requete_valeur ** valeurs;
  int nb_ligne, nb_col;
}table_resultat;

/* Vérifie si une allocation s'est bien passée */
int err_alloc(void *p);

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

/**/
int compiler_requete(sqlite3 *db, char *requete_txt, sqlite3_stmt **rq, const char **lecture, FILE *logs);

/*
 * Execute une requete, stocke son résultat dans une
 * table de résultats, l'affiche et la libère.
 * rq : la requete compilée à éxécuter
 * logs : fichier où sont écrits les logs
 */
int executer_afficher_requete(sqlite3_stmt *rq, FILE *logs);

char *charger_requete(FILE * fichier_requete);


#endif // REQUETE_H_
