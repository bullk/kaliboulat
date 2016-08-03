# TODO

---------------
## MAINTENANT

* Résoudre le problème d'affichage des samples audio (nécessite de potasser les tableaux en C)
* Mettre au point une horloge maison (voir clock.h)

### Moteur

* Horloge (objet à créer)
	* Audio
	* MIDI

* Ajout / suppression de clips
* Playlist / TrackList
* Proprietes des clips / Affichage
* Bases de séquençage pour les clips


### GUI

* Intégrer ImGui
* Le programme doit pouvoir s'éxecuter sans GUI


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


