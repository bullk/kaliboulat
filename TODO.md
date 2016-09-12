# TODO

---------------
## MAINTENANT

Panneau de détail des fichiers et clips à isoler pour l'emmener de ProjectScreen vers Console et Sequencer

### Redéfinir le moteur 

 * moteur = baie de brassage + centre de commande  avec à l'esprit le 
 pattern Command
 * entrées (canaux Audio | MIDI | OSC |..., sequenceur, horloge)
 * commencer avec une approche de bourrin -> les signaux (bar, beat...) 
 du séquenceur sont diffusés dans les tracks et de là dans les clips
 * pour bénéficier d'une approche "cablage" (comme dans PureData) 
 ouvrant la porte aux plugins et autres modules il faut au maximum gérer 
 la connexion des objets par callback.

### Fonctionnalités

* Propriétés de lecture des clips (single, loop, warped...)
	* single
	* looped
	* warped ?
	* free / next beat / next bar

* Import de fichiers

* Gestion des projets
	* Nouveau
	* Ouvrir
	* Enregistrer
	
### Architecture / Moteur

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
	* logo BUllK


