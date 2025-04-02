#ifndef LISTE_CHAINEE_H
#define LISTE_CHAINEE_H

typedef struct cellule{
  int valeur;
  struct cellule *precedent, *suivant;
}cellule;
typedef cellule *liste_chainee;
typedef liste_chainee pile;
typedef liste_chainee file;

int est_vide(liste_chainee l);
int tete(liste_chainee l);
liste_chainee liste_vide();
liste_chainee supprimer_tete(liste_chainee l);
liste_chainee inserer_tete(liste_chainee l, int val);
liste_chainee inserer_tri(liste_chainee l, int val);
liste_chainee inserer_fin(liste_chainee l, int val);
liste_chainee supprimer(liste_chainee l, int val);
void afficher_liste(liste_chainee l);
void liberer_liste(liste_chainee l);
pile pile_vide();
int pile_est_vide(pile p);
pile empiler(pile p, int val);
pile depiler(pile p);
int sdp(pile p);
void afficher_pile(pile p);
void liberer_pile(pile p);
file file_vide();
int file_est_vide(file f);
file defiler(file f);
int debut(file f);
file enfiler(file f, int val);
void afficher_file(file f);
void liberer_file(file f);

#endif 
