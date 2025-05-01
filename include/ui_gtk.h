#ifndef UI_GTK_H
#define UI_GTK_H

#include <gtk/gtk.h>
#include "../include/var.h"
#include "../include/generation.h"
#include "../include/requete.h"

/**
 * @brief Crée le widget contenant l'edt.
 */
GtkWidget *creer_edt_widget(edt e, gboolean afficher_noms);

/**
 * @brief Met à jour les données avec un nouvel edt de lignes.
 */
void mettre_a_jour_edt_ligne(GtkWidget *button, gpointer data);

/**
 * @brief Idem mais pour les edt des lutins.
 */
void mettre_a_jour_edt_lutin(GtkWidget *button, gpointer data);

/**
 * @brief Callback appelée quand l'utilisateur choisit un fichier export.
 */
void fichier_export_choisi(GObject *source, GAsyncResult *res, gpointer user_data);

/**
 * @brief Ouvre la fenêtre de choix de fichier pour exporter un edt ligne.
 */
void export_edt_ligne_gtk(GtkWidget *button, gpointer data);

/**
 * @brief Ouvre la fenêtre de choix de fichier pour exporter un edt lutin.
 */
void export_edt_lutin_gtk(GtkWidget *button, gpointer data);

/**
 * @brief Gère l'affichage de l'onglet "Lignes".
 */
void afficher_menu_lignes(AppData *app_data);

/**
 * @brief Remplit le combo lutins.
 */
void remplit_combo_lutins(AppData *app_data);

/**
 * @brief Gère l'affichage de l'onglet "Lutins".
 */
void afficher_menu_lutins(AppData *app_data);

/**
 * @brief Supprime un lutin de la base de données.
 */
void supprimer_lutin(GtkWidget *button, gpointer data);

/**
 * @brief Ajoute une absence ou une disponibilité dans la base de données.
 */
void ajouter_absence_ou_dispo(GtkWidget *button, gpointer data, gboolean est_absence);

/**
 * @brief Wrapper pour ajouter une absence.
 */
void ajouter_absence(GtkWidget *button, gpointer data);

/**
 * @brief Wrapper pour ajouter une disponibilité.
 */
void ajouter_dispo(GtkWidget *button, gpointer data);

/**
 * @brief Affiche le menu d'ajout d'absence ou de disponibilité.
 */
void afficher_menu_ajouter_absence_ou_dispo(GtkWidget *button, gpointer data);

/**
 * @brief Wrapper pour afficher le menu d'ajout d'absence.
 */
void afficher_menu_ajt_absence(GtkWidget *button, gpointer data);

/**
 * @brief Wrapper pour afficher le menu d'ajout de disponibilité.
 */
void afficher_menu_ajt_dispo(GtkWidget *button, gpointer data);

/**
 * @brief Affiche la grille des données.
 */
void afficher_grid_donnees(AppData *app_data);

/**
 * @brief Ajoute un lutin dans la base de données.
 */
void ajouter_lutin(GtkWidget *button, gpointer data);

/**
 * @brief Affiche l'onglet "Données".
 */
void afficher_menu_donnees(AppData *app_data);

/**
 * @brief Affiche le changelog.
 */
GtkWidget *afficher_changelog(void);

/**
 * @brief Affiche les alertes.
 */
void *afficher_alertes(AppData *app_data_alertes);

/**
 * @brief Rafraîchit périodiquement les alertes.
 */
gboolean rafraichir_alertes(gpointer data);

/**
 * @brief Affiche le menu principal.
 */
void afficher_menu_principal(GtkWindow *fenetre);

/**
 * @brief Ouvre la fenêtre principale.
 */
void ouverture_fenetre(GtkApplication* app);

/**
 * @brief Lance l'application.
 */
int demarrage_appli(int argc, char **argv);

#endif /* UI_GTK_H */
