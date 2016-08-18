#include <stdlib.h>
#include "MidiFile.hpp"

//-------------
// Constructor 
//-------------

MidiFile::MidiFile (std::string path) : MidiFileIn (path)
{
	path_ = path;
	int p = path_.rfind("/") + 1;
	name_ = path_.substr(p, path_.length()-p);
	length_ = 0;
	time_ = 0;
}


//------------
// Destructor 
//------------

MidiFile::~MidiFile ()
{
}


//------------

void MidiFile::rewind ()
{
	time_ = 0;
	for (unsigned int i=0; i<getNumberOfTracks(); i++) rewindTrack(i);
}

void MidiFile::parse(MidiTrack * miditrack_p)
{
	// Assuming File is MIDI Format 1
	std::vector<unsigned char> * event = new std::vector<unsigned char> ();
	unsigned long delta_time, abs_time;
	
	// TODO : Special parsing for Track 0
	for (unsigned int i=1; i<getNumberOfTracks(); i++)
	{
		char buffer[50];
		sprintf(buffer, "%s-track-%02d", name_.c_str(), i);
		std::string clipname = buffer;
		MidiClip * daClip = new MidiClip (clipname);
		daClip -> setDivision (division_);
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
							clipname = "";
							unsigned int strend = event->at(2) + 3;
							for ( unsigned int i=3; i < strend; i++ )
								clipname += (char) event -> at(i);
							daClip -> setName (clipname);
							break;
						//default:
							//break;
					}
					break;
				default:
					daClip -> appendEvent(abs_time, event);
			}
			delta_time = getNextEvent (event, i);
		}
		daClip -> setLength(abs_time);
		miditrack_p -> addClip (daClip);
	}
	delete event;
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

