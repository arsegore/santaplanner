#ifndef _ALERTE_H_
#define _ALERTE_H_

#include <stdio.h>
#include "types.h"

/*! 
 * \file alerte.h
 * 
 * \brief Fichier d'en-tête pour la gestion des alertes de disponibilité
 */


/*!
 * \fn void alerte_disponibilite(char *specialite, int id_jour, int id_semaine, int id_creneau)
 * \brief Alerte de disponibilité
 *
 * \param specialite La spécialité de l'alerte
 * \param id_jour L'identifiant du jour
 * \param id_semaine L'identifiant de la semaine
 * \param id_creneau L'identifiant du créneau
 */
void alerte_disponibilite(char *specialite, int id_jour, int id_semaine, int id_creneau);

/*!
 * \fn void nettoyer_alertes()
 * \brief Nettoie le contenu de alertes.txt
 */
void nettoyer_alertes();

#endif