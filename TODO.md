# TODO

---------------
## MAINTENANT

* Modularisation des fonctions audio
* regrouper l'initialisation des contenus (pour préparer la gestion de projet)
* ENTREE MIDI !!!
* MIDI THRU
* Sorite Audio en double mono
* Propriétés de lecture des clips (single, loop, warped...)
	* single
	* looped
	* warped ?
	* free / next beat / next bar


### Moteur

* Ajout / suppression de clips
* Playlist / TrackList
* Proprietes des clips / Affichage
* Bases de séquençage pour les clips


### GUI

* MAJ imgui
* Etudier l'affichage d'images
* Résoudre le problème d'affichage des samples audio (nécessite de potasser les tableaux en C)


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


