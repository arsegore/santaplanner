#ifndef _EXPORT_H_
#define _EXPORT_H_

#include <stdio.h>
#include "types.h"

void exporter_edt_ligne_en_ical(edt e, const char *chemin_fichier, int semaine, int mois, int annee);

void exporter_edt_lutin_en_ical(edt e, const char *chemin_fichier, int semaine, int mois, int annee);

#endif