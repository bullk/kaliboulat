#include "MidiFile.hpp"
#include "MidiClip.hpp"
#include <stdlib.h>
//#include "midi.hpp"

//-------------
// Constructor 
//-------------

MidiFile::MidiFile( std::string path ) : MidiFileIn( path ), RessourceFile( MIDI, path )
{
	auto mainlog= spdlog::get( "main" );	
	mainlog->debug( "MidiFile::MidiFile {}", path );
	length_ = 0;
	time_ = 0;
	miditracks_ = new std::vector<std::shared_ptr<MidiClip>>;
	parse ();
}


//------------
// Destructor 
//------------

MidiFile::~MidiFile()
{
	delete miditracks_;
}


//------------

void MidiFile::rewind()
{
	time_ = 0;
	for ( unsigned int i=0; i<getNumberOfTracks(); i++ ) rewindTrack(i);
}

std::shared_ptr<MidiClip> MidiFile::getTrack( unsigned int i )
{
	return miditracks_ -> at(i);
}

std::string MidiFile::trackName( unsigned int tn )
{
	auto mainlog= spdlog::get( "main" );	
	mainlog->info( "MidiFile::nameTrack {}", tn );
	
	char buffer[50];
	//sprintf(buffer, "%s-track-%02d", name_.c_str(), tn);
	sprintf( buffer, "track-%02d", tn );
	std::string clipname = buffer;
	
	mainlog->info( "parsing events" );
	rewindTrack( tn);
	bool go = true;
	std::vector<unsigned char> * event = new std::vector<unsigned char> ();
	getNextEvent( event, tn);
	while ( go and (event -> size () > 0) )
	{
		switch ( event -> at(0) )
		{
		case 0xFF:
			mainlog->info("SMF meta : {} {}", char_vector_to_hex(*event));
			switch ( event -> at(1) )
			{
			case 0x03:
			{
				clipname = "";
				unsigned int strend = event->at(2) + 3;
				for ( unsigned int i=3; i < strend; i++ )
					clipname += (char) event -> at(i);
				mainlog->info( "track {} : setting trackname to {}", tn, clipname );
				go = false;
				break;
			}
			default:
				break;
			}
			break;
		default:
			break;
		}
		getNextEvent( event, tn );
	}
	mainlog->info( "parsing done" );
	
	delete event;
	mainlog->info( "/MidiFile::nameTrack" );

	return clipname;
}

void MidiFile::parse ()
{
	auto mainlog= spdlog::get( "main" );	
	mainlog->info( "MidiFile::parse" );
	tracks_.clear();
	for ( unsigned int i=0; i<getNumberOfTracks(); i++ )
		tracks_.push_back( trackName(i) );
	//if (not length_)
	//{
		//switch ( getFileFormat() )
		//{
		//case 0:
			//mainlog->info("SMF format 0");
			//parseTrack (0);
			//break;

		//case 1:
			//mainlog->info("SMF format 1"); // TODO : piste Master pour le format 1
			//for (unsigned int i=0; i<getNumberOfTracks(); i++) parseTrack (i);
			//break;

		//case 2:
			//mainlog->info("SMF format 2");
			//for (unsigned int i=0; i<getNumberOfTracks(); i++) parseTrack (i);
			//break;
		//}
	//}
	mainlog->info( "/MidiFile::parse" );
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

