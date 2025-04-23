#ifndef _VAR_H_
#define _VAR_H_

#include "types.h"
#include "sqlite3.h"
#include <stdio.h>

/*!
* \file var.h
* \brief Fichier d'en-tête contenant les variables globales de l'application
*/

/*! 
* \brief Pointeur vers la base de données SQLite
*/
extern sqlite3 *db;

/*!
* \brief Pointeur vers le fichier de logs
*/
extern FILE *logs;

/*!
* \brief Pointeur vers la liste des lutins
*/
extern lutin *liste_lutins; 

/*!
* \brief Nombre de lutins dans la liste
*/
extern int nb_lutins;

#endif
