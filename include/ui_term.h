#ifndef UI_TERM_H_
#define UI_TERM_H_

#include "../include/sqlite3.h"
#include <stdio.h>

void consulter_edt_ligne_terminal(sqlite3 *db, FILE *logs);

void consulter_edt_lutin_terminal(sqlite3 *db, FILE *logs);

void inscrire_lutin_terminal(sqlite3 *db, FILE *logs);

void inscrire_absence_terminal(sqlite3 *db, FILE *logs);

/* Gestion du menu principal, appelle les autres sous-menus */
int menu_principal(sqlite3 *db, FILE *logs);

#endif /*UI_TERM_H*/
