#include "Modules.hpp"


//-------------
// Constructor 
//-------------

AudioModule::AudioModule ()
{
	trackset_ = new std::vector<std::shared_ptr<AudioTrack>>;
	//clipset_ = new std::vector<std::shared_ptr<AudioClip>>;
}


//------------
// Destructor 
//------------

AudioModule::~AudioModule ()
{
	//if ( clipset_ != NULL )
		//for (unsigned int i=0; i < clipset_ -> size(); i++)
			//delete clipset_ -> at (i);
    //delete each track
	if ( trackset_ != NULL )
		for (unsigned int i=0; i < trackset_ -> size(); i++)
			deleteTrack (i);
}

//==================

std::shared_ptr<AudioTrack> AudioModule::addTrack (std::string s)
{
	std::shared_ptr<AudioTrack> track(new AudioTrack(s));
	trackset_ -> push_back (track);
	return track;
}

void AudioModule::deleteTrack (unsigned int i)
{
	//std::shared_ptr<AudioTrack> track = trackset_ -> at(i);
	trackset_ -> erase (trackset_ -> begin() + i);
	//delete track;
}

void AudioModule::deleteTrack (std::shared_ptr<AudioTrack> t)
{
	unsigned int i=0;
	while ( trackset_ -> at(i) != t ) i++;
	deleteTrack (i);
}

//void AudioModule::addClip (std::string path)
//{
	//clipset_ -> push_back (new AudioClip(path));
//}
//
//void AudioModule::deleteClip (unsigned int i)
//{
	//std::shared_ptr<AudioClip> clip = clipset_ -> at(i);
	//clipset_ -> erase (clipset_ -> begin() + i);
	//delete clip;
//}


//======================================================================

//-------------
// Constructor 
//-------------

MidiModule::MidiModule ()
{
	trackset_ = new std::vector<std::shared_ptr<MidiTrack>>;
	//clipset_ = new std::vector<std::shared_ptr<MidiClip>>;
}


//------------
// Destructor 
//------------

MidiModule::~MidiModule ()
{
	//if ( clipset_ != NULL )
		//for (unsigned int i=0; i < clipset_ -> size(); i++)
			//delete clipset_ -> at (i);
    //delete each track
	if ( trackset_ != NULL )
		for (unsigned int i=0; i < trackset_ -> size(); i++)
			deleteTrack (i);
}

//======================================================================

std::shared_ptr<MidiTrack> MidiModule::addTrack (std::string s)
{
	std::shared_ptr<MidiTrack> track(new MidiTrack(s));
	trackset_ -> push_back (track);
	//std::cout << "Midi Module has " << trackset_ -> size () << " tracks." << std::endl;
	return track;
}

void MidiModule::deleteTrack (unsigned int i)
{
	//std::shared_ptr<MidiTrack> track = trackset_ -> at(i);
	trackset_ -> erase (trackset_ -> begin() + i);
	//std::cout << "Midi Module has " << trackset_ -> size () << " tracks." << std::endl;
	//delete track;
}

void MidiModule::deleteTrack (std::shared_ptr<MidiTrack> t)
{
	unsigned int i=0;
	while ( trackset_ -> at(i) != t ) i++;
	deleteTrack (i);
}

//void MidiModule::addClip (std::string path)
//{
	//clipset_ -> push_back (new MidiClip(path));
//}
//
//void MidiModule::deleteClip (unsigned int i)
//{
	//MidiClip * clip = clipset_ -> at(i);
	//clipset_ -> erase (clipset_ -> begin() + i);
	//delete clip;
//}

void MidiModule::tick (RtMidiOut * midiout)
{
}
