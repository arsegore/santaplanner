SELECT DISTINCT id_creneau, id_jour, id_ligne 
FROM travaille 
JOIN disponibilite ON travaille.id_disponibilite = disponibilite.id
JOIN semaine ON disponibilite.id_semaine = semaine.id
WHERE numero = ?1
AND mois = ?2
AND annee = ?3
AND id_lutin = ?4
;
