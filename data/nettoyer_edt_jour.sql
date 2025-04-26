DELETE FROM travaille
WHERE id_disponibilite IN (
      SELECT id
      FROM disponibilite
      WHERE id_semaine = ?1
      AND id_jour = ?2
)
;
