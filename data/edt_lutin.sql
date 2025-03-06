SELECT jour.nom AS 'Jour', semaine.id AS 'Semaine', creneau.heure_debut AS 'Début', creneau.heure_fin AS 'Fin', travaille.id_ligne AS 'Numero de ligne'
FROM travaille
JOIN disponibilite ON travaille.id_disponibilite = disponibilite.id
JOIN semaine ON disponibilite.id_semaine = semaine.id
JOIN lutin ON disponibilite.id_lutin = lutin.id
JOIN jour ON disponibilite.id_jour = jour.id
JOIN creneau ON disponibilite.id_creneau = creneau.id
WHERE lutin.id = ?1
ORDER BY jour.id
;
