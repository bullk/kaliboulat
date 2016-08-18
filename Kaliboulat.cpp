#define WITH_GUI

#include <stdio.h>
#include <string>

#include <RtError.h>
#include <RtAudio.h>
#include <RtMidi.h>

#include "globals.h"
#include "Clock.hpp"
#include "AudioTrack.hpp"
#include "MidiGroup.hpp"
#include "MidiFile.hpp"
#include "Project.hpp"

#ifdef WITH_GUI
#include "GUI.hpp"
#endif

using namespace stk;
using namespace std;

// MIDI

void midiInit (MidiGroup * midigroup_p);
void midiPanic (MidiGroup * midigroup_p);


// Audio

void audioInit (RtAudio * dac, AudioTrack * audiotrack_p);
void audioClose (RtAudio * dac);



//----------------------------------------------------------------------
// MIDI FUNCTIONS
//----------------------------------------------------------------------

void midiInit (MidiGroup * midigroup_p)
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
	AudioTrack * audiotrack_p = (AudioTrack *) dataPointer;
	register StkFloat * samples = (StkFloat *) outputBuffer;

	for ( unsigned int i=0; i<nBufferFrames; i++ )
	{
		*samples = 0;
		for ( unsigned int j = 0; j < audiotrack_p -> getClipSet () -> size (); j++ )
		{
			AudioClip * daClip = audiotrack_p -> getClipSet () -> at (j);
			if ( daClip -> getState () == CS_PLAYING )
				*samples += daClip -> tick () * *(daClip -> getVolume ());
		}
		samples++;
	}
	
	return 0;
}


void audioInit (RtAudio * dac, AudioTrack * audiotrack_p)
{
	Stk::setSampleRate (GLOBAL_SAMPLE_RATE);
	Stk::showWarnings (true);
	
	// Figure out how many bytes in an StkFloat and setup the RtAudio stream.
	RtAudio::StreamParameters parameters;
	parameters.deviceId = dac -> getDefaultOutputDevice ();
	parameters.nChannels = 1;
	
	RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
	
	RtAudio::StreamOptions options;
	options.streamName = APP_NAME;
	
	unsigned int bufferFrames = RT_BUFFER_SIZE;

	try { dac->openStream ( &parameters, NULL, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)audiotrack_p, &options ); }
	catch ( RtError &error ) { error.printMessage (); }

	try { dac->startStream (); }
	catch ( RtError &error ) { error.printMessage (); }
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
	// AUDIO INIT
	#ifdef __UNIX_JACK__
		RtAudio * dac = new RtAudio (RtAudio::UNIX_JACK); // main audio output
	#else
		RtAudio * dac = new RtAudio (); // main audio output
	#endif
	AudioTrack * audioMaster = new AudioTrack (); // Audio clips manager
	audioInit (dac, audioMaster);

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
	MidiGroup * midiMaster = new MidiGroup (); // MIDI clips manager
	midiInit (midiMaster);

	// CLOCK INIT
	Clock * daClock = new Clock ();
	
	// GUI INIT
	#ifdef WITH_GUI
		if ( GUI_Init () != 0 )	return -1;
	#endif
	
	// INIT PROJECT
	Project * project = new Project("test");
	
	// MAIN LOOP
	
	bool go_on = true;
	unsigned int midi_ticks = 0;
	
	while (go_on)
	{
		// Clock update
		if ( daClock -> getState () ) 
		{
			midi_ticks = daClock -> update ();
			//char bbt[13];
			//sprintf (bbt, "%02d:%02d:%03d   ", daClock -> getBar(), daClock -> getBeat(), daClock -> getTick());
			//std::cout << bbt << midi_ticks << std::endl;
			// MIDI flow
			for (unsigned int i=0; i<midi_ticks; i++) 
				midiMaster -> tick (midiout);
		}

		// GUI
		#ifdef WITH_GUI
			GUI_Main (&go_on, daClock, audioMaster, midiMaster, project);
		#endif

	}
	
	// CLOSE APP
	
	#ifdef WITH_GUI
		GUI_Close ();
	#endif
	
	midiPanic (midiout);
	
	midiout -> closePort ();
	audioClose (dac);

	delete daClock;
	delete midiMaster;
	delete midiout;
	delete audioMaster;
	delete dac;

	return 0; /* ISO C requires main to return int. */

}

