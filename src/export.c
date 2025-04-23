#include "../include/types.h"
#include "../include/var.h"
#include "../include/generation.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/* un exemple de .ics : https://gist.github.com/DeMarko/6142417 */


/* exporte un edt de ligne en .ics, chaque cellule contient les noms des lutins */
void exporter_edt_ligne_en_ical(edt e, const char *chemin_fichier, int semaine, int mois, int annee){
    FILE *f;
    /* on définit des str représentant les créneaux*/
    const char *heures_debut[14] = {
        "080000", "090000", "100000", "110000", "120000", "130000", "140000",
        "150000", "160000", "170000", "180000", "190000", "200000", "210000"
    };
    const char *heures_fin[14] = {
        "090000", "100000", "110000", "120000", "130000", "140000", "150000",
        "160000", "170000", "180000", "190000", "200000", "210000", "220000"
    };
    int i, j;
    cellule *c;
    /* deux buffers pour la desc et la date...*/
    char description[512];
    char date_str[9];
    struct tm date; /* on se sert d'une structure pr convertir nos données "arbitraires" SQL en une vraie date*/

    /* ouverture du fichier (création éventuellement)*/
    f = fopen(chemin_fichier, "w");
    if (f == NULL){
        fprintf(stderr, "Erreur d'ouverture du fichier %s\n", chemin_fichier);
        return;
    }

    /* écriture au format .ics*/
    fprintf(f, "BEGIN:VCALENDAR\nVERSION:2.0\nPRODID:-//Santaplanner//Ligne//FR\n");
    printf("OUE OUEEEEEE1\n");
    /* on boucle sur chaque créneau pour chaque jour */
    for (i = 0; i < 14; i++) {
        for (j = 0; j < 5; j++) {
            if (!est_vide(e[i][j])) {
                description[0] = '\0';
                c = e[i][j];
                while (c != NULL) {
                    sprintf(description, "%s %s\\n", description, recup_nom_lutin(c->valeur));
                    c = c->suivant;
                }
                date.tm_year = annee - 1900;
                date.tm_mon = mois - 1;
                date.tm_mday = j + (semaine - 1) * 7;
                date.tm_hour = 0;
                date.tm_min = 0;
                date.tm_sec = 0;
                mktime(&date);
                strftime(date_str, sizeof(date_str), "%Y%m%d", &date);
                fprintf(f,
                    "BEGIN:VEVENT\nSUMMARY:Production ligne\nDTSTART;TZID=Europe/Paris:%sT%s\nDTEND;TZID=Europe/Paris:%sT%s\nDESCRIPTION:%s\nEND:VEVENT\n",
                    date_str, heures_debut[i], date_str, heures_fin[i], description);
            }
        }
    }

    fprintf(f, "END:VCALENDAR\n");
    fclose(f);
}

/* exporte un edt de lutin en .ics, chaque cellule contient l'id de la ligne 
    exactemet le meme principe que la fonction précédente */
void exporter_edt_lutin_en_ical(edt e, const char *chemin_fichier, int semaine, int mois, int annee){
    FILE *f;
    const char *heures_debut[14] = {
        "080000", "090000", "100000", "110000", "120000", "130000", "140000",
        "150000", "160000", "170000", "180000", "190000", "200000", "210000"
    };
    const char *heures_fin[14] = {
        "090000", "100000", "110000", "120000", "130000", "140000", "150000",
        "160000", "170000", "180000", "190000", "200000", "210000", "220000"
    };
    int i, j;
    cellule *c;
    char lignes_str[128];
    char date_str[9];
    struct tm date;

    f = fopen(chemin_fichier, "w");
    if (!f) return;

    fprintf(f, "BEGIN:VCALENDAR\nVERSION:2.0\nPRODID:-//Santaplanner//Lutin//FR\n");

    for (i = 0; i < 14; i++) {
        for (j = 0; j < 5; j++) {
            if (!est_vide(e[i][j])) {
                lignes_str[0] = '\0';
                c = e[i][j];
                while (c != NULL) {
                    sprintf (lignes_str, "Ligne %d\\n", c->valeur);

                    c = c->suivant;
                }

                date.tm_year = annee - 1900;
                date.tm_mon = mois - 1;
                date.tm_mday = (j + 1) + (semaine - 1) * 7;
                date.tm_hour = 0;
                date.tm_min = 0;
                date.tm_sec = 0;
                mktime(&date);
                strftime(date_str, sizeof(date_str), "%Y%m%d", &date);

                fprintf(f,
                    "BEGIN:VEVENT\nSUMMARY:Mission du lutin\nDTSTART;TZID=Europe/Paris:%sT%s\nDTEND;TZID=Europe/Paris:%sT%s\nDESCRIPTION:%s\nEND:VEVENT\n",
                    date_str, heures_debut[i], date_str, heures_fin[i], lignes_str);
            }
        }
    }

    fprintf(f, "END:VCALENDAR\n");
    fclose(f);
}
