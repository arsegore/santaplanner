SELECT id
FROM disponibilite
WHERE id_jour = ?1
AND id_semaine = ?2
AND id_creneau = ?3
AND id NOT IN (
      SELECT id_disponibilite
      FROM travaille
)
;
