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

struct App
{
	bool * main_switch;
	Clock * clock;
	AudioTrack * audioMaster;
	MidiTrack * midiMaster;
	//Project * project;
};


//----------------------------------------------------------------------
// MIDI FUNCTIONS
//----------------------------------------------------------------------

void midiInit (MidiTrack * miditrack_p)
{
}

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
	App * diApp = (App *) dataPointer;
	AudioTrack * audiotrack = diApp -> audioMaster;
	register StkFloat * samples = (StkFloat *) outputBuffer;

	if ( diApp -> clock -> isStarted () ) for ( unsigned int i=0; i<nBufferFrames; i++ )
	{
		*samples = 0;
		for ( unsigned int j = 0; j < audiotrack -> getClipSet () -> size (); j++ )
		{
			AudioClip * daClip = audiotrack -> getClipSet () -> at (j);
			if ( daClip -> isPlaying () )
				*samples += daClip -> tick () * *(daClip -> getVolume ());
		}
		samples++;
	}
	else for ( unsigned int i=0; i<nBufferFrames; i++ ) 
	{
		*samples = 0;
		samples++;
	}
	return 0;
}


void audioInit (RtAudio * dac, App * diApp)
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
	
	RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
	
	RtAudio::StreamOptions options;
	options.streamName = APP_NAME;
	
	unsigned int bufferFrames = RT_BUFFER_SIZE;

	try { dac->openStream ( &output_parameters, &input_parameters, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)diApp, &options ); }
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
	App diApp = { NULL, NULL, NULL, NULL };

	bool go_on = true;
	diApp.main_switch = &go_on;

	diApp.clock = new Clock ();
	diApp.audioMaster = new AudioTrack (); // Audio clips manager
	diApp.midiMaster = new MidiTrack (); // MIDI clips manager
		
	// AUDIO INIT
	#ifdef __UNIX_JACK__
		RtAudio * dac = new RtAudio (RtAudio::UNIX_JACK); // main audio output
	#else
		RtAudio * dac = new RtAudio (); // main audio output
	#endif
	audioInit (dac, &diApp);

	// MIDI INIT
	#ifdef __UNIX_JACK__
		RtMidiOut * midiout = new RtMidiOut (RtMidi::UNIX_JACK, APP_NAME);
	#else
		RtMidiOut * midiout = new RtMidiOut(APP_NAME);
	#endif
	//unsigned int nPorts = midiout -> getPortCount (); // Check available ports.
	//if ( nPorts == 0 )
		//std::cout << "No ports available !" << std::endl;
	//else
	//{
		//for ( unsigned int i=0; i < nPorts; i++ )
			//std::cout << "MIDI port " << i << " -> " << midiout -> getPortName (i) << std::endl;
		//midiout -> openPort (); // Open first available port.
	//}
	midiout -> openVirtualPort ();
	midiInit (diApp.midiMaster);

	
	// GUI INIT
	#ifdef WITH_GUI
		if ( GUI_Init () != 0 )	return -1;
	#endif
	
	// INIT PROJECT
	Project * project = new Project("test");
	
	// MAIN LOOP
	
	unsigned int midi_ticks = 0;
	
	while (go_on)
	{
		// Clock update
		if ( diApp.clock -> getState () ) 
		{
			midi_ticks = diApp.clock -> update ();
			//char bbt[13];
			//sprintf (bbt, "%02d:%02d:%03d   ", daClock -> getBar(), daClock -> getBeat(), diApp.clock -> getTick());
			//std::cout << bbt << midi_ticks << std::endl;
			// MIDI flow
			for (unsigned int i=0; i<midi_ticks; i++) 
				diApp.midiMaster -> tick (midiout);
		}

		// GUI
		#ifdef WITH_GUI
			GUI_Main (&go_on, diApp.clock, diApp.audioMaster, diApp.midiMaster, project);
		#endif

	}
	
	// CLOSE APP
	
	#ifdef WITH_GUI
		GUI_Close ();
	#endif
	
	midiPanic (midiout);
	
	midiout -> closePort ();
	audioClose (dac);

	delete midiout;
	delete dac;
	//delete diApp;
	delete diApp.clock;
	delete diApp.midiMaster;
	delete diApp.audioMaster;

	return 0; /* ISO C requires main to return int. */

}

