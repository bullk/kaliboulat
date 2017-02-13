#ifndef INC_MIDIFILE_H
#define INC_MIDIFILE_H

#include <memory>
#include <vector>
#include <stk/MidiFileIn.h>
#include "Engine.hpp"


class MidiClip;

class MidiFile : public stk::MidiFileIn, public RessourceFile
{
public:
	MidiFile (std::string path);
	~MidiFile ();
	void rewind ();
	void parse ();
	inline std::string getTrackName (unsigned int tn) { return tracks_.at (tn) ;}
	std::shared_ptr<MidiClip> getTrack (unsigned int);
	//inline unsigned int nClips () { return miditracks_ -> size(); }
	//long unsigned int getLength ();
	//long unsigned int getTime ();
	//void tick (RtMidiOut *);
	
protected:
	long unsigned int length_, time_;
	std::vector<std::shared_ptr<MidiClip>> * miditracks_;
	std::vector<std::string> tracks_;
	//std::vector<long unsigned int> track_indexes_;
	std::string trackName (unsigned int);
	void parseTrack (unsigned int);
};

#endif
