#include <gtk/gtk.h>
#include "../include/generation.h"
#include "../include/var.h"
void test(){}

typedef struct {
    GtkWindow *fenetre;
    GtkWidget *entry_semaine;
    GtkWidget *entry_mois;
    GtkWidget *entry_annee;
    GtkWidget *entry_lutin;
    GtkWidget *combo_ligne;
    GtkWidget *box_principale;
    GtkWidget *edt;
} AppData;


GtkWidget *affiche_edt_ligne(edt e) {
    GtkWidget *grid, *textview;
    GtkTextBuffer *buffer;
    liste_chainee l;
    cellule *curr;
    char contenu[256];
    char ligne[16];
    int i, j;

    grid = gtk_grid_new();

    /* gestion du style des cellules */
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css_provider,
        ".cellule-edt { border: 1px solid black; }", -1);
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    /* on parcourt l'edt et remplit le gtkwidget qui sera ensuite affiché */
    for (i = 0; i < 14; i++) {
        for (j = 0; j < 5; j++) {

            /* on cree une textview qui servira à stocker du texte, un buffer en gros */
            textview = gtk_text_view_new();
            gtk_widget_set_hexpand(textview, TRUE);
            buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
            gtk_widget_add_css_class(textview, "cellule-edt");

            /* on recup la liste correspondante à la cellule parcourue */
            l = e[i][j];

            /* si elle est vide on affiche une case vide */
            if (est_vide(l)) {
                gtk_text_buffer_set_text(buffer, "\n\n", -1);
            } else { /* sinon on remplit avec les id_ligne/lutins (selon la requete) */ 
                contenu[0] = '\0'; /* on part d'une chaine vide qu'on remplit au fur et à mesure */ 
                curr = l;
                while (curr != NULL) {
                    snprintf(ligne, sizeof(ligne), "%d\n", curr->valeur);
                    strncat(contenu, ligne, sizeof(contenu) - strlen(contenu) - 1);
                    curr = curr->suivant;
                }
                gtk_text_buffer_set_text(buffer, contenu, -1);
            }

            // Ajout à la grille
            gtk_grid_attach(GTK_GRID(grid), textview, j + 1, i + 1, 1, 1); // Décalage de 1 pour laisser place aux titres
        }
    }

    return grid;
}

GtkWidget *affiche_edt_ligne_noms_lutins(edt e) {
    GtkWidget *grid, *textview;
    GtkTextBuffer *buffer;
    liste_chainee l;
    cellule *curr;
    char contenu[256];
    int i, j, id;

    grid = gtk_grid_new();

    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css_provider,
        ".cellule-edt { border: 1px solid black; }", -1);
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    for (i = 0; i < 14; i++) {
        for (j = 0; j < 5; j++) {

            textview = gtk_text_view_new();
            gtk_widget_set_hexpand(textview, TRUE);
            buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
            gtk_widget_add_css_class(textview, "cellule-edt");

            l = e[i][j];

            if (est_vide(l)) {
                gtk_text_buffer_set_text(buffer, "\n\n", -1);
            } else {
                contenu[0] = '\0';
                curr = l;
                while (curr != NULL) {
                    id = curr->valeur;
                    strncat(contenu, recup_nom_lutin(id), 256 - strlen(contenu) - 1);
                    strncat(contenu, "\n", sizeof(char));
                    curr = curr->suivant;
                }
                gtk_text_buffer_set_text(buffer, contenu, -1);
            }

            gtk_grid_attach(GTK_GRID(grid), textview, j + 1, i + 1, 1, 1);
        }
    }

    return grid;
}

void mettre_en_forme_edt(GtkWidget *grid) {
    const char *jours[] = {"Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi"};
    const char *horaires[] = {
        "08:00 - 09:00", "09:00 - 10:00", "10:00 - 11:00",
        "11:00 - 12:00", "12:00 - 13:00", "13:00 - 14:00",
        "14:00 - 15:00", "15:00 - 16:00", "16:00 - 17:00",
        "17:00 - 18:00", "18:00 - 19:00", "19:00 - 20:00",
        "20:00 - 21:00", "21:00 - 22:00"
    };
    GtkWidget *label;

    /* remplissage de la premiere ligne avec les j de la semaine */
    for (int j = 1; j <= 5; j++) { 
        label = gtk_label_new(jours[j - 1]);
        gtk_widget_add_css_class(label, "titre-jour");
        gtk_grid_attach(GTK_GRID(grid), label, j, 0, 1, 1);
    }

    /* remplissage de la 1ere colonne avec les creneaux */
    for (int i = 1; i <= 14; i++) { 
        label = gtk_label_new(horaires[i - 1]); 
        gtk_widget_add_css_class(label, "titre-horaire");
        gtk_grid_attach(GTK_GRID(grid), label, 0, i, 1, 1);
    }
}


void mettre_a_jour_edt_ligne(GtkWidget *button, gpointer data) {
    AppData *app_data = (AppData *)data;  
    const gchar *lutin_id;
    int semaine, mois, annee, id_ligne;
    edt nouvel_edt;
    GtkWidget *nouvel_edt_widget;

    /* on recupere les input de l'utilisateur */
    // lutin_id = gtk_entry_get_text(GTK_ENTRY(app_data->entry_lutin));
    semaine = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_data->entry_semaine));
    mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_data->entry_mois));
    annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_data->entry_annee));
    id_ligne = gtk_combo_box_get_active(GTK_COMBO_BOX(app_data->combo_ligne));

    /* on genere l'edt correspondant */
    if (id_ligne == 0){
        creation_table_edt_ligne_semaine(db, logs, nouvel_edt, semaine, mois, annee);
        nouvel_edt_widget = affiche_edt_ligne(nouvel_edt);

    }else{
        creation_table_edt_ligne_semaine_avec_id(db, logs, nouvel_edt, semaine, mois, annee, id_ligne);
        nouvel_edt_widget = affiche_edt_ligne_noms_lutins(nouvel_edt);

    }
    /* on cree le gtkwidget correspondant */ 
        mettre_en_forme_edt(nouvel_edt_widget);

    /* on retire l'ancien edt affiché et le remplace par celui qui vient d'etre généré */
    gtk_box_remove(GTK_BOX(app_data->box_principale), app_data->edt);
    gtk_box_append(GTK_BOX(app_data->box_principale), nouvel_edt_widget);

    /* on met à jour la structure des données */
    app_data->edt = nouvel_edt_widget;

}



void afficher_menu_lignes(AppData *app_data) {
    // Déclarations en début de fonction
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


void afficher_menu_principal(GtkWindow *fenetre) {
    AppData *app_data;
    GtkWidget *stack;
    GtkWidget *stack_switcher;
    GtkWidget *page_accueil;
    GtkWidget *label_bienvenue;
    GtkWidget *page_lignes;
    GtkWidget *page_lutins;
    GtkWidget *page_donnees;
    GtkWidget *container;

    app_data = g_new0(AppData, 1);
    app_data->fenetre = fenetre;

    /* on cree une stack (c en grooos une sorte de gestion d'onglets, ça me sert pr gerer les sous menus) */ 
    stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    gtk_stack_set_transition_duration(GTK_STACK(stack), 300);

    /* il faut un stack switcher pr manipuler la stack */
    stack_switcher = gtk_stack_switcher_new();
    gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(stack_switcher), GTK_STACK(stack));

    /* création de la page d'accueil */
    page_accueil = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    label_bienvenue = gtk_label_new("🎅 Bienvenue sur Santaplanner !");
    gtk_widget_set_halign(label_bienvenue, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(label_bienvenue, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(page_accueil), label_bienvenue);
    gtk_stack_add_titled(GTK_STACK(stack), page_accueil, "accueil", "Accueil");

    /* création de la page des lignes : affichage des edt globaux de la prod */ 
    page_lignes = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    app_data->box_principale = page_lignes;
    afficher_menu_lignes(app_data);
    gtk_stack_add_titled(GTK_STACK(stack), page_lignes, "lignes", "Lignes");

    /* création de la page des lutins : affichage des edt spécifiques d'un lutin*/
    page_lutins = gtk_label_new("À venir !");
    gtk_stack_add_titled(GTK_STACK(stack), page_lutins, "lutins", "Lutins");

    /* création de la page des données : pr interagir avec la bdd, donc ajouter/supprimer des lutins ou des absences*/ 
    page_donnees = gtk_label_new("À venir !");
    gtk_stack_add_titled(GTK_STACK(stack), page_donnees, "donnees", "Données");

    /* box dans laquelle l'ui est dessiné */
    container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(container), stack_switcher);
    gtk_box_append(GTK_BOX(container), stack);

    /* on connecte la box à la fenetre */
    gtk_window_set_child(fenetre, container);
}

void ouverture_fenetre(GtkApplication* app){
  GtkWidget *fenetre;

  fenetre = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (fenetre), "Santaplanner");
  gtk_window_set_default_size (GTK_WINDOW (fenetre), 1280, 720);
  afficher_menu_principal(GTK_WINDOW(fenetre));

  gtk_window_present (GTK_WINDOW (fenetre));
}


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






