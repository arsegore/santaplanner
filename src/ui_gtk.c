#include <gtk/gtk.h>
#include "../include/generation.h"
#include "../include/var.h"
#include "../include/requete.h"

/* globalement toutes les fonctions se ressemblent donc jv pas tout commenter : déclarations -> creation des objets de l'ui -> 
 * -> transmission dans app_data -> placement dans l'ui */


 /* crée le widget contenant l'edt (ansi que les jours et les créneaux)
    je mets le tout dans une scrolled window, comme ça si les cellules s'étendent et bah le logiciel
    va pas s'agrandir jusqu'à sortir de l'écran
    */
 GtkWidget *creer_edt_widget(edt e, gboolean afficher_noms) {
    GtkWidget *grid, *textview, *scrolled_window, *label;
    GtkTextBuffer *buffer;
    liste_chainee l;
    cellule *curr;
    char contenu[256];
    char ligne[16];
    int i, j, id;
    const char *jours[] = {"Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi"};
    const char *horaires[] = {
        "08:00 - 09:00", "09:00 - 10:00", "10:00 - 11:00",
        "11:00 - 12:00", "12:00 - 13:00", "13:00 - 14:00",
        "14:00 - 15:00", "15:00 - 16:00", "16:00 - 17:00",
        "17:00 - 18:00", "18:00 - 19:00", "19:00 - 20:00",
        "20:00 - 21:00", "21:00 - 22:00"
    };

    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css_provider,
        ".cellule-edt { border: 1px solid black; padding: 10px; font-size: 14px; text-align: center; background-color: #f9f9f9; }"
        ".cellule-edt-plein { background-color: #d1f7d1; }"
        ".cellule-edt-vide { background-color: #f7d1d1; }"
        ".titre-jour, .titre-horaire { font-weight: bold; background-color: #eeeeee; padding: 6px; border: 1px solid #ccc; }", -1);
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    grid = gtk_grid_new();
    gtk_widget_set_hexpand(grid, TRUE);
    gtk_widget_set_vexpand(grid, TRUE);

    /* le coin vide */
    label = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    /* on gere d'abord la premiere ligne : contient les jours*/
    for (j = 0; j < 5; j++) {
        label = gtk_label_new(jours[j]);
        gtk_widget_add_css_class(label, "titre-jour");
        gtk_grid_attach(GTK_GRID(grid), label, j + 1, 0, 1, 1);
    }

    /* puis la premiere colonne : contient les créneaux*/
    for (i = 0; i < 14; i++) {
        label = gtk_label_new(horaires[i]);
        gtk_widget_add_css_class(label, "titre-horaire");
        gtk_grid_attach(GTK_GRID(grid), label, 0, i + 1, 1, 1);
    }

    /* et mtn l'affichage du contenu de l'edt*/
    for (i = 0; i < 14; i++) {
        for (j = 0; j < 5; j++) {
            /* on crée une nouvelle text-view et on lui applique une classe css*/
            textview = gtk_text_view_new();
            buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
            gtk_widget_set_hexpand(textview, TRUE);
            gtk_widget_set_vexpand(textview, TRUE);
            gtk_widget_add_css_class(textview, "cellule-edt");

            l = e[i][j];
            if (est_vide(l)) { /* si la cellule est vide */
                gtk_text_buffer_set_text(buffer, "\n\n", -1);
                gtk_widget_add_css_class(textview, "cellule-edt-vide");
            } else { /* sinon on génère une chaine avec le contenu*/
                contenu[0] = '\0';
                curr = l;
                while (curr != NULL) {
                    if (afficher_noms) {
                        id = curr->valeur;
                        strncat(contenu, recup_nom_lutin(id), sizeof(contenu) - strlen(contenu) - 1);
                    } else {
                        snprintf(ligne, sizeof(ligne), "%d\n", curr->valeur);
                        strncat(contenu, ligne, sizeof(contenu) - strlen(contenu) - 1);
                    }
                    strncat(contenu, "\n", sizeof(contenu) - strlen(contenu) - 1);
                    curr = curr->suivant;
                }
                /* et on met cette chaine dans un buffer*/
                gtk_text_buffer_set_text(buffer, contenu, -1);
                gtk_widget_add_css_class(textview, "cellule-edt-plein");
            }

            gtk_text_view_set_justification(GTK_TEXT_VIEW(textview), GTK_JUSTIFY_CENTER);
            gtk_grid_attach(GTK_GRID(grid), textview, j + 1, i + 1, 1, 1);
        }
    }

    /* on met le tout dans une scrolled window pour éviter que la fenetre grandisse jusqu'à 
    *  dépasser de l'écran*/
    scrolled_window = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), grid);

    return scrolled_window;
}


/* met à jour les données avec un nouvel edt de lignes correspondant à ce qu'à demandé l'utilsateur */
void mettre_a_jour_edt_ligne(GtkWidget *button, gpointer data) {
    AppData *app_data = (AppData *)data;  
    const gchar *lutin_id;
    int semaine, mois, annee, id_ligne;
    edt nouvel_edt;
    GtkWidget *nouvel_edt_widget;

    /* on recupere les input de l'utilisateur */
    semaine = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_data->entry_semaine));
    mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_data->entry_mois));
    annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_data->entry_annee));
    id_ligne = gtk_combo_box_get_active(GTK_COMBO_BOX(app_data->combo_ligne));

    /* on genere l'edt correspondant */
    if (id_ligne == 0){
        creation_table_edt_ligne_semaine(db, logs, nouvel_edt, semaine, mois, annee);
        nouvel_edt_widget = creer_edt_widget(nouvel_edt, FALSE);

    }else{
        creation_table_edt_ligne_semaine_avec_id(db, logs, nouvel_edt, semaine, mois, annee, id_ligne);
        nouvel_edt_widget = creer_edt_widget(nouvel_edt, TRUE);

    }

    /* on retire l'ancien edt affiché et le remplace par celui qui vient d'etre généré */
    gtk_box_remove(GTK_BOX(app_data->box_principale), app_data->edt);
    gtk_box_append(GTK_BOX(app_data->box_principale), nouvel_edt_widget);

    /* on met à jour la structure des données */
    app_data->edt = nouvel_edt_widget;

}

/* idem mais pour les edt des lutins */
void mettre_a_jour_edt_lutin(GtkWidget *button, gpointer data) {
    AppData *app_data = (AppData *)data;
    int semaine, mois, annee, id_combo, id_lutin;
    edt nouvel_edt;
    GtkWidget *nouvel_edt_widget;

    /* on recup les inputs */
    semaine = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_data->entry_semaine));
    mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_data->entry_mois));
    annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_data->entry_annee));
    id_combo = gtk_combo_box_get_active(GTK_COMBO_BOX(app_data->combo_lutins));

    /* on verif que l'utilisateur a bien choisi un lutin */
    if (id_combo < 0 || id_combo >= nb_lutins) {
        fprintf(stderr, "Aucun lutin sélectionné ou index invalide\n");
        return;
    }
    id_lutin = liste_lutins[id_combo].id;
    printf("id_lutin sélectionné : %d\n", id_lutin);

    /* on recrée l'edt */
    creation_table_edt_lutin_semaine(db, logs, nouvel_edt, semaine, mois, annee, id_lutin);
    nouvel_edt_widget = creer_edt_widget(nouvel_edt, FALSE);

    /* on remplace l'ancien edt */
    gtk_box_remove(GTK_BOX(app_data->box_principale), app_data->edt);
    gtk_box_append(GTK_BOX(app_data->box_principale), nouvel_edt_widget);
    app_data->edt = nouvel_edt_widget;
}

/* gère l'affichage de l'onglet "Lignes", donc notamment les différents boutons que 
l'utilisateur peut utiliser pr selectionner un edt à afficher */
void afficher_menu_lignes(AppData *app_data) {
    GtkWidget *box_principale;
    GtkWidget *header_bar;
    GtkWidget *entry_semaine;
    GtkWidget *entry_mois;
    GtkWidget *entry_annee;
    GtkWidget *combo_ligne;
    GtkWidget *btn_confirmer;
    GtkWidget *edt;
    GtkWidget *label_semaine, *label_mois, *label_annee;

    /* on affecte la "boite" (qui stocke l'ui) à celle pointée dans appdata, donc on dessine l'ui dans la boite transmise par 
     * la fonction appelante */
    box_principale = app_data->box_principale;

    /* creation de la barre d'en-tete où l'utilisateur va faire ses inputs */
    header_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    /* qlq labels pr rendre la barre plus lisible */
    label_semaine = gtk_label_new("Semaine : ");
    label_mois = gtk_label_new("Mois :");
    label_annee = gtk_label_new("Année : ");

    /* pr entrer le numero de la semaine */
    entry_semaine = gtk_spin_button_new_with_range(1, 4, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(entry_semaine), 1);
    app_data->entry_semaine = entry_semaine;

    /* pr entrer le numero du mois */
    entry_mois = gtk_spin_button_new_with_range(1, 12, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(entry_mois), 12);
    app_data->entry_mois = entry_mois;

    /* pr entre l'année */
    entry_annee = gtk_spin_button_new_with_range(2023, 2100, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(entry_annee), 2025);
    app_data->entry_annee = entry_annee;


    /* comme on se limite à 5 lignes on va faire une ptite liste déroulante, c plus beau mdrrr */ 
    combo_ligne = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_ligne), "Toutes");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_ligne), "Ligne 1");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_ligne), "Ligne 2");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_ligne), "Ligne 3");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_ligne), "Ligne 4");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_ligne), "Ligne 5");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_ligne), 0);
    app_data->combo_ligne = combo_ligne;

    /* un bouton pour confirmer (qui appellera la fonction qui met à jour l'affichage) */
    btn_confirmer = gtk_button_new_with_label("Confirmer");

    /* une nouvelle box qui contiendra l'affichage de l'edt et on la transmet via l'appdata */
    edt = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    app_data->edt = edt;

    g_signal_connect(btn_confirmer, "clicked", G_CALLBACK(mettre_a_jour_edt_ligne), app_data);

    gtk_box_append(GTK_BOX(header_bar), label_semaine);
    gtk_box_append(GTK_BOX(header_bar), entry_semaine);
    gtk_box_append(GTK_BOX(header_bar), label_mois);
    gtk_box_append(GTK_BOX(header_bar), entry_mois);
    gtk_box_append(GTK_BOX(header_bar), label_annee);
    gtk_box_append(GTK_BOX(header_bar), entry_annee);
    gtk_box_append(GTK_BOX(header_bar), combo_ligne);
    gtk_box_append(GTK_BOX(header_bar), btn_confirmer);

    gtk_box_append(GTK_BOX(box_principale), header_bar);
    gtk_box_append(GTK_BOX(box_principale), edt);

}

/* juste une petite fonction qui remplit un combo (liste déroulante mais askip ça s'appelle 
comme ça dans gtk) avec les noms des lutins, qui sont je le rappelle stockés dans une var globale*/
void remplit_combo_lutins(AppData *app_data){
    GtkWidget *combo_lutins;

    if (app_data->combo_lutins != NULL){
        gtk_widget_unparent(app_data->combo_lutins);
    }

    combo_lutins = gtk_combo_box_text_new();
    for (int i = 0; i < nb_lutins; i++) {
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_lutins), NULL, liste_lutins[i].nom);
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_lutins), 0);
    app_data->combo_lutins = combo_lutins;
}

/* bon pareil que pour les lignes mais c'est l'onglet "Lutins"*/
void afficher_menu_lutins(AppData *app_data) {
    GtkWidget *box_principale;
    GtkWidget *header_bar;
    GtkWidget *entry_semaine;
    GtkWidget *entry_mois;
    GtkWidget *entry_annee;
    GtkWidget *combo_lutins;
    GtkWidget *btn_confirmer;
    GtkWidget *edt;
    GtkWidget *label_semaine, *label_mois, *label_annee, *label_lutin;

    box_principale = app_data->box_principale;

    header_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    label_semaine = gtk_label_new("Semaine : ");
    label_mois = gtk_label_new("Mois :");
    label_annee = gtk_label_new("Année :");
    label_lutin = gtk_label_new("Lutin :");

    entry_semaine = gtk_spin_button_new_with_range(1, 4, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(entry_semaine), 1);
    app_data->entry_semaine = entry_semaine;

    entry_mois = gtk_spin_button_new_with_range(1, 12, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(entry_mois), 12);
    app_data->entry_mois = entry_mois;

    entry_annee = gtk_spin_button_new_with_range(2023, 2100, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(entry_annee), 2025);
    app_data->entry_annee = entry_annee;

    remplit_combo_lutins(app_data);

    
    btn_confirmer = gtk_button_new_with_label("Confirmer");

    edt = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    app_data->edt = edt;

    g_signal_connect(btn_confirmer, "clicked", G_CALLBACK(mettre_a_jour_edt_lutin), app_data);

    gtk_box_append(GTK_BOX(header_bar), label_semaine);
    gtk_box_append(GTK_BOX(header_bar), app_data->entry_semaine);
    gtk_box_append(GTK_BOX(header_bar), label_mois);
    gtk_box_append(GTK_BOX(header_bar), app_data->entry_mois);
    gtk_box_append(GTK_BOX(header_bar), label_annee);
    gtk_box_append(GTK_BOX(header_bar), app_data->entry_annee);
    gtk_box_append(GTK_BOX(header_bar), label_lutin);
    gtk_box_append(GTK_BOX(header_bar), app_data->combo_lutins);
    gtk_box_append(GTK_BOX(header_bar), btn_confirmer);

    
    gtk_box_append(GTK_BOX(box_principale), header_bar);
    gtk_box_append(GTK_BOX(box_principale), edt);
}

/* fonction qui supprime un lutin de la bdd et met à jour ce qui doit l'etre */
void supprimer_lutin(GtkWidget *button, gpointer data){
    AppDataLtn *app_data_ltn = (AppDataLtn *) data;
    table_resultat *t; 
    int id_lutin; 

    id_lutin = app_data_ltn->id_lutin;
    t = requete_supprimer_lutin(db, logs, id_lutin);
    if (t != NULL){
        printf("suppression réussie\n"); 
    }
    liberer_resultats(t);
    
    maj_liste_lutins();
    nettoyer_tous_edt(db, logs);
    generer_tous_edt(db,logs);
    afficher_grid_donnees(app_data_ltn->app_data);
}

/* ajoute une nouvelle absence dans la bdd (pr rappel, inserer une absence = supprimer une dispo)
et régénère ensuite les edt*/
void ajouter_absence_ou_dispo(GtkWidget *button, gpointer data, gboolean est_absence){
    AppDataLtn *app_data_ltn = (AppDataLtn *)data;
    AppData *app_data = app_data_ltn->app_data;
    int jour, semaine, mois, annee; 
    int id_creneau, id_combo, id_lutin, id_semaine;
    table_resultat *t;
    GtkWidget *label_res;

    /* on recup les inputs */
    id_lutin = app_data_ltn->id_lutin;
    jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_data->entry_jour));
    semaine = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_data->entry_semaine));
    mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_data->entry_mois));
    annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_data->entry_annee));
    id_semaine = semaine2id(db, logs, semaine, mois, annee);
    id_creneau = gtk_combo_box_get_active(GTK_COMBO_BOX(app_data->combo_creneaux)) + 1;
    if (est_absence){
        t = requete_inscrire_absence(db, logs, id_lutin, jour, id_semaine, id_creneau);
    }else{
        t = requete_inscrire_dispo(db, logs, id_lutin, jour, id_semaine, id_creneau);
    }
    /* on verifie que l'insertion s'est bien passée */
    if (t != NULL){
        label_res = gtk_label_new("Insertion reussie.");
    }else{
        label_res = gtk_label_new("Echec de l'insertion.");
    }
    maj_edt_semaine(db, logs, id_semaine);
    printf("mise à jour de l'edt de la semaine %d, du mois %d, l'année %d...\n", semaine, mois, annee);
    gtk_box_append(GTK_BOX(app_data->box_principale), label_res); 

    liberer_resultats(t);
    return;

}

void ajouter_absence(GtkWidget *button, gpointer data) {
    AppDataLtn *app_data_ltn = (AppDataLtn *)data;
    ajouter_absence_ou_dispo(button, app_data_ltn, TRUE);
}

void ajouter_dispo(GtkWidget *button, gpointer data) {
    AppDataLtn *app_data_ltn = (AppDataLtn *)data;
    ajouter_absence_ou_dispo(button, app_data_ltn, FALSE);
}

/* affiche le menu d'ajout d'absence/disponibilité (qui est une fentre pop up grossomodo)*/
void afficher_menu_ajouter_absence_ou_dispo(GtkWidget *button, gpointer data) {
    AppDataLtn *app_data_ltn_old = (AppDataLtn *)data; 
    gboolean est_absence = app_data_ltn_old->app_data->est_absence;
    AppDataLtn *app_data_ltn;
    AppData *app_data_absence;
    GtkWidget *fenetre_absence;
    GtkWidget *box_principale;
    GtkWidget *header_bar;
    GtkWidget *combo_creneaux;
    GtkWidget *entry_jour;
    GtkWidget *entry_semaine;
    GtkWidget *entry_mois;
    GtkWidget *entry_annee;
    GtkWidget *btn_confirmer;
    GtkWidget *label_jour, *label_annee, *label_semaine, *label_mois, *label_creneau;

    
    /* on alloue des nouvelles structures appdata qu'on va combiner pr les transmettre à ajouter_absence */
    app_data_absence = g_new0(AppData, 1);
    app_data_ltn = g_new0(AppDataLtn, 1);
    
    fenetre_absence = gtk_window_new();
    if (est_absence){
        gtk_window_set_title(GTK_WINDOW(fenetre_absence), "Absence");
    }else{
        gtk_window_set_title(GTK_WINDOW(fenetre_absence), "Disponibilité");
    }
    gtk_window_set_default_size(GTK_WINDOW(fenetre_absence), 550, 100);
    app_data_absence->fenetre = fenetre_absence;

    box_principale = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(fenetre_absence), box_principale);
    app_data_absence->box_principale = box_principale;

    header_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    label_creneau = gtk_label_new("Créneau : ");
    label_jour = gtk_label_new("Jour : ");
    label_semaine = gtk_label_new("Semaine : ");
    label_mois = gtk_label_new("Mois :");
    label_annee = gtk_label_new("Annee : ");

    combo_creneaux = gtk_combo_box_text_new();
    for (int i = 8; i < 22; i++) {
        gchar *creneau = g_strdup_printf("%02d:00-%02d:00", i, i + 1);
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_creneaux), NULL, creneau);
        g_free(creneau);
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_creneaux), 0);
    app_data_absence->combo_creneaux = combo_creneaux;

    entry_semaine = gtk_spin_button_new_with_range(1, 4, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(entry_semaine), 1);
    app_data_absence->entry_semaine = entry_semaine;

    entry_jour = gtk_spin_button_new_with_range(1, 31, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(entry_jour), 1);
    app_data_absence->entry_jour = entry_jour;

    entry_mois = gtk_spin_button_new_with_range(1, 12, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(entry_mois), 1);
    app_data_absence->entry_mois = entry_mois;

    entry_annee = gtk_spin_button_new_with_range(2023, 2100, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(entry_annee), 2025);
    app_data_absence->entry_annee = entry_annee;

    btn_confirmer = gtk_button_new_with_label("Confirmer");
    app_data_ltn->id_lutin = app_data_ltn_old->id_lutin; 
    app_data_ltn->app_data = app_data_absence;

    if (est_absence){
        g_signal_connect(btn_confirmer, "clicked", G_CALLBACK(ajouter_absence), app_data_ltn);
    }else{
        g_signal_connect(btn_confirmer, "clicked", G_CALLBACK(ajouter_dispo), app_data_ltn);
    }

    gtk_box_append(GTK_BOX(header_bar), label_creneau);
    gtk_box_append(GTK_BOX(header_bar), combo_creneaux);
    gtk_box_append(GTK_BOX(header_bar), label_jour);
    gtk_box_append(GTK_BOX(header_bar), entry_jour);
    gtk_box_append(GTK_BOX(header_bar), label_semaine);
    gtk_box_append(GTK_BOX(header_bar), entry_semaine);
    gtk_box_append(GTK_BOX(header_bar), label_mois);
    gtk_box_append(GTK_BOX(header_bar), entry_mois);
    gtk_box_append(GTK_BOX(header_bar), label_annee);
    gtk_box_append(GTK_BOX(header_bar), entry_annee);
    gtk_box_append(GTK_BOX(header_bar), btn_confirmer);

    gtk_box_append(GTK_BOX(box_principale), header_bar);

    gtk_window_present(GTK_WINDOW(fenetre_absence));
}

/* s'occupe de l'affichage de la grid contenant les lutins et les boutons pr interagir avec la bdd*/
void afficher_grid_donnees(AppData *app_data){
    GtkWidget *grid_donnees;
    GtkWidget *btn_declarer_disponibilite;
    GtkWidget *btn_inserer_absence;
    GtkWidget *btn_inserer_dispo;
    GtkWidget *btn_supprimer_lutin;
    GtkWidget *scrolled_window;
    AppDataLtn *app_data_ltn;


    /* creation d'une fenetre coulissante pr éviter que la fenetre sorte de l'écran  */ 
    scrolled_window = gtk_scrolled_window_new();
    gtk_scrolled_window_set_max_content_height(GTK_SCROLLED_WINDOW(scrolled_window), 900);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scrolled_window), 720);
    
    grid_donnees = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(grid_donnees), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid_donnees), TRUE);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), grid_donnees); 

    for (int i = 0; i < nb_lutins; i++) {

        app_data_ltn = g_new0(AppDataLtn, 1);
        app_data_ltn->app_data = app_data;
        app_data_ltn->id_lutin = liste_lutins[i].id;
        printf("app_data_ltn->id_lutin = %d\n", app_data_ltn->id_lutin);

        /* bouton pr inserer une nouvelle absence à ce lutin */
        btn_inserer_absence = gtk_button_new_with_label("Déclarer une absence");
        g_object_set_data(G_OBJECT(btn_inserer_absence), "lutin_id", GINT_TO_POINTER(app_data_ltn->id_lutin));
        g_signal_connect(btn_inserer_absence, "clicked", G_CALLBACK(afficher_menu_ajouter_absence_ou_dispo), app_data_ltn);

        /* bouton pr inserer une nouvelle disponibilité à ce lutin */
        btn_inserer_dispo = gtk_button_new_with_label("Déclarer une disponibilité");
        g_object_set_data(G_OBJECT(btn_inserer_dispo), "lutin_id", GINT_TO_POINTER(app_data_ltn->id_lutin));
        g_signal_connect(btn_inserer_dispo, "clicked", G_CALLBACK(afficher_menu_ajouter_absence_ou_dispo), app_data_ltn);

        /* bouton pr supprimer ce lutin de la bdd */
        btn_supprimer_lutin = gtk_button_new_with_label("Supprimer");
        g_object_set_data(G_OBJECT(btn_supprimer_lutin), "lutin_id", GINT_TO_POINTER(app_data_ltn->id_lutin));
        g_signal_connect(btn_supprimer_lutin, "clicked", G_CALLBACK(supprimer_lutin), app_data_ltn);

        gtk_grid_attach(GTK_GRID(grid_donnees), gtk_label_new(liste_lutins[i].nom), 0, i, 1, 1);
        gtk_grid_attach(GTK_GRID(grid_donnees), gtk_label_new(liste_lutins[i].specialite), 1, i, 1, 1);
        gtk_grid_attach(GTK_GRID(grid_donnees), btn_inserer_dispo, 2, i, 1, 1);
        gtk_grid_attach(GTK_GRID(grid_donnees), btn_inserer_absence, 3, i, 1, 1);
        gtk_grid_attach(GTK_GRID(grid_donnees), btn_supprimer_lutin, 4, i, 1, 1);
    }

    gtk_box_remove(GTK_BOX(app_data->box_principale), app_data->scrolled_window_donnees);
    app_data->scrolled_window_donnees = scrolled_window;
    gtk_box_append(GTK_BOX(app_data->box_principale), scrolled_window);
}

/* là ça gère l'ajout d'un nouveau lutin dans la bdd*/
void ajouter_lutin(GtkWidget *button, gpointer data) {
    AppData *app_data = (AppData *)data;
    GtkEntryBuffer *nom_buffer;
    GtkWidget *fenetre_erreur;
    GtkWidget *label_erreur;
    const gchar *nom_lutin;
    const gchar *specialite;

    nom_buffer = gtk_entry_get_buffer(GTK_ENTRY(app_data->entry_nom_lutin));
    nom_lutin = gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(nom_buffer));
    specialite = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(app_data->combo_specialite));

    if (nom_lutin && specialite && strlen(nom_lutin) > 0) {
        requete_inscrire_lutin(db, logs, nom_lutin, specialite);
    } else {
        fenetre_erreur = gtk_window_new(); 
        gtk_window_set_title (GTK_WINDOW (fenetre_erreur), "Erreur");
        gtk_window_set_default_size (GTK_WINDOW (fenetre_erreur), 128, 72);
        label_erreur = gtk_label_new("Erreur: des champs sont vides !");
        gtk_window_set_child(GTK_WINDOW(fenetre_erreur), label_erreur);
        gtk_window_present(GTK_WINDOW(fenetre_erreur));

    }

    maj_liste_lutins();
    remplit_combo_lutins(app_data);
    afficher_grid_donnees(app_data);
    return;
}

/* afficahage de l'onglet "Données"*/
void afficher_menu_donnees(AppData *app_data){
    GtkWidget *header_bar;
    GtkWidget *entry_nom_lutin;
    GtkWidget *combo_specialite;
    GtkWidget *btn_ajouter_lutin;
   
    header_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    entry_nom_lutin = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_nom_lutin), "Nom du lutin");
    app_data->entry_nom_lutin = entry_nom_lutin;

    combo_specialite = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_specialite), "Empaqueteur");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_specialite), "Bricoleur");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_specialite), "Controleur");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_specialite), 0);  // Par défaut, Empaqueteur
    app_data->combo_specialite = combo_specialite;

    btn_ajouter_lutin = gtk_button_new_with_label("Ajouter Lutin");
    g_signal_connect(btn_ajouter_lutin, "clicked", G_CALLBACK(ajouter_lutin), app_data);

    gtk_box_append(GTK_BOX(header_bar), entry_nom_lutin);
    gtk_box_append(GTK_BOX(header_bar), combo_specialite);
    gtk_box_append(GTK_BOX(header_bar), btn_ajouter_lutin);

    gtk_box_append(GTK_BOX(app_data->box_principale), header_bar);
    afficher_grid_donnees(app_data);

}

/* affichage du changelog dans l'onglet "Accueil"*/
GtkWidget *afficher_changelog(){
    FILE *f_changelog; 
    GtkWidget *text_view;
    char txt_buffer[2048] = "\0";

    f_changelog = fopen("changelog.txt", "r"); 
    if (f_changelog == NULL){
        fprintf(stderr, "Impossible d'accéder aux changelogs\n");
    }
    fread(txt_buffer, 1, 2048 - 1, f_changelog);
    text_view = gtk_text_view_new();
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, txt_buffer, -1);
    fclose(f_changelog);

    return text_view;
}

/* ça ça s'occupe d'afficher le menu principal, donc en gros le squelette global de la fenetre*/
void afficher_menu_principal(GtkWindow *fenetre) {
    AppData *app_data_lignes;
    AppData *app_data_lutins;
    AppData *app_data_donnees;
    GtkWidget *stack;
    GtkWidget *stack_switcher;
    GtkWidget *changelog;
    GtkWidget *page_accueil;
    GtkWidget *label_bienvenue;
    GtkWidget *page_lignes;
    GtkWidget *page_lutins;
    GtkWidget *page_donnees;
    GtkWidget *container;

    /* on cree une stack (c en grooos une sorte de gestion d'onglets, ça me sert pr gerer les sous menus) */ 
    stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    gtk_stack_set_transition_duration(GTK_STACK(stack), 300);

    /* il faut un stack switcher pr manipuler la stack */
    stack_switcher = gtk_stack_switcher_new();
    gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(stack_switcher), GTK_STACK(stack));

    /* création de la page d'accueil */
    changelog = afficher_changelog();
    page_accueil = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    label_bienvenue = gtk_label_new("🎅 Bienvenue sur Santaplanner !");
    gtk_widget_set_halign(label_bienvenue, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(label_bienvenue, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(page_accueil), label_bienvenue);
    gtk_box_append(GTK_BOX(page_accueil), changelog);
    gtk_stack_add_titled(GTK_STACK(stack), page_accueil, "accueil", "Accueil");

    /* création de la page des lignes : affichage des edt globaux de la prod */ 
    page_lignes = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    app_data_lignes = g_new0(AppData, 1);
    app_data_lignes->fenetre = fenetre;
    app_data_lignes->box_principale = page_lignes;
    afficher_menu_lignes(app_data_lignes);
    gtk_stack_add_titled(GTK_STACK(stack), page_lignes, "lignes", "Lignes");

    /* création de la page des lutins : affichage des edt spécifiques d'un lutin*/
    page_lutins = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    app_data_lutins = g_new0(AppData, 1);
    app_data_lutins->fenetre = fenetre;
    app_data_lutins->box_principale = page_lutins; 
    afficher_menu_lutins(app_data_lutins);
    gtk_stack_add_titled(GTK_STACK(stack), page_lutins, "lutins", "Lutins");

    /* création de la page des données : pr interagir avec la bdd, donc ajouter/supprimer des lutins ou des absences*/ 
    page_donnees = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    app_data_donnees = g_new0(AppData, 1);
    app_data_donnees->fenetre = fenetre; 
    app_data_donnees->box_principale = page_donnees;
    afficher_menu_donnees(app_data_donnees);
    gtk_stack_add_titled(GTK_STACK(stack), page_donnees, "donnees", "Donnees");

    /* box dans laquelle l'ui est dessiné */
    container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(container), stack_switcher);
    gtk_box_append(GTK_BOX(container), stack);

    /* on connecte la box à la fenetre */
    gtk_window_set_child(fenetre, container);
}

/* ouvre la fenetre */
void ouverture_fenetre(GtkApplication* app){
    GtkWidget *fenetre;

    fenetre = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (fenetre), "Santaplanner");
    gtk_window_set_default_size (GTK_WINDOW (fenetre), 1280, 720);
    afficher_menu_principal(GTK_WINDOW(fenetre));

    gtk_window_present (GTK_WINDOW (fenetre));
}

/* lance Santaplanner*/
int demarrage_appli(int argc, char **argv){
    GtkApplication *app;
    int status;  

    remplir_liste_lutins();
    afficher_liste_lutins();
    app = gtk_application_new ("org.santaplanner", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (ouverture_fenetre), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}






