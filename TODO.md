# TODO

---------------
## MAINTENANT

MIDI IN parsing

### Fonctionnalités

* finir l'implémentation des Launch / Stop / Loop pour AudioClip
  et l'adapter pour MidiClip
* Navigation / Import de fichiers
* Gestion des projets
	* Nouveau
	* Ouvrir
	* Enregistrer
* Clip Warp
* ajout multi-clips (ex : fichier MIDI multitracks entier)
	
### C/C++

* fonctions 'static'

### Architecture / Moteur

* Logger
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

* isoler l'interface graphique dans un thread

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


