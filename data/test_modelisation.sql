

/*
DROP TABLE IF EXISTS travaille;
DROP TABLE IF EXISTS disponibilite;
DROP TABLE IF EXISTS semaine;
DROP TABLE IF EXISTS jour;
DROP TABLE IF EXISTS creneau;
DROP TABLE IF EXISTS ligne;
DROP TABLE IF EXISTS lutin;


CREATE TABLE IF NOT EXISTS lutin (
       id INT UNSIGNED NOT NULL AUTO_INCREMENT,
       nom VARCHAR(255) NOT NULL,
       specialite ENUM('bricoleur', 'empaqueteur', 'controleur'),
       CONSTRAINT pk_lutin_id PRIMARY KEY(id)
);

CREATE TABLE IF NOT EXISTS ligne(
       id INT UNSIGNED NOT NULL AUTO_INCREMENT,
       CONSTRAINT pk_ligne_id PRIMARY KEY(id)
);

CREATE TABLE IF NOT EXISTS creneau(
       id INT UNSIGNED NOT NULL AUTO_INCREMENT,
       heure_debut TIME NOT NULL,
       heure_fin TIME NOT NULL,
       CONSTRAINT pk_creneau_id PRIMARY KEY(id)
);

CREATE TABLE jour(
       nom VARCHAR(255) NOT NULL,
       CONSTRAINT pk_jour_nom PRIMARY KEY(nom)
);

CREATE TABLE semaine(
       id INT UNSIGNED NOT NULL AUTO_INCREMENT,
       numero INT UNSIGNED NOT NULL,
       mois VARCHAR(255) NOT NULL,
       annee YEAR NOT NULL,
       CONSTRAINT pk_semaine PRIMARY KEY(id),
       CONSTRAINT unq_semaine_numero_mois_annee UNIQUE(numero, mois, annee)
);

CREATE TABLE IF NOT EXISTS disponibilite(
       id INT UNSIGNED NOT NULL AUTO_INCREMENT,
       id_lutin INT UNSIGNED NOT NULL,
       nom_jour VARCHAR(255) NOT NULL,
       id_semaine INT UNSIGNED NOT NULL,
       id_creneau INT UNSIGNED NOT NULL,
       CONSTRAINT pk_disponibilite_id PRIMARY KEY(id),
       CONSTRAINT fk_disponibilite_id_lutin_lutin FOREIGN KEY(id_lutin) REFERENCES lutin(id),
       CONSTRAINT fk_disponibilite_nom_jour_jour FOREIGN KEY(nom_jour) REFERENCES jour(nom),
       CONSTRAINT fk_disponibilite_id_creneau_creneau FOREIGN KEY(id_creneau) REFERENCES creneau(id),
       CONSTRAINT fk_disponibilite_id_semaine_semaine FOREIGN KEY(id_semaine) REFERENCES semaine(id)
);

CREATE TABLE IF NOT EXISTS travaille(
       id INT UNSIGNED NOT NULL AUTO_INCREMENT,
       id_lutin INT UNSIGNED NOT NULL,
       id_ligne INT UNSIGNED NOT NULL,
       id_semaine INT UNSIGNED NOT NULL,
       nom_jour VARCHAR(255) NOT NULL,
       id_creneau INT UNSIGNED,
       CONSTRAINT pk_travaille_id PRIMARY KEY(id),
       CONSTRAINT fk_travaille_id_lutin_lutin FOREIGN KEY(id_lutin) REFERENCES lutin(id),
       CONSTRAINT fk_travaille_id_ligne_ligne FOREIGN KEY(id_ligne) REFERENCES ligne(id),
       CONSTRAINT fk_travaille_id_semaine_semaine FOREIGN KEY(id_semaine) REFERENCES semaine(id),
       CONSTRAINT fk_travaille_nom_jour_jour FOREIGN KEY(nom_jour) REFERENCES jour(nom),
       CONSTRAINT fk_travaille_id_creneau_creneau FOREIGN KEY(id_creneau) REFERENCES creneau(id)
);
*/


/*
INSERT INTO lutin (nom, specialite) VALUES
('Lutin Bricoleur', 'bricoleur'),
('Lutin Empaqueteur', 'empaqueteur'),
('Lutin Controleur', 'controleur');

INSERT INTO ligne VALUES (NULL), (NULL), (NULL);

INSERT INTO creneau (heure_debut, heure_fin) VALUES
('08:00:00', '12:00:00'),
('12:00:00', '16:00:00'),
('16:00:00', '20:00:00');

INSERT INTO jour (nom) VALUES
('Lundi'),
('Mardi'),
('Mercredi'),
('Jeudi'),
('Vendredi');

INSERT INTO semaine (numero, mois, annee) VALUES
(1, 'Janvier', 2025),
(2, 'Janvier', 2025),
(3, 'Février', 2025);

INSERT INTO disponibilite (id_lutin, nom_jour, id_semaine, id_creneau) VALUES
(1, 'Lundi', 1, 1),
(2, 'Mardi', 2, 2),
(3, 'Mercredi', 3, 3);

INSERT INTO travaille (id_lutin, id_ligne, id_semaine, nom_jour, id_creneau) VALUES
(1, 1, 1, 'Lundi', 1),
(2, 2, 2, 'Mardi', 2),
(3, 3, 3, 'Mercredi', 3);
*/

SELECT * FROM travaille;

SELECT lutin.nom AS 'Nom', jour.nom AS 'Jour', creneau.heure_debut AS 'Début', creneau.heure_fin AS 'Fin', ligne.id AS 'Numero de ligne'
FROM travaille
JOIN lutin ON travaille.id_lutin = lutin.id
JOIN ligne ON travaille.id_ligne = ligne.id
JOIN creneau ON travaille.id_creneau = creneau.id
JOIN jour ON travaille.nom_jour = jour.nom
JOIN semaine ON travaille.id_semaine = semaine.id
;
