#include "../include/generation.h"
#include "../include/requete.h"

#include <stdio.h>
#include <stdlib.h>

/*!
 * \brief Récupère l'id d'une semaine donnée dans la base de données
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
 * \param num_semaine Le numéro de la semaine dans l'année
 * \mois Le numéro du mois
 * \annee L'année
*/
int semaine2id(sqlite3 *db, FILE *logs, int num_semaine, int mois, int annee){
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  table_resultat *t;
  int res;

  fichier_rq = fopen("data/semaine2id.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);
  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  sqlite3_bind_int(rq, 1, num_semaine);
  sqlite3_bind_int(rq, 2, mois);
  sqlite3_bind_int(rq, 3, annee);
  t = executer_requete(rq, logs);
  afficher_resultats(t);
  res = atoi(t->valeurs[0][0]);

  printf("id_semaine = %d\n", res);

  liberer_resultats(t);
  fclose(fichier_rq);
  free(requete_txt);
  return res;
}


/*!
 * \brief Renvoie le nombre de lignes ouvrables à un instant donné
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
 * \param id_semaine L'identifiant d'une semaine dans la base de données
 * \param id_jour L'identifiant d'un jour dans la base de données (qui correspond en fait au numéro d'un jour dans la semaine)
 * \param id_creneau L'identifiant d'un créneau dans la base de données
*/
int nb_lignes_ouvrables(sqlite3 *db, FILE *logs, int id_semaine, int id_jour, int id_creneau){
  int res;
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  table_resultat *t;

  fichier_rq = fopen("data/lignes_ouvrables.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  sqlite3_bind_int(rq, 1, id_creneau);
  sqlite3_bind_int(rq, 2, id_semaine);
  sqlite3_bind_int(rq, 3, id_jour);
  t = executer_requete(rq, logs);
  res = atoi(t->valeurs[0][0]);

  liberer_resultats(t);
  fclose(fichier_rq);
  free(requete_txt);
  return res / 3;
}

/*!
 * \brief Ouvre une ligne à un instant donné
 *
 * Pour un certain horaire, on récupère l'ensemble des lutins disponibles ordonnés
 * aléatoirement. On insère une nouvelle entrée dans la table travaille pour 3 des lutins
 * disponibles à l'horaire donné sur la ligne choisie.
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
 * \param id_semaine L'identifiant d'une semaine dans la base de données
 * \param id_jour L'identifiant d'un jour dans la base de données (qui correspond en fait au numéro d'un jour dans la semaine)
 * \param id_creneau L'identifiant d'un créneau dans la base de données
 * \param id_ligne La ligne à ouvrir
*/
void ouvrir_ligne(sqlite3 *db, FILE *logs, int id_semaine, int id_jour, int id_creneau, int id_ligne){
  table_resultat *lutins_dispo, *t;
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  int i, id_dispo;

  /* On recupere l'ensemble des lutins disponibles */
  fichier_rq = fopen("data/dispos_a_inserer.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  sqlite3_bind_int(rq, 1, id_jour);
  sqlite3_bind_int(rq, 2, id_semaine);
  sqlite3_bind_int(rq, 3, id_creneau);
  lutins_dispo = executer_requete(rq, logs);
  fclose(fichier_rq);

  /* On ouvre une ligne */
  fichier_rq = fopen("data/ouvrir_ligne.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  for (i = 0; i < 3; i++){
    compiler_requete(db, requete_txt, &rq, &lecture, logs);
    id_dispo = atoi(lutins_dispo->valeurs[i][0]);
    sqlite3_bind_int(rq, 1, id_dispo);
    sqlite3_bind_int(rq, 2, id_ligne);
    t = executer_requete(rq, logs);
    liberer_resultats(t);
  }

  liberer_resultats(lutins_dispo);
  fclose(fichier_rq);
  free(requete_txt);
  return;
}

/*!
 * \brief Renvoie aléatoirement l'une des lignes libres à un horaire donné
 *
 * Récupère l'ensemble des lignes libres à un horaire donné, les ordonne aléatoirement
 * et n'en renvoie qu'un
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
 * \param id_semaine L'identifiant d'une semaine dans la base de données
 * \param id_jour L'identifiant d'un jour dans la base de données (qui correspond en fait au numéro d'un jour dans la semaine)
 * \param id_creneau L'identifiant d'un créneau dans la base de données
 */
int ligne_libre(sqlite3 *db, FILE *logs, int id_semaine, int id_jour, int id_creneau){
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  table_resultat *t;
  int res;

  fichier_rq = fopen("data/ligne_libre.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  sqlite3_bind_int(rq, 1, id_creneau);
  sqlite3_bind_int(rq, 2, id_semaine);
  sqlite3_bind_int(rq, 3, id_jour);
  t = executer_requete(rq, logs);
  res = atoi(t->valeurs[0][0]);
  ajoute_log(logs, "Ligne ouverte.\n");

  liberer_resultats(t);
  fclose(fichier_rq);
  free(requete_txt);
  return res;
}

/*!
 * \brief  Planifie une journée
 *
 * Pour chaque créneau de la journée, vérifie si une ou plusieurs lignes sont ouvrables.
 * Si oui, alors on y ouvre autant de lignes que possible.
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
 * \param id_semaine L'identifiant d'une semaine dans la base de données
 * \param id_jour L'identifiant d'un jour dans la base de données (qui correspond en fait au numéro d'un jour dans la semaine)
 */
void generer_edt_jour(sqlite3 *db, FILE *logs, int id_semaine, int id_jour){
  int id_creneau, nbl, i, id_ligne;

  for (id_creneau = 1; id_creneau < 15; id_creneau++){
    nbl = nb_lignes_ouvrables(db, logs, id_semaine, id_jour, id_creneau);
    if (nbl > 0){
      for (i = 0; i < nbl; i++){
        id_ligne = ligne_libre(db, logs, id_semaine, id_jour, id_creneau);
        ouvrir_ligne(db, logs, id_semaine, id_jour, id_creneau, id_ligne);
      }
    }
  }
  return;
}

/*!
 * \brief Planifie tous les jours d'une semaine
 *
 * Appelle generer_edt_jour pour tous les jours d'une semaine donnée.
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
 * \param id_semaine L'identifiant d'une semaine dans la base de données
*/
void generer_edt_semaine(sqlite3 *db, FILE *logs, int id_semaine){
  int id_jour;

  for (id_jour = 1; id_jour < 8; id_jour++){
    generer_edt_jour(db, logs, id_semaine, id_jour);
  }

  return;
}

/*!
 * \brief Supprime la planification d'une semaine
 *
 * Retire chaque entrée concernant la semaine donnée de la table travaille
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
 * \param id_semaine L'identifiant d'une semaine dans la base de données
 */
void nettoyer_edt_semaine(sqlite3 *db, FILE *logs, int id_semaine){
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  table_resultat *t;

  fichier_rq = fopen("data/nettoyer_edt_semaine.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  sqlite3_bind_int(rq, 1, id_semaine);
  t = executer_requete(rq, logs);

  liberer_resultats(t);
  fclose(fichier_rq);
  free(requete_txt);
  return ;
}
/*!
 * \brief Nettoie puis régénère la planification d'une semaine
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
 * \param id_semaine L'identifiant d'une semaine dans la base de données
 */
void maj_edt_semaine(sqlite3 *db, FILE *logs, int id_semaine){
  nettoyer_edt_semaine(db, logs, id_semaine);
  generer_edt_semaine(db, logs, id_semaine);
}

/*!
 * \brief Nettoie la planification de toutes les semaines
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
 */
void nettoyer_tous_edt(sqlite3 *db, FILE *logs){
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  table_resultat *t;

  fichier_rq = fopen("data/nettoyer_tous_edt.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  t = executer_requete(rq, logs);

  liberer_resultats(t);
  fclose(fichier_rq);
  free(requete_txt);
  return ;
}

/*!
 * \brief Génère la planification de toutes les semaines
 *
 * Récupère l'ensemble des semaines présentes dans la bdd et génère pour chacune
 * sa planification
 *
 * \param db La bdd sur laquelle interagir
 * \param logs Le fichier de où écrire les logs
*/
void generer_tous_edt(sqlite3 *db, FILE *logs){
  FILE *fichier_rq;
  char *requete_txt;
  sqlite3_stmt *rq;
  const char *lecture;
  table_resultat *t;
  int i;

  nettoyer_tous_edt(db, logs);

  fichier_rq = fopen("data/semaines.sql", "r");
  if (fichier_rq == NULL) fprintf(stderr, "Erreur lecture requete\n");
  requete_txt = charger_requete(fichier_rq);

  compiler_requete(db, requete_txt, &rq, &lecture, logs);
  t = executer_requete(rq, logs);

  for (i = 0; i < t->nb_ligne; i++ ){
    generer_edt_semaine(db, logs, atoi(t->valeurs[i][0]));
  }

  liberer_resultats(t);
  fclose(fichier_rq);
  free(requete_txt);
  return;

}
