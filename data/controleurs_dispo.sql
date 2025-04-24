SELECT disponibilite.id
FROM disponibilite
JOIN lutin ON disponibilite.id_lutin = lutin.id
WHERE lutin.specialite = 'Contrôleur'
AND id_jour = ?1
AND id_semaine = ?2
AND id_creneau = ?3
AND disponibilite.id NOT IN (
    SELECT id_disponibilite
    FROM travaille
)
;