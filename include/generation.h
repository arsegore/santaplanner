#ifndef GENERATION_H_
#define GENERATION_H_

#include "sqlite3.h"
#include "types.h"
#include <stdio.h>

/*!
 * \file generation.h
 *
 * \brief En-tête des fonctions de planification de la ligne de production
*/


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
 * \fn void nettoyer_edt_jour(sqlite3 *db, FILE *logs, int id_semaine, int id_jour)
 * \brief Supprime la planification d'une journée
 *
 * Retire chaque entrée concernant la journée donnée de la table travaille
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
 * \param id_semaine L'identifiant d'une semaine dans la base de données
 * \param id_jour L'identifiant d'un jour dans la base de données (qui correspond en fait au numéro d'un jour dans la semaine)
 */
void nettoyer_edt_jour(sqlite3 *db, FILE *logs, int id_semaine, int id_jour);

/*!
 * \fn generer_edt_jour(sqlite3 *db, FILE *logs, int id_semaine, int id_jour)
 * \brief Nettoie puis régénère la planification d'une journée
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
 * \param id_semaine L'identifiant d'une semaine dans la base de données
 * \param id_jour L'identifiant d'un jour dans la base de données (qui correspond en fait au numéro d'un jour dans la semaine)
 */
void maj_edt_jour(sqlite3 *db, FILE *logs, int id_semaine, int id_jour);


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

/*!
 * \fn void creation_table_edt_ligne_semaine(sqlite3 *db, FILE *logs, edt e, int numero, int mois, int annee)
 * \brief Crée une table de planification pour une ligne donnée
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
 * \param e L'EDT à créer
 * \param numero Le numéro de la ligne
 * \param mois Le mois de la semaine
 * \param annee L'année de la semaine
 */
void creation_table_edt_ligne_semaine(sqlite3 *db, FILE *logs, edt e, int numero, int mois, int annee);

/*!
 * \fn void creation_table_edt_ligne_semaine_avec_id(sqlite3 *db, FILE *logs, edt e, int numero, int mois, int annee, int id_ligne)
 * \brief Crée une table de planification pour une ligne donnée avec un id
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
 * \param e L'EDT à créer
 * \param numero Le numéro de la ligne
 * \param mois Le mois de la semaine
 * \param annee L'année de la semaine
 * \param id_ligne L'id de la ligne
 */
void creation_table_edt_ligne_semaine_avec_id(sqlite3 *db, FILE *logs, edt e, int numero, int mois, int annee, int id_ligne);

/*!
* \fn void remplir_liste_lutins()
 * \brief Remplit la liste des lutins
 *
 * Récupère l'ensemble des lutins présents dans la base de données et les stocke
 * dans un tableau de lutins
 */
void remplir_liste_lutins();

/*!
 *\fn maj_liste_lutins()
 * \brief Met à jour la liste des lutins
 *
 * Récupère l'ensemble des lutins présents dans la base de données et les stocke
 * dans un tableau de lutins
 */
void maj_liste_lutins();

/*!
 * \fn void afficher_liste_lutins()
 * \brief Affiche la liste des lutins
 *
 * Récupère l'ensemble des lutins présents dans la base de données et les affiche
 * dans la console
 */
void afficher_liste_lutins();

/*!
 \fn recup_nom_lutin(int id)
    * \brief Récupère le nom d'un lutin
    *
    * Récupère le nom d'un lutin à partir de son id
    *
    * \param id L'id du lutin
    * \return Le nom du lutin
    */
const char *recup_nom_lutin(int id);

/*!
 * \fn void creation_table_edt_lutin_semaine(sqlite3 *db, FILE *logs, edt e, int numero, int mois, int annee, int id_lutin)
 * \brief Crée une table de planification pour un lutin donné
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
 * \param e L'EDT à créer
 * \param numero Le numéro du lutin
 * \param mois Le mois de la semaine
 * \param annee L'année de la semaine
 * \param id_lutin L'id du lutin
 */
void creation_table_edt_lutin_semaine(sqlite3 *db, FILE *logs, edt e, int numero, int mois, int annee, int id_lutin);

#endif
