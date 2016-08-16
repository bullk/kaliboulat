# TODO

---------------
## MAINTENANT

* isoler imgui
* Renommer mes fichiers h en hpp

### Fonctionnalités

* Import de fichiers

* Propriétés de lecture des clips (single, loop, warped...)
	* single
	* looped
	* warped ?
	* free / next beat / next bar

* Gestion des projets
	* Nouveau
	* Ouvrir
	* Enregistrer
	
### Architecture / Moteur

* Factoriser le code (via un objet Group Set ?)
* Gestion des pistes
* Playlist / TrackList
* Bases de séquençage pour les clips

### I/O

* MIDI IN
* MIDI THRU
* Sortie Audio en double mono
* Entrees / Sorties multiples

### GUI

* Etudier l'intérêt d'un objet GUI
* Factoriser le code
	* couleurs
	* affichage des clips
* Fenêtres d'options
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


* Navigateur de fichier
	* chercher dans l'existant (SDL)
	* dirent.h

* Design
	* par page A DETAILLER
	* page principale
	* logo BUllK


