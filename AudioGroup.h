#ifndef INC_AUDIOGROUP_H
#define INC_AUDIOGROUP_H

#include <string>
#include <vector>
#include "Clip.h"

//using namespace std;

/* Doit contenir un tableau de pointeurs vers des AudioClip
 *	et plus tard des pistes ?
 * Des fonctions d'encapsulation du tableau
 * Et une fonction de rendering (tick)
 */
 
class AudioGroup
{
	
public:

	// Constructor 
	AudioGroup();
	// Destructor 
	~AudioGroup();

    std::vector<AudioClip *> * getClipSet ();
    void addClip (std::string path);
    void stopAll ();
    
private:

	std::vector<AudioClip *> * ClipSet;

};

#endif
