#ifndef REQUETE_H_
#define REQUETE_H_

#include "sqlite3.h"
#include "types.h"
#include <stdio.h>

/*
 * \file requete.h
 *
 * \brief Fichier d'en-tête de l'interface C/SQlite de santaplanner
*/


/*!
 * \fn err_alloc(void *p)
 * \brief Vérifie si une allocation mémoire s'est bien déroulée
 *
 * \param p Le pointeur vers la zone allouée
*/
int err_alloc(void *p);

/*!
 * \fn int info_temps(char *temps_buffer )
 * \brief Récupère la date et heure
 *
 * \param temps_buffer Le buffer où inscrire les infos temporelles récupérées
 */
int info_temps(char *temps_buffer );

/*!
 * \fn int ajoute_log(FILE *logs, char * message)
 * \brief Inscrit un message dans le fichier de logs
 *
 * \param logs Le fichier où écrire les logs
 * \param message Le message à écrire
*/
int ajoute_log(FILE *logs, char * message);


/*!
 * \fn int ouvrir_bdd(const char *chemin, sqlite3 **db, FILE *logs)
 * \brief Ouvre une base de données
 *
 * \param chemin Le chemin vers la base de données
 * \param db Un pointeur vers l'objet sqlite où ouvrir la base de données
 * \param logs Le fichier où écrire les logs
*/
int ouvrir_bdd(const char *chemin, sqlite3 **db, FILE *logs);


/*!
 * \fn int fermer_bdd(sqlite3 *db, FILE *logs)
 * \brief Ferme une base de données
 *
 * \param db L'objet sqlite où est ouverte la base de données
 * \param logs Le fichier où écrire les logs
*/
int fermer_bdd(sqlite3 *db, FILE *logs);

/*!
 * \fn int compiler_requete(sqlite3 *db, char *requete_txt, sqlite3_stmt **rq, const char **lecture, FILE *logs){
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

int compiler_requete(sqlite3 *db, char *requete_txt, sqlite3_stmt **rq, const char **lecture, FILE *logs);

 /*!
 * \fn table_resultat *creer_table_res(int nb_col)
 * \brief Crée une nouvelle table de résultats
 *
 * Alloue une zone mémoire pour contenir les résultats d'une requête
 *
 * \param nb_col Le nombre de colonnes renvoyées par la requête
 */
table_resultat *creer_table_res(int nb_col);

/*!
 * \fn table_resultat *ajoute_ligne(table_resultat *t)
 * \brief Ajoute une nouvelle ligne à une table de résultatts
 *
 * \param t Un pointeur vers une table de résultats
*/
table_resultat *ajoute_ligne(table_resultat *t);


/*!
 * \fn void liberer_resultats(table_resultat *t)
 * \brief Libèrer la zone mémoire d'une table de résultats
 *
 * \param t Un pointeur vers une table de résultats
*/
void liberer_resultats(table_resultat *t);


/*!
 * \fn table_resultat *executer_requete(sqlite3_stmt *rq, FILE *logs)
 * \brief Execute une requête compilée et renvoie sa table de résultats
 *
 * Génère une nouvelle table de résultats et la remplit au fur et à mesure
 * avec les résultats de la requête. Ceux-ci sont parcourus ligne par ligne,
 * colonne par colonne, la table est donc agrandie à la volée
 *
 * \param rq Une requête compilée
 * \param logs Le fichier où écrire les logs
 */
table_resultat *executer_requete(sqlite3_stmt *rq, FILE *logs);


/*!
 * \fn void combler_espace(int longueur, int max)
 * \brief Ajoute des espaces pour un affichage plus joli dans un tableau...
 *
 * \param longueur La longueur de la chaîne
 * \param max La longueur d'une cellule du tableau
 */
void combler_espace(int longueur, int max);


/*!
 * \fn void afficher_resultats(table_resultat *t)
 * \brief Affiche le contenu d'une table de résultats
 *
 * \param t Un pointeur vers une table de résulats
*/
void afficher_resultats(table_resultat *t);


/*!
 * \fn char *charger_requete(FILE * fichier_requete)
 * \brief Copie le contenu dans fichier dans un buffer
 *
 * Parcourt un fichier texte et recopie son contenu dans un buffer. Cette fonction
 * est utilisée pour charger des requêtes inscrites dans des fichiers .sql
 *
 * \param fichier_requete Le fichier contenant le code d'une requête
*/
char *charger_requete(FILE * fichier_requete);

/*!
 * \fn void afficher_lutins(sqlite3 *db, FILE *logs)
 * \brief Affiche tous les lutins inscrits dans la base de données
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier où écrire les logs
*/
void afficher_lutins(sqlite3 *db, FILE *logs);

/*!
 * \fn table_resultat * requete_edt_ligne(sqlite3 *db, FILE *logs, int id)
 * \brief Renvoie la planification d'une ligne
 *
 * Récupère dans la table travaille les informations de planification
 * concernant une ligne donnée
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier où écrire les logs
 * \param id L'id d'une ligne
*/
table_resultat * requete_edt_ligne(sqlite3 *db, FILE *logs, int id);

/*!
 * \fn table_resultat *requete_edt_lutin(sqlite3 *db, FILE *logs, int id)
 * \brief Renvoie la planification d'un lutin
 *
 * Récupère dans la table travaille les informations de planification
 * concernant un lutin donnée
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier où écrire les logs
 * \param id L'id d'une ligne
*/
table_resultat *requete_edt_lutin(sqlite3 *db, FILE *logs, int id);

/*!
 * \fn table_resultat *requete_inscrire_lutin(sqlite3 *db, FILE *logs, char *nom, char *specialite)
 * \brief Inscrit un nouveau lutin dans la base de données
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier où écrire les logs
 * \param nom Le nom d'un lutin
 * \param specialite La spécialité d'un lutin
 */
table_resultat *requete_inscrire_lutin(sqlite3 *db, FILE *logs, char *nom, char *specialite);

/*!
 * \fn table_resultat *requete_inscrire_absence(sqlite3 *db, FILE *logs, int id_lutin, int id_jour, int id_semaine, int id_creneau);
 * \brief Inscrit une absence dans la base de données
 *
 * Une absence étant la non-disponibilité, inscrire une absence correspond en fait
 * à supprimer une entrée dans la table disponibilité
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier où écrire les logs
 * \param id_lutin L'id d'un lutin
 * \param id_jour L'id d'un jour
 * \param id_semaine L'id d'une semaine
 * \param id_creneau L'id d'un creneau
 */
table_resultat *requete_inscrire_absence(sqlite3 *db, FILE *logs, int id_lutin, int id_jour, int id_semaine, int id_creneau);

/*!
 * \fn table_resultat *requete_inscrire_dispo(sqlite3 *db, FILE *logs, int id_lutin, int id_jour, int id_semaine, int id_creneau);
 * \brief Inscrit une disponibilité dans la base de données
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier où écrire les logs
 * \param id_lutin L'id d'un lutin
 * \param id_jour L'id d'un jour
 * \param id_semaine L'id d'une semaine
 * \param id_creneau L'id d'un creneau
 */
table_resultat *requete_inscrire_dispo(sqlite3 *db, FILE *logs, int id_lutin, int id_jour, int id_semaine, int id_creneau);

/*!
 * \fn table_resultat *requete_supprimer_lutin(sqlite3 *db, FILE *logs, int id_lutin);
 * \brief Supprime un lutin de la base de données
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier où écrire les logs
 * \param id_lutin L'id d'un lutin
 */
table_resultat *requete_supprimer_lutin(sqlite3 *db, FILE *logs, int id_lutin);

int edt_est_vide(sqlite3 *db, FILE *logs);


#endif /* REQUETE_H_*/
