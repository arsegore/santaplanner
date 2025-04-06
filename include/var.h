#ifndef _VAR_H_
#define _VAR_H_

#include "types.h"
#include "sqlite3.h"
#include <stdio.h>

extern sqlite3 *db;
extern FILE *logs;
extern lutin *liste_lutins; 
extern int nb_lutins;

#endif
