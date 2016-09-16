#define WITH_GUI

#include <unistd.h>

#include <stdio.h>
#include <string>

#include <RtError.h>
#include <RtAudio.h>
#include <RtMidi.h>

#include "globals.h"
#include "Clock.hpp"
#include "AudioTrack.hpp"
#include "MidiTrack.hpp"
#include "MidiFile.hpp"
#include "Project.hpp"
#include "Modules.hpp"

#ifdef WITH_GUI
#include "GUI.hpp"
#endif

using namespace stk;
using namespace std;


// MIDI

void midiInit (MidiTrack * miditrack_p);
void midiPanic (MidiTrack * miditrack_p);


// Audio

void audioInit (RtAudio * dac, AudioTrack * audiotrack_p);
void audioClose (RtAudio * dac);

//----------------------------------------------------------------------


//----------------------------------------------------------------------
// MIDI FUNCTIONS
//----------------------------------------------------------------------

//void midiInit ()
//{
//}

void midiPanic (RtMidiOut * midiout)
{
	vector<unsigned char> cc121 { 0xB0, 0x79, 0xFF }; // Channel 1, all CC OFF
	vector<unsigned char> cc123 { 0xB0, 0x7B, 0xFF }; // Channel 1, all notes OFF
	vector<unsigned char> message;
	
	for (unsigned short i=0; i<16; i++)
	{
		message = cc123;
		message[0] += i;
		midiout -> sendMessage(&message);
		message = cc121;
		message[0] += i;
		midiout -> sendMessage(&message);
	}
}


//----------------------------------------------------------------------
// AUDIO FUNCTIONS
//----------------------------------------------------------------------

int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
		 double streamTime, RtAudioStreamStatus status, void *dataPointer )
{
	Project * project = (Project *) dataPointer;
	register StkFloat * samples = (StkFloat *) outputBuffer;
	
	for ( unsigned int i=0; i<nBufferFrames; i++ )
	{
		*samples = 0;
		if ( project -> getClock () -> isStarted () ) 
		{
			std::vector<AudioTrack *> * trackset = project -> getAudio () -> getTrackSet ();
			for ( unsigned int j = 0; j < trackset -> size (); j++ )
			{
				AudioTrack * daTrack = trackset -> at (j);
				if ( daTrack -> isPlaying () )
					*samples += daTrack -> tick () * *(daTrack -> getVolume ());
			}
		}
		std::vector<AudioClip *> * clipset = project -> getAudio () -> getClipSet ();
		for ( unsigned int j = 0; j < clipset -> size (); j++ )
		{
			AudioClip * daClip = clipset -> at (j);
			if ( daClip -> isPlaying () )
				*samples += daClip -> tick () * *(daClip -> getVolume ());
		}
		samples++;
	}
	return 0;
}


void audioInit (RtAudio * dac, Project * project)
{
	Stk::setSampleRate (GLOBAL_SAMPLE_RATE);
	Stk::showWarnings (true);
	
	// Figure out how many bytes in an StkFloat and setup the RtAudio stream.
	RtAudio::StreamParameters input_parameters;
	input_parameters.deviceId = dac -> getDefaultInputDevice ();
	input_parameters.nChannels = AUDIO_INPUTS;
	
	RtAudio::StreamParameters output_parameters;
	output_parameters.deviceId = dac -> getDefaultOutputDevice ();
	output_parameters.nChannels = AUDIO_OUTPUTS;
	
	RtAudioFormat format = ( sizeof (StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
	
	RtAudio::StreamOptions options;
	options.streamName = AUDIO_MODULE_NAME;
	
	unsigned int bufferFrames = RT_BUFFER_SIZE;

	try { dac->openStream ( &output_parameters, &input_parameters, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)project, &options ); }
	catch ( RtError &error ) { error.printMessage (); }

	try { dac->startStream (); }
	catch ( RtError &error ) { error.printMessage (); }
	//sleep ( 1 );	cout << "bonjour" << endl;
	//cout << "bonjour" << endl;
} 

void audioClose (RtAudio * dac)
{
	try { dac->stopStream (); }
	catch ( RtError &error ) { error.printMessage (); }
	
	try { dac->closeStream (); }
	catch ( RtError &error ) { error.printMessage (); }
}


//----------------------------------------------------------------------

int main( int argc, char* args[] )
{
	cout << "\n----- engine init -----" << endl;
	Waiter * waiter;
	waiter = Waiter::getInstance ();

	cout << "\n----- project init -----" << endl;
	// INIT PROJECT
	Project * project = new Project("test");
	
	//project -> addAudioTrack ( "AudioTrack1" );
	//project -> addAudioTrack ( "AudioTrack2" );
	//project -> addMidiTrack ( "MidiTrack1" );
	
	
	cout << "\n----- audio init -----" << endl;
	// AUDIO INIT
	#ifdef __UNIX_JACK__
		RtAudio * dac = new RtAudio (RtAudio::UNIX_JACK); // main audio output
	#else
		RtAudio * dac = new RtAudio (); // main audio output
	#endif
	audioInit (dac, project);

	// MIDI INIT
	cout << "\n----- MIDI init -----" << endl;
	cout << "creating RtMidiOut";
	RtMidiOut * midiout = NULL;
	RtMidiIn * midiin = NULL;
	try { 
	#ifdef __UNIX_JACK__
		cout << " with JACK" << endl;
		midiout = new RtMidiOut (RtMidi::UNIX_JACK, MIDIOUT_MODULE_NAME);
		midiin = new RtMidiIn (RtMidi::UNIX_JACK, MIDIIN_MODULE_NAME);
	#else
		cout << " without JACK !!!" << endl;
		midiout = new RtMidiOut (MIDI_MODULE_NAME);
		midiin = new RtMidiIn (MIDI_MODULE_NAME);
	#endif
	}
	catch ( RtError &error ) 
	{
		error.printMessage ();
		exit ( EXIT_FAILURE );
	}
	
	//unsigned int nPorts = midiout -> getPortCount (); // Check available ports.
	//if ( nPorts == 0 )
		//std::cout << "No ports available !" << std::endl;
	//else
	//{
		//for ( unsigned int i=0; i < nPorts; i++ )
			//std::cout << "MIDI port " << i << " -> " << midiout -> getPortName (i) << std::endl;
		//midiout -> openPort (); // Open first available port.
	//}
	cout << "opening virtual MIDI out port" << endl;
	try { 
		midiout -> openVirtualPort ();
	}
	catch ( RtError &error ) 
	{
		error.printMessage();
		exit( EXIT_FAILURE );
	}
	
	cout << "opening virtual MIDI in port" << endl;
	try { 
		midiin -> openVirtualPort ();
	}
	catch ( RtError &error ) 
	{
		error.printMessage();
		exit( EXIT_FAILURE );
	}
	
	//cout << "...MIDI module" << endl;
	//midiInit ();

	cout << "\n----- GUI init -----" << endl;
	// GUI INIT
	#ifdef WITH_GUI
		if ( GUI_Init () != 0 )	return -1;
	#endif
	
	// MAIN LOOP
	
	cout << "\n----- starting main loop -----" << endl;
	unsigned int midi_ticks = 0;
	bool main_switch = true;
	
	while (main_switch)
	{
		// Clock update
		if ( project -> getClock () -> getState () ) 
		{
			midi_ticks = project -> getClock () -> update ();
			//char bbt[13];
			//sprintf (bbt, "%02d:%02d:%03d   ", daClock -> getBar(), daClock -> getBeat(), diApp.clock -> getTick());
			//std::cout << bbt << midi_ticks << std::endl;
			// MIDI flow
			for (unsigned int i=0; i<midi_ticks; i++) 
				project -> getMIDI () -> tick (midiout);
		}
		
		// Waiter
		waiter -> main ();
		
		// GUI
		#ifdef WITH_GUI
			GUI_Main (&main_switch, project);
		#endif

	}
	
	// CLOSE APP
	
	#ifdef WITH_GUI
		GUI_Close ();
	#endif
	
	midiPanic (midiout);
	
	midiout -> closePort ();
	audioClose (dac);

	delete project;
	delete midiout;
	delete dac;
	//delete diApp;
	waiter -> kill ();
	
	return 0; /* ISO C requires main to return int. */

}

