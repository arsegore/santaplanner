#ifndef _EXPORT_H_
#define _EXPORT_H_

#include <stdio.h>
#include "types.h"

/*! 
 * \file export.h
 * 
 * \brief Fichier d'en-tête pour l'exportation de l'EDT au format iCalendar
 */

 /*!
 * \fn void exporter_edt_ligne_en_ical(edt e, const char *chemin_fichier, int semaine, int mois, int annee)
 * \brief Exporte l'EDT d'une ligne au format iCalendar
 *
 * \param e L'EDT à exporter
 * \param chemin_fichier Le chemin du fichier de sortie
 * \param semaine Le numéro de la semaine
 * \param mois Le mois de l'année
 * \param annee L'année
 */
void exporter_edt_ligne_en_ical(edt e, const char *chemin_fichier, int semaine, int mois, int annee);

/*!
 * \fn void exporter_edt_lutin_en_ical(edt e, const char *chemin_fichier, int semaine, int mois, int annee)
 * \brief Exporte l'EDT d'un lutin au format iCalendar
 *
 * \param e L'EDT à exporter
 * \param chemin_fichier Le chemin du fichier de sortie
 * \param semaine Le numéro de la semaine
 * \param mois Le mois de l'année
 * \param annee L'année
 */
void exporter_edt_lutin_en_ical(edt e, const char *chemin_fichier, int semaine, int mois, int annee);

#endif