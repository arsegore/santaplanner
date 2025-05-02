# Santaplanner

**Santaplaner** est un logiciel gérant la planification des lignes de production du père Noël. Il est possible de créer les emplois du temps pour les lignes, mais aussi pour chaque lutin individuellement ! Exportez les au format .ics pour ensuite les ajouter à votre agenda Google Calendar ! 
De plus, vous pouvez directement inscrire de nouveaux lutins dans l'équipe et gérer leurs créneaux de disponibilité dans le logiciel ! 


# Installation 

- Télécharger le code source 
- Installer la version dev de GTK4 (lib graphique) : 
    - Sur un système Ubuntu, tapez simplement : 
    `$ sudo apt install libgtk-4-dev`
- Installer Doxygen :
    - Sur un système Ubuntu : 
    `$ sudo apt install Doxygen`
- Compiler le logiciel : dans le répertoire principal, tapez : 
`$ make`
- Lancer Santaplanner : depuis le répertoire santaplanner : `$ bin/santaplanner`

*NB: Il est normal que la compilation prenne du temps (à cause de SQLite) et génère quelques warnings (à cause de prototypes de fonctions forcés par la lib)*

# Utilisation

Le logiciel se divise en 5 onglets : 
- Accueil 
    - Contient notamment les notes de mise à jour 
- Lignes 
    - Permet de générer et exporter les emplois du temps des lignes (individuelles ou bien toutes les lignes). 
    - Si toutes les lignes sont sélectionnées, une cellule de l'emploi du temps contient les numéros de lignes ouvertes 
    - Si une ligne spécifique est choisie, une cellule contient les lutins travaillant dessus
- Lutins 
    - Permet de générer et exporter les emplois des lutins individuellement
- Données 
    - Permet d'ajouter/supprimer des lutins de l'équipe
    - Ajouter une disponibilité ou une absence à un lutin a pour effet de régénérer la planification de la journée concernée
- Alertes 
    - Affiche le contneu du fichier alertes.txt qui contient la liste de toutes les alertes déclenchées lors des générations. Le buffer se met à jour toutes les 5 secondes. Ce fichier ne sert que de logs pour les alertes, il n'y a donc pas de problème à ce que l'utilisateur l'ouvre de son côté et le modifie (après avoir réglé un conflit par exemple)



