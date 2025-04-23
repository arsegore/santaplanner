#ifndef _UI_GTK_H_  
#define _UI_GTK_H_

#include <gtk/gtk.h>

/* Fichier d'en-tête de l'interface GTK de Santaplanner
 *
 * Ce fichier contient les déclarations des fonctions et structures utilisées pour l'interface graphique
 * de l'application Santaplanner.
 */

 /*!
    \fn void ouverture_fenetre(GtkApplication* app, gpointer u_donnees)
    \brief Ouvre la fenêtre principale de l'application
    \param app L'application GTK
    \param u_donnees Pointeur vers les données de l'application
    \return void
 */
void ouverture_fenetre(GtkApplication* app, gpointer u_donnees);

/*!
    \fn void demarrage_appli(int argc, char **argv)
    \brief Démarre l'application GTK
    \param argc Nombre d'arguments de la ligne de commande
    \param argv Tableau des arguments de la ligne de commande
    \return void
 */
void demarrage_appli(int argc, char **argv);

#endif
