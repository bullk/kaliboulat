#include "MidiFile.hpp"
#include <stdlib.h>
#include "MidiClip.hpp"


//-------------
// Constructor 
//-------------

MidiFile::MidiFile (std::string path) : MidiFileIn (path), RessourceFile(MIDI, path)
{
	length_ = 0;
	time_ = 0;
	miditracks_ = new std::vector<std::shared_ptr<MidiClip>>;
}


//------------
// Destructor 
//------------

MidiFile::~MidiFile ()
{
	delete miditracks_;
}


//------------

void MidiFile::rewind ()
{
	time_ = 0;
	for (unsigned int i=0; i<getNumberOfTracks(); i++) rewindTrack(i);
}

std::shared_ptr<MidiClip> MidiFile::getTrack (unsigned int i)
{
	return miditracks_ -> at(i);
}

void MidiFile::parseTrack (unsigned int i=0)
{
	std::vector<unsigned char> * event = new std::vector<unsigned char> ();
	unsigned long delta_time, abs_time;
	
	char buffer[50];
	sprintf(buffer, "%s-track-%02d", name_.c_str(), i);
	std::string clipname = buffer;
	std::shared_ptr<MidiClip> clip(new MidiClip (clipname));
	clip -> setDivision (division_);
	rewindTrack (i);
	abs_time=0;
	delta_time = getNextEvent (event, i);
	while ( event -> size () > 0 )
	{
		abs_time += delta_time;
		switch ( event -> at(0) )
		{
			case 0xFF:
				switch ( event -> at(1) )
				{
					case 0x03:
					{
						clipname = "";
						unsigned int strend = event->at(2) + 3;
						for ( unsigned int i=3; i < strend; i++ )
							clipname += (char) event -> at(i);
						clip -> setName (clipname);
						break;
					}
					default:
						break;
				}
				break;
			default:
				clip -> appendEvent (abs_time, event);
		}
		delta_time = getNextEvent (event, i);
	}
	clip -> setLength (abs_time);
	if ( abs_time > length_ ) length_ = abs_time;
	miditracks_ -> push_back (clip);
					
	delete event;
}

void MidiFile::parse ()
{
	if (not length_)
	{
		switch ( getFileFormat() )
		{
			case 0:
				parseTrack (0);
				break;

			default:
				for (unsigned int i=1; i<getNumberOfTracks(); i++) parseTrack (i);
				break;
		}
	}
}

//long unsigned int MidiFile::getLength () { return length_; }

//long unsigned int MidiFile::getTime () { return time_; }

//void MidiFile::tick (RtMidiOut * midiout)
//{
	//if (time_ == length_) rewind ();
	//for (unsigned int i=0; i<getNumberOfTracks(); i++)
	//{
		//track_indexes_[i]++;
		//
	//}
	//time_++;
//}

