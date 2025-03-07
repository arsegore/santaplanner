DELETE FROM disponibilite
WHERE id_lutin = ?1
AND id_jour = ?2
AND id_semaine = ?3
AND id_creneau = ?4
;
