#include "../include/liste_chainee.h"

#include <stdlib.h>
#include <stdio.h>

/****************************************
* LISTES
*****************************************/
int est_vide(liste_chainee l){
  if (l == NULL){
    return 1;
  }
  return 0;
}

int tete(liste_chainee l){
  return l->valeur;
}

liste_chainee liste_vide(){
  return NULL;
}

liste_chainee supprimer_tete(liste_chainee l){
  liste_chainee nouveau_depart;

  if (est_vide(l)){
    return liste_vide();
  }
  nouveau_depart = l->suivant;
  free(l);
  return nouveau_depart;
}

liste_chainee inserer_tete(liste_chainee l, int val){
  liste_chainee lbis;

  lbis = (liste_chainee) malloc(sizeof(cellule));
  if (lbis == NULL){
    fprintf(stderr, "Erreur d'allocation\n");
    return NULL;
  }
  lbis->suivant = l;
  lbis->precedent = NULL;
  lbis->valeur = val;
  return lbis;
}

liste_chainee inserer_tri(liste_chainee l, int val){
  liste_chainee courant = l, lnew;

  if (est_vide(l)){
    inserer_tete(l, val);
  }

  lnew = liste_vide();
  while (tete(courant->suivant) < val && !est_vide(courant->suivant)){
    courant = courant->suivant;
  }
  lnew = inserer_tete(lnew, val);
  lnew->precedent = courant;
  lnew->suivant = courant->suivant;
  courant->suivant = lnew;

  return l;
}

liste_chainee inserer_fin(liste_chainee l, int val){
  liste_chainee courant = l, lnew = NULL;

  lnew = inserer_tete(lnew, val);
  if (est_vide(l)){
    return lnew;
  }

  while (!est_vide(courant->suivant)){
    courant = courant->suivant;
  }

  lnew->precedent = courant;
  courant->suivant = lnew;

  return l;
}

liste_chainee supprimer(liste_chainee l, int val){
  liste_chainee courant = l, prec = NULL;
  int fin_liste;

  if (est_vide(l)){
    return l;
  }

  if (tete(l) == val){
    supprimer_tete(l);
  }

  while(tete(courant) != val && !(fin_liste = est_vide(courant->suivant))){
    prec = courant;
    courant = courant->suivant;
  }
  prec->suivant = courant->suivant;
  if (!fin_liste) courant->precedent = prec;
  free(courant);

  return l;
}

void afficher_liste(liste_chainee l){
  liste_chainee courant = l;

  while (!est_vide(courant)){
    printf("%d ", tete(courant));
    courant = courant->suivant;
  }

  printf("\n");
}

void liberer_liste(liste_chainee l){
   liste_chainee courant = l;

  while (!est_vide(l->suivant)){
    courant = l;
    l = l->suivant;
    supprimer_tete(courant);
  }
  supprimer_tete(l);
}



/*************************************
* PILES
**************************************/
pile pile_vide(){
  return NULL;
}

int pile_est_vide(pile p){
  if (p == NULL) return 1;
  return 0;
}

pile empiler(pile p, int val){
  p = inserer_tete(p, val);
  return p;
}

pile depiler(pile p){
  p = supprimer_tete(p);
  return p;
}

int sdp(pile p){
  return tete(p);
}

void afficher_pile(pile p){
  afficher_liste(p);
}

void liberer_pile(pile p){
  liberer_liste(p);
}


/************************************
* FILES
*************************************/
file file_vide(){
  return NULL;
}

int file_est_vide(file f){
  if (f == NULL) return 1;
  return 0;
}

file defiler(file f){
  f = supprimer_tete(f);
  return f;
}

int debut(file f){
  return tete(f);
}

file enfiler(file f, int val){
  f = inserer_fin(f, val);
  return f;
}

void afficher_file(file f){
  afficher_liste(f);
}

void liberer_file(file f){
  liberer_liste(f);
}



