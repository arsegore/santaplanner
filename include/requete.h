#ifndef REQUETE_H_
#define REQUETE_H_

#include "../include/sqlite3.h"
#include <stdio.h>

typedef char requete_valeur[100];

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

/*
 * Compile une reuquete pour pouvoir l'éxécuter sur la bdd
 */
int compiler_requete(sqlite3 *db, char *requete_txt, sqlite3_stmt **rq, const char **lecture, FILE *logs);


/*
 * Affiche le contenu d'une table de résultats
 */
void afficher_resultats(table_resultat *t);

/*
 * Libère une table de résultats
 */
void liberer_resultats(table_resultat *t);

/*
 *  Execute une requete et renvoie sa table de résultats
 */
table_resultat *executer_requete(sqlite3_stmt *rq, FILE *logs);

/*
 * Charge le code d'une requete depuis un fichier .sql
 */
char *charger_requete(FILE * fichier_requete);

/* Insère un id dans une requête compilée */
void inserer_id(sqlite3_stmt *rq, int id);


/*
 * Affiche tous les lutins, leur numéro et leur spécialité
*/
void afficher_lutins(sqlite3 *db, FILE *logs);

/*
 * Charge et éxécute la requete pour obtenir l'edt d'une ligne
 * et renvoie sa table de résultats
 */
table_resultat * requete_edt_ligne(sqlite3 *db, FILE *logs, int id);

/*
 * Charge et éxécute la requete pour obtenir l'edt d'une ligne
 * et renvoie sa table de résultats
 */
table_resultat * requete_edt_lutin(sqlite3 *db, FILE *logs, int id);

/*
 * Charge et éxécute la requete permettant d'inscrire un lutin dans la bdd
*/
table_resultat * requete_inscrire_lutin(sqlite3 *db, FILE *logs, char *nom, char *specialite);

/*
 * Charge et éxécute la requete permettant d'inscrire une absence dans la bdd
*/
table_resultat * requete_inscrire_absence(sqlite3 *db, FILE *logs, int id_lutin, int id_jour, int id_semaine, int id_creneau);


#endif /* REQUETE_H_*/
