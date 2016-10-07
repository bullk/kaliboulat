#define WITH_GUI

#include <unistd.h> // sleep

#include <stdio.h>
#include <string>
#include <iomanip>

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
#include "Listener.hpp"

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

void midiCallback (double timeStamp, vector< unsigned char > *message, void *userData)
{
	unsigned int high_digit = message->at(0) >> 4;
	unsigned int low_digit = message->at(0) & 0xF;
	switch ( high_digit )
	{
		case 0x8: // note off
			cout << "channel " << low_digit +1
				<< " NOTE OFF " << (unsigned int)message->at(1)
				<< endl;
			break;
		case 0x9: // note on
			cout << "channel " << low_digit +1
				<< " NOTE ON " << (unsigned int)message->at(1)
				<< " velocity " << (unsigned int)message->at(2)
				<< endl;
			break;
		case 0xA: // polyphonic after touch
			cout << "channel " << low_digit +1
				<< " polyphonic after touch note " << (unsigned int)message->at(1)
				<< " pressure " << (unsigned int)message->at(2)
				<< endl;
			break;
		case 0xB: // control change
			cout << "channel " << low_digit +1
				<< " controller " << (unsigned int)message->at(1)
				<< " value " << (unsigned int)message->at(2)
				<< endl;
			break;
		case 0xC: // program change
			cout << "channel " << low_digit +1
				<< " program change " << (unsigned int)message->at(1)
				<< endl;
			break;
		case 0xD: // channel after touch
			cout << "channel " << low_digit +1
				<< " after touch " << (unsigned int)message->at(1)
				<< endl;
			break;
		case 0xE: // pitch bend
			cout << "channel " << low_digit +1
				<< " pitch bend " << (unsigned int)message->at(2)
				<< endl;
			break;
		case 0xF: // system message
			switch ( low_digit )
			{
				case 0x0:
					cout << "SYSEX" << endl;
					break;
				case 0x1:
					cout << "MTC" << endl;
					break;
				case 0x2:
					cout << "Song Position" << endl;
					break;
				case 0x3:
					cout << "Song Select" << endl;
					break;
				case 0x6:
					cout << "Tune request" << endl;
					break;
				case 0x8:
					cout << "Timing clock" << endl;
					break;
				case 0x9:
					cout << "Mesure end" << endl;
					break;
				case 0xA:
					cout << "Start" << endl;
					break;
				case 0xB:
					cout << "Continue" << endl;
					break;
				case 0xC:
					cout << "Stop" << endl;
					break;
				case 0xE:
					cout << "Active sensing" << endl;
					break;
				case 0xF:
					cout << "System reset" << endl;
					break;
				default:
					cout << "WARNING : unknown MIDI data" << endl;
					break;
			}
			break;
		default:
			cout << "WARNING : unknown MIDI data" << endl;
			break;
	}
	State::getInstance() -> midiLog (message);
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
			AudioClip * clip = clipset -> at (j);
			if ( clip -> isPlaying () )
				*samples += clip -> tick () * *(clip -> getVolume ());
		}
		*samples += Listener::getInstance() -> tick();
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
	cout << "----- engine init -----" << endl;
	cout << "creating State" << endl;
	State * state = State::getInstance ();
	cout << "creating Waiter" << endl;
	Waiter * waiter = Waiter::getInstance ();
	cout << "creating Listener" << endl;
	Listener * listener = Listener::getInstance ();

	cout << endl << "----- project init -----" << endl;
	// INIT PROJECT
	Project * project = new Project("test");
	
	//project -> addAudioTrack ( "AudioTrack1" );
	//project -> addAudioTrack ( "AudioTrack2" );
	//project -> addMidiTrack ( "MidiTrack1" );
	
	
	cout << endl << "----- audio init -----" << endl;
	// AUDIO INIT
	#ifdef __UNIX_JACK__
		RtAudio * dac = new RtAudio (RtAudio::UNIX_JACK); // main audio output
	#else
		RtAudio * dac = new RtAudio (); // main audio output
	#endif
	audioInit (dac, project);

	// MIDI INIT
	cout << endl << "----- MIDI init -----" << endl;
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
	midiin->setCallback (midiCallback);
	midiin->ignoreTypes( false, true, true );
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

	cout << endl << "----- GUI init -----" << endl;
	// GUI INIT
	#ifdef WITH_GUI
		if ( GUI_Init () != 0 )	return -1;
	#endif
	
	// MAIN LOOP
	
	cout << endl << "----- starting main loop -----" << endl;
	unsigned int midi_ticks = 0;
	
	while ( state -> isOn () )
	{
		// Clock update
		if ( project -> getClock () -> getState () ) 
		{
			midi_ticks = project -> getClock () -> update ();
			// MIDI flow
			for (unsigned int i=0; i<midi_ticks; i++) 
				project -> getMIDI () -> tick (midiout);
		}
		
		// Waiter
		waiter -> main ();
		
		// GUI
		#ifdef WITH_GUI
			GUI_Main (project);
		#endif

	}
	
	// CLOSE APP
	
	#ifdef WITH_GUI
		GUI_Close ();
	#endif
	
	midiPanic (midiout);

	cout << "closing MidiOut port" << endl;
	midiout -> closePort ();
	cout << "closing MidiIn port" << endl;
	midiin -> closePort ();
	cout << "canceling MidiIn callback" << endl;
	midiin -> cancelCallback ();
	cout << "closing Audio ports" << endl;
	audioClose (dac);

	cout << "deleting project" << endl;
	delete project;
	cout << "deleting RtMidiIn" << endl;
	delete midiin;
	cout << "deleting RtMidiOut" << endl;
	delete midiout;
	cout << "deleting RtAudio" << endl;
	delete dac;
	//delete diApp;
	cout << "killing Listener" << endl;
	listener -> kill ();
	cout << "killing Waiter" << endl;
	waiter -> kill ();
	cout << "killing State" << endl;
	state -> kill ();
	
	return 0; /* ISO C requires main to return int. */

}

