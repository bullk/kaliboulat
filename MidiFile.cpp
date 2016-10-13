#include <stdlib.h>
#include "MidiFile.hpp"

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


void MidiFile::parse ()
{
	if (not length_)
	{
		// Assuming File is MIDI Format 1
		std::vector<unsigned char> * event = new std::vector<unsigned char> ();
		unsigned long delta_time, abs_time;
		// TODO : Special parsing for Track 0
		// TODO : Destruction des clips en cas d'appel au destructeur
		if ( getFileFormat() == 1 )
			for (unsigned int i=1; i<getNumberOfTracks(); i++)
			{
				//std::cout << "preparing track name" << std::endl;
				char buffer[50];
				sprintf(buffer, "%s-track-%02d", name_.c_str(), i);
				std::string clipname = buffer;
				//std::cout << "new midi clip : " << clipname;
				std::shared_ptr<MidiClip> clip(new MidiClip (clipname));
				//std::cout << " : at address : " << clip << std::endl;
				clip -> setDivision (division_);
				rewindTrack (i);
				abs_time=0;
				//std::cout << "start events parsing" << std::endl;
				delta_time = getNextEvent (event, i);
				while ( event -> size () > 0 )
				{
					abs_time += delta_time;
					//std::cout << std::dec << delta_time << " : " << abs_time << " : event ";
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
									//std::cout << "clipname : " << clipname << std::endl;
									break;
								}
								default:
									//std::cout << "unknown 0xFF event : ";
									//for (auto j = event->begin(); j != event->end(); ++j)
										//std::cout << std::hex << (int)*j << ' ';
									break;
							}
							break;
						default:
							//for (auto j = event->begin(); j != event->end(); ++j)
								//std::cout << std::hex << (int)*j << ' ';
							clip -> appendEvent (abs_time, event);
					}
					//std::cout << std::endl;
					delta_time = getNextEvent (event, i);
				}
				//std::cout << "events parsing done" << std::endl;
				clip -> setLength (abs_time);
				if ( abs_time > length_ ) length_ = abs_time;
				//std::cout << "Set clip length : " << std::dec << abs_time << std::endl;
				miditracks_ -> push_back (clip);
				//std::cout << "clip " << clipname << " available" << std::endl;
			}
		delete event;
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

