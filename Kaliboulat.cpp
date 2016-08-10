#define WITH_GUI

#include <stdio.h>
#include <string>

#include <RtError.h>
#include <RtAudio.h>

#include "globals.h"
#include "Clock.h"
#include "AudioGroup.h"
#include "MidiGroup.h"
#include "MidiFile.h"

#ifdef WITH_GUI
#include "GUI.h"
#endif

using namespace stk;
using namespace std;

// MIDI

void midiInit();
string midiClipDir = "user/MIDI";
string midiClipLs[1] = { "test-Drums-1.mid" };


// Audio

string sampleDir = "user/Audio";
string sampleLs[3] = { "bar.wav", "hellosine.wav", "bonjouratoutes.wav" };

int tick();
void audioInit();
void audioClose();



//----------------------------------------------------------------------
// MIDI FUNCTIONS
//----------------------------------------------------------------------

void midiInit(MidiGroup * midigroup_p)
{
	MidiFile * midifile = new MidiFile(midiClipDir + "/" + midiClipLs[0]);
	midifile->parse(midigroup_p);
}


//----------------------------------------------------------------------
// AUDIO FUNCTIONS
//----------------------------------------------------------------------

int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
		 double streamTime, RtAudioStreamStatus status, void *dataPointer )
{
	AudioGroup * audiogroup_p = (AudioGroup *) dataPointer;
	register StkFloat * samples = (StkFloat *) outputBuffer;

	for ( unsigned int i=0; i<nBufferFrames; i++ )
	{
		*samples = 0;
		for ( unsigned int j = 0; j < audiogroup_p->getClipSet()->size(); j++ )
		{
			AudioClip * daClip = audiogroup_p->getClipSet()->at(j);
			if ( daClip->getState() == CS_PLAYING )
				*samples += daClip->tick() * *(daClip->getVolume());
		}
		samples++;
	}
	
	return 0;
}


void audioInit (RtAudio * dac, AudioGroup * audiogroup_p)
{
	audiogroup_p->addAclip (sampleDir + "/" + sampleLs[0]);
	audiogroup_p->addAclip (sampleDir + "/" + sampleLs[1]);
	audiogroup_p->addAclip (sampleDir + "/" + sampleLs[2]);

	Stk::setSampleRate (GLOBAL_SAMPLE_RATE);
	Stk::showWarnings (true);
	
	// Figure out how many bytes in an StkFloat and setup the RtAudio stream.
	RtAudio::StreamParameters parameters;
	parameters.deviceId = dac->getDefaultOutputDevice();
	parameters.nChannels = 1;
	
	RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
	
	RtAudio::StreamOptions options;
	options.streamName = APP_NAME;
	
	unsigned int bufferFrames = RT_BUFFER_SIZE;

	try { dac->openStream ( &parameters, NULL, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)audiogroup_p, &options ); }
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

	RtAudio * dac = new RtAudio(RtAudio::UNIX_JACK); // main audio output
	AudioGroup audioMaster; // Audio clips manager
	audioInit (dac, &audioMaster);

	RtMidiOut * midiout = new RtMidiOut(RtMidi::UNIX_JACK, APP_NAME);
	// Check available ports.
	unsigned int nPorts = midiout->getPortCount();
	if ( nPorts == 0 ) {
		std::cout << "No ports available!\n";
		//goto cleanup;
	}
	// Open first available port.
	midiout->openPort( 0 );
	MidiGroup midiMaster; // MIDI clips manager
	midiInit (&midiMaster);

	Clock daClock; // main clock
	daClock.init();

	#ifdef WITH_GUI
		if (GUI_Init () != 0)	return -1;
	#endif
	
	// Main loop
	
	bool go_on = true;
	unsigned int midi_ticks = 0;
	
	// MAIN LOOP
	while (go_on)
	{
		// Clock update
		if (daClock.getState()) midi_ticks = daClock.update();
		
		// MIDI flow
		for (unsigned int i=0; i<midi_ticks; i++) 
			midiMaster.tick(midiout);

		// GUI
		#ifdef WITH_GUI
			GUI_Main (&go_on, &daClock, &audioMaster, &midiMaster);
		#endif

	}
	
	// Close application
	
	#ifdef WITH_GUI
		GUI_Close ();
	#endif
	delete midiout;
	audioClose (dac);
	delete dac;

	return 0; /* ISO C requires main to return int. */

}

