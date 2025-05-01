SELECT MIN(nb) AS nb_lignes
FROM (
    SELECT COUNT(*) AS nb FROM disponibilite
    JOIN lutin ON disponibilite.id_lutin = lutin.id
    WHERE disponibilite.id_creneau = ?1 AND disponibilite.id_semaine = ?2 AND disponibilite.id_jour = ?3 AND lutin.specialite = 'Bricoleur'
    UNION ALL
    SELECT COUNT(*) FROM disponibilite
    JOIN lutin ON disponibilite.id_lutin = lutin.id
    WHERE disponibilite.id_creneau = ?1 AND disponibilite.id_semaine = ?2 AND disponibilite.id_jour = ?3 AND lutin.specialite = 'Contrôleur'
    UNION ALL
    SELECT COUNT(*) FROM disponibilite
    JOIN lutin ON disponibilite.id_lutin = lutin.id
    WHERE disponibilite.id_creneau = ?1 AND disponibilite.id_semaine = ?2 AND disponibilite.id_jour = ?3 AND lutin.specialite = 'Empaqueteur'
);

