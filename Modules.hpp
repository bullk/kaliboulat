#ifndef INC_MODULES_H
#define INC_MODULES_H

#include <string>
#include <vector>
#include "globals.h"
#include "AudioTrack.hpp"
#include "MidiTrack.hpp"

template <typename TrackType>
class DataModule
{
public:

    TrackType * addTrack (std::string s);
    void deleteTrack (unsigned int i);
    void deleteTrack (TrackType * t);
    inline std::vector<TrackType *> * getTrackSet () { return trackset_; }
    void stopAll ();
    void tick ();
    
protected:
	// Constructor 
	DataModule ();
	// Destructor 
	~DataModule ();
	std::vector<TrackType *> * trackset_;

};


class AudioModule
{
	
public:
	// Constructor 
	AudioModule ();
	// Destructor 
	~AudioModule ();

    std::shared_ptr<AudioTrack> addTrack (std::string s);
    void deleteTrack (unsigned int i);
    void deleteTrack (std::shared_ptr<AudioTrack> t);
    //inline std::shared_ptr<AudioTrack> getTrack (unsigned int i) { return trackset_ -> at(i); }
    inline std::vector<std::shared_ptr<AudioTrack>> * getTrackSet () { return trackset_; }
    //void addClip (std::string path);
    //void deleteClip (unsigned int i);
    //inline std::vector<std::shared_ptr<AudioClip>> * getClipSet () { return clipset_; }
    void stopAll ();
    
protected:
	std::vector<std::shared_ptr<AudioTrack>> * trackset_;
	//std::vector<std::shared_ptr<AudioClip>> * clipset_;

};

class MidiModule
{
	
public:
	// Constructor 
	MidiModule ();
	// Destructor 
	~MidiModule ();

    std::shared_ptr<MidiTrack> addTrack (std::string s);
    void deleteTrack (unsigned int i);
    void deleteTrack (std::shared_ptr<MidiTrack> t);
    //inline std::shared_ptr<MidiTrack> getTrack (unsigned int i) { return trackset_ -> at(i); }
    inline std::vector<std::shared_ptr<MidiTrack>> * getTrackSet () { return trackset_; }
    //void addClip (std::string path);
    //void deleteClip (unsigned int i);
    //inline std::vector<std::shared_ptr<MidiClip>> * getClipSet () { return clipset_; }
    void stopAll ();
    void tick (RtMidiOut * midiout);
    
protected:
	std::vector<std::shared_ptr<MidiTrack>> * trackset_;
	//std::vector<std::shared_ptr<MidiClip>> * clipset_;

};

#endif
