SELECT DISTINCT id_creneau, id_jour 
FROM travaille 
JOIN disponibilite ON travaille.id_disponibilite = disponibilite.id
JOIN semaine ON disponibilite.id_semaine = semaine.id
WHERE id_semaine = ?1
AND mois = ?2 
AND id_ligne = ?3
; 
