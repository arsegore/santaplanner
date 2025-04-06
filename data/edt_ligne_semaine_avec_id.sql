SELECT DISTINCT id_creneau, id_jour, id_lutin 
FROM travaille 
JOIN disponibilite ON travaille.id_disponibilite = disponibilite.id
JOIN semaine ON disponibilite.id_semaine = semaine.id
WHERE semaine.numero = ?1
AND semaine.mois = ?2
AND semaine.annee = ?3
AND id_ligne = ?4
;
