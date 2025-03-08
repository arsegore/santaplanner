#ifndef GENERATION_H_
#define GENERATION_H_

#include "sqlite3.h"
#include <stdio.h>

/*
 * Récupère l'identifiant d'une semaine dans la bdd
*/
int semaine2id(sqlite3 *db, FILE *logs, int num_semaine, int mois, int annee);

/*
 * Renvoie le nombre de lignes ouvrables à un horaire donné
 */
int nb_lignes_ouvrables(sqlite3 *db, FILE *logs, int id_semaine, int id_jour, int id_creneau);

/*
 * Met à jour la planification d'une semaine
*/
void maj_edt_semaine(sqlite3 *db, FILE *logs, int id_semaine);

/*
 * Génère toute la planification
*/
void generer_tous_edt(sqlite3 *db, FILE * logs);



#endif // GENERATION_H_
