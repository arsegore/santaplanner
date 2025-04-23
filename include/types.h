#ifndef _TYPES_H_ 
#define _TYPES_H_

#include <gtk/gtk.h>
#include "liste_chainee.h"

typedef struct{
  int id;
  char nom[50];
  char specialite[50];
}lutin;

/* Un edt est un tableau de cellules */
typedef liste_chainee edt[20][7];

/*!
 * \brief Une entrée d'une table de résultats
*/
typedef char requete_valeur[100];

/*!
 * \brief Un tableau de résultats
*/
typedef struct {
  requete_valeur ** valeurs;
  int nb_ligne, nb_col;
}table_resultat;

typedef struct {
    GtkWindow *fenetre;
    GtkWidget *entry_jour;
    GtkWidget *entry_semaine;
    GtkWidget *entry_mois;
    GtkWidget *entry_annee;
    GtkWidget *entry_lutin;
    GtkWidget *combo_ligne;
    GtkWidget *combo_lutins;
    GtkWidget *box_principale;
    GtkWidget *edt;
    GtkWidget *entry_nom_lutin;
    GtkWidget *combo_specialite;
    GtkWidget *btn_ajouter_lutin;
    GtkWidget *scrolled_window_donnees;
    GtkWidget *combo_creneaux; 
    gboolean est_absence;
} AppData;

typedef struct{
    AppData *app_data; 
    int id_lutin; 
}AppDataLtn;

#endif
