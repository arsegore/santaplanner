SELECT id
FROM ligne
WHERE id NOT IN (
	SELECT id_ligne
	FROM travaille
	JOIN disponibilite ON disponibilite.id = travaille.id_disponibilite
	WHERE id_creneau = ?1
	AND id_semaine = ?2
	AND id_jour = ?3
	)
ORDER BY RANDOM()
LIMIT 1
;
