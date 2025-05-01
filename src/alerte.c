#include "../include/types.h"
#include <stdio.h>
#include <stdlib.h>

void alerte_disponibilite(char *specialite, int id_jour, int id_semaine, int id_creneau){
  const char jour[7][10] = {"lundi", "mardi", "mercredi", "jeudi", "vendredi", "samedi", "dimanche"};
  const char creneau[15][10] = {"8h-9h", "9h-10h", "10h-11h", "11h-12h", "12h-13h", "13h-14h", "14h-15h", "15h-16h", "16h-17h", "17h-18h", "18h-19h", "19h-20h", "20h-21h", "21h-22h"};
  FILE *alerte;
  alerte = fopen("alertes.txt", "a");
  if (alerte == NULL){
    fprintf(stderr, "Erreur d'ouverture du fichier d'alerte\n");
    return;
  }
  fprintf(alerte, "Alerte : pas de %s disponible le %s de la semaine %d entre %s\n", specialite, jour[id_jour - 1], id_semaine, creneau[id_creneau - 1]);
  fclose(alerte);
  return; 
}