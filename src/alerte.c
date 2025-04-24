#include "../include/types.h"
#include <stdio.h>
#include <stdlib.h>

void alerte_disponibilite(char *specialite, int id_jour, int id_semaine, int id_creneau){
  printf("Alerte : pas de %s disponible le jour %d de la semaine %d au créneau %d\n", specialite, id_jour, id_semaine, id_creneau);
}