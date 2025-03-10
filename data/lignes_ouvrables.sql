SELECT COUNT(DISTINCT specialite) FROM (
	SELECT lutin.nom, lutin.specialite, semaine.numero, semaine.mois, semaine.annee, creneau.heure_debut, creneau.heure_fin
	FROM disponibilite
	JOIN lutin ON disponibilite.id_lutin = lutin.id
	JOIN semaine ON disponibilite.id_semaine = semaine.id
	JOIN jour ON disponibilite.id_jour = jour.id
	JOIN creneau ON disponibilite.id_creneau = creneau.id
	WHERE creneau.id = ?1
    AND semaine.id = ?2
    AND jour.id = ?3
)
;
