# TODO

---------------
## MAINTENANT

* Rangement et nettoyage
	* Passer les commandes par le Waiter
	* Découper les fichier GUI
	* réagencer le code source dans des répertoires (core? audio midi...)

* Etudier et mettre en place
	* isoler l'interface graphique dans un thread
	* Documentation
	* Tests

* MIDI
	* adapter la vitesse de lecture
	* MIDI IN parsing a finir
	* revoir les logs MIDI

* Gestion des projets
	* Enregistrer (/sous)
	* historique

### Fonctionnalités

* Clip Warp
* ajout multi-clips (ex : fichier MIDI multitracks entier)
	
### C/C++

* fonctions 'static'

### Architecture / Moteur

* Playlist / TrackList
* Bases de séquençage pour les clips
* Baie de brassage ?

### I/O

* entrées (canaux Audio | MIDI | OSC |..., sequenceur, horloge)
* MIDI THRU
* Sortie Audio en double mono
* Entrees / Sorties multiples

### GUI

* Etudier l'intérêt d'un objet GUI
* Factoriser le code
	* couleurs
	* affichage des clips
* Fenêtres d'options
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
	* logo BullK


