#include "../include/sqlite3.h"
#include "../include/requete.h"
#include "../include/ui_term.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


int main(){
  int statut = 1;
  while (statut == 1){
   statut = menu_principal();
  }
  exit(EXIT_SUCCESS);

}
