#ifndef UI_TERM_H_
#define UI_TERM_H_

#include "../include/sqlite3.h"
#include <stdio.h>


/*
 * \file ui_term.h
 *
 * \brief En-tête de la gestion de l'interface terminale
 */

/*!
 * \fn void viderBuffer()
 * \brief Vide stdout pour éviter les problèmes de scanf
 */
void viderBuffer();

/*!
 * \fn int quelle_semaine(sqlite3 *db, FILE *logs)
 * \brief Demande à l'utilisateur de choisir une semaine
 *
 * Récupère un numéro de semaine (i.e. 3eme semaine de l'année), un mois et une année et appelle semaine2id
 * pour renvoyer l'id lui correspondant dans la base de données
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier où écrire les logs
 */
int quelle_semaine(sqlite3 *db, FILE *logs);

/*!
 * \fn void consulter_edt_ligne_terminal(sqlite3 *db, FILE *logs)
 * \brief Sous-menu gérant la consultation de la planification d'une ligne
 *
 * Récupère un id_ligne auprès de l'utilisateur, gère l'affichage de l'en-tête de la table des résultats
 * et appelle requete_edt_ligne
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier où écrire les logs
 */
void consulter_edt_ligne_terminal(sqlite3 *db, FILE *logs);

/*!
 * \fn void consulter_edt_lutin_terminal(sqlite3 *db, FILE *logs)
 * \brief Sous-menu gérant la consultation de la planification d'un lutin
 *
 * Affiche l'ensemble des lutins inscrits dans la base de données (pour simplifier l'interaction),
 * récupère un id_lutin auprès de l'utilisateur, gère l'affichage de l'en-tête de la table des
 * résultats et appelle requete_edt_lutin
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier où écrire les logs
 */
void consulter_edt_lutin_terminal(sqlite3 *db, FILE *logs);

/*!
 * \fn void inscrire_lutin_terminal(sqlite3 *db, FILE *logs)
 * \brief Sous-menu gérant l'inscription d'un nouveau lutin
 *
 * Récupère auprès de l'utilisateur les informations d'un nouveau lutin puis
 * appelle requete_inscrire_lutin
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier où écrire les logs
 */
void inscrire_lutin_terminal(sqlite3 *db, FILE *logs);

/*!
 * \fn void inscrire_absence_terminal(sqlite3 *db, FILE *logs)
 * \brief Sous-menu gérant l'inscription d'une absence d'un lutin
 *
 * Récupère auprès de l'utilisateur les informations concernant une absence,
 * puis appelle requete_inscrire_absence.
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier où écrire les logs
 */
void inscrire_absence_terminal(sqlite3 *db, FILE *logs);


/*!
 * \fn int menu_principal(sqlite3 *db, FILE *logs)
 * \brief Gestion du menu principal
 *
 * Affiche les différentes options possibles en ouvrant le logiciel et gère les
 * entrées de l'utilisateur
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier où écrire les logs
 * \returns Le statut d'éxécution du logiciel
 */
int menu_principal(sqlite3 *db, FILE *logs);

#endif /*UI_TERM_H*/
