#ifndef _TYPES_H_ 
#define _TYPES_H_

#include <gtk/gtk.h>
#include "liste_chainee.h"

/*!
 * \file types.h
 *
 * \brief Fichier d'en-tête contenant les définitions de types utilisés dans l'application
 */

/*!
  * \brief Une structure modelisant un lutin 
  */
typedef struct{
  int id;
  char nom[50];
  char specialite[50];
}lutin;

/*!
 * \brief Une structure modelisant un EDT
 * 
 * \details Chaque cellule de l'EDT est une liste chaînée, permettant de contenir plusieurs valeurs pour un même créneau
*/
typedef liste_chainee edt[20][7];

/*!
 * \brief Une structure contenant un EDT
 * 
 * \details Permet de passer l'EDT en paramètre à des fonctions GTK
*/
typedef struct{
  edt edt_tab;
}edt_wrp;

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

/*!
 * \brief Une structure contenant les données de l'application
*/
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
    edt_wrp *edt_tab;
    gboolean est_edt_lutin;
    GtkWidget *entry_nom_lutin;
    GtkWidget *combo_specialite;
    GtkWidget *btn_ajouter_lutin;
    GtkWidget *scrolled_window_donnees;
    GtkWidget *combo_creneaux; 
    gboolean est_absence;
} AppData;

/*!
 * \brief Une structure contenant les données de l'application combinées à un id de lutin
*/
typedef struct{
    AppData *app_data; 
    int id_lutin; 
}AppDataLtn;

#endif
