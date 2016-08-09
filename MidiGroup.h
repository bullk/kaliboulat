#ifndef INC_MIDIGROUP_H
#define INC_MIDIGROUP_H

#include <string>
#include <vector>
#include "Clip.h"

//using namespace std;

class MidiGroup
{

public:

	// Constructor 
	MidiGroup();
	// Destructor 
	~MidiGroup();

    std::vector<MidiClip *> * getClipSet();
    void addAclip(std::string path);
    //unsigned long getLength();
    void tick();
    
private:

	std::vector<MidiClip *> * ClipSet;

};

#endif
