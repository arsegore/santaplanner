#ifndef GENERATION_H_
#define GENERATION_H_

#include "sqlite3.h"
#include "liste_chainee.h"
#include <stdio.h>

/*!
 * \file generation.h
 *
 * \brief En-tête des fonctions de planification de la ligne de production
*/

/* Un edt est un tableau de cellules */
typedef liste_chainee edt[20][7]; 

/*!
 * \fn int semaine2id(sqlite3 *db, FILE *logs, int num_semaine, int mois, int annee)
 * \brief Récupère l'id d'une semaine donnée dans la base de données
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
 * \param num_semaine Le numéro de la semaine dans l'année
 * \param mois Le numéro du mois
 * \param annee L'année
*/
int semaine2id(sqlite3 *db, FILE *logs, int num_semaine, int mois, int annee);

/*!
 * \fn int nb_lignes_ouvrables(sqlite3 *db, FILE *logs, int id_semaine, int id_jour, int id_creneau)
 * \brief Renvoie le nombre de lignes ouvrables à un instant donné
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
 * \param id_semaine L'identifiant d'une semaine dans la base de données
 * \param id_jour L'identifiant d'un jour dans la base de données (qui correspond en fait au numéro d'un jour dans la semaine)
 * \param id_creneau L'identifiant d'un créneau dans la base de données
*/
int nb_lignes_ouvrables(sqlite3 *db, FILE *logs, int id_semaine, int id_jour, int id_creneau);


/*!
 * \fn void ouvrir_ligne(sqlite3 *db, FILE *logs, int id_semaine, int id_jour, int id_creneau, int id_ligne);
 * \brief Ouvre une ligne à un instant donné
 *
 * Pour un certain horaire, on récupère l'ensemble des lutins disponibles ordonnés
 * aléatoirement. On insère une nouvelle entrée dans la table travaille pour 3 des lutins
 * disponibles à l'horaire donné sur la ligne choisie.
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
 * \param id_semaine L'identifiant d'une semaine dans la base de données
 * \param id_jour L'identifiant d'un jour dans la base de données (qui correspond en fait au numéro d'un jour dans la semaine)
 * \param id_creneau L'identifiant d'un créneau dans la base de données
 * \param id_ligne La ligne à ouvrir
*/
void ouvrir_ligne(sqlite3 *db, FILE *logs, int id_semaine, int id_jour, int id_creneau, int id_ligne);

/*!
 * \fn int ligne_libre(sqlite3 *db, FILE *logs, int id_semaine, int id_jour, int id_creneau)
 * \brief Renvoie aléatoirement l'une des lignes libres à un horaire donné
 *
 * Récupère l'ensemble des lignes libres à un horaire donné, les ordonne aléatoirement
 * et n'en renvoie qu'une
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
 * \param id_semaine L'identifiant d'une semaine dans la base de données
 * \param id_jour L'identifiant d'un jour dans la base de données (qui correspond en fait au numéro d'un jour dans la semaine)
 * \param id_creneau L'identifiant d'un créneau dans la base de données
 */
int ligne_libre(sqlite3 *db, FILE *logs, int id_semaine, int id_jour, int id_creneau);

/*!
 * \fn void generer_edt_jour(sqlite3 *db, FILE *logs, int id_semaine, int id_jour)
 * \brief Planifie une journée
 *
 * Pour chaque créneau de la journée, vérifie si une ou plusieurs lignes sont ouvrables.
 * Si oui, alors on y ouvre autant de lignes que possible.
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
 * \param id_semaine L'identifiant d'une semaine dans la base de données
 * \param id_jour L'identifiant d'un jour dans la base de données (qui correspond en fait au numéro d'un jour dans la semaine)
 */
void generer_edt_jour(sqlite3 *db, FILE *logs, int id_semaine, int id_jour);


/*!
 * \fn void generer_edt_semaine(sqlite3 *db, FILE *logs, int id_semaine)
 * \brief Planifie tous les jours d'une semaine
 *
 * Appelle generer_edt_jour pour tous les jours d'une semaine donnée.
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
 * \param id_semaine L'identifiant d'une semaine dans la base de données
*/
void generer_edt_semaine(sqlite3 *db, FILE *logs, int id_semaine);


/*!
 * \fn void nettoyer_edt_semaine(sqlite3 *db, FILE *logs, int id_semaine);
 * \brief Supprime la planification d'une semaine
 *
 * Retire chaque entrée concernant la semaine donnée de la table travaille
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
 * \param id_semaine L'identifiant d'une semaine dans la base de données
 */
void nettoyer_edt_semaine(sqlite3 *db, FILE *logs, int id_semaine);

/*!
 * \fn void maj_edt_semaine(sqlite3 *db, FILE *logs, int id_semaine)
 * \brief Nettoie puis régénère la planification d'une semaine
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
 * \param id_semaine L'identifiant d'une semaine dans la base de données
 */
void maj_edt_semaine(sqlite3 *db, FILE *logs, int id_semaine);


/*!
 * \fn void nettoyer_tous_edt(sqlite3 *db, FILE *logs)
 * \brief Nettoie la planification de toutes les semaines
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
 */
void nettoyer_tous_edt(sqlite3 *db, FILE *logs);


/*!
 * \fn void generer_tous_edt(sqlite3 *db, FILE *logs)
 * \brief Génère la planification de toutes les semaines
 *
 * Récupère l'ensemble des semaines présentes dans la bdd et génère pour chacune
 * sa planification
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
*/
void generer_tous_edt(sqlite3 *db, FILE *logs);


void creation_table_edt_ligne_semaine(sqlite3 *db, FILE *logs, edt e, int id_semaine, int mois);

void creation_table_edt_ligne_semaine_avec_id(sqlite3 *db, FILE *logs, edt e, int id_semaine, int mois, int id_ligne);

#endif
