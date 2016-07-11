# TODO

---------------
## MAINTENANT

### Moteur

* Horloge Audio
* Ajout / suppression de clips
    * AudioGroup
    * Playlist / TrackList
* Horloge MIDI
* Clips MIDI
* Proprietes des clips / Affichage
* Bases de séquençage pour les clips


### GUI

* objet CLICKABLE
* Placer la gestion d'interface graphique sur l'"extérieur" du main : *elle doit démarrer en premier et s'arrêter en dernier*


---------------
## Architecture

* Trouver un modèle modulaire permettant de d'intégrer 
	* Horloge + Audio + Midi
	* Réseau
	* GUI
	* Plug-ins

* Etudier l'intégration de plugins LV2 (LV2 host)

* Gestion de projets
	* Sauvegarde
	* Restauration
	* Playlist

---------------
## GUI

* Conditionner l'intégration de l'interface pour la compilation	afin de permettre une compilation sans GUI et donc un exécutable en ligne de commande

* Navigateur de fichier
	* chercher dans l'existant (SDL)
	* dirent.h

* Design
	* par page A DETAILLER
	* page principale
	* logo BUllK


