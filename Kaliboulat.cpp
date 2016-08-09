#define WITH_GUI

#include <stdio.h>
#include <string>

#include <RtAudio.h>
#include <RtError.h>

#include "globals.h"
#include "Clock.h"
#include "AudioGroup.h"
#include "MidiGroup.h"

#ifdef WITH_GUI
#include "GUI.h"
#endif

using namespace stk;
using namespace std;

RtAudio dac; // main audio output
AudioGroup audioMaster; // Audio clips manager
MidiGroup midiMaster; // MIDI clips manager

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

void midiInit()
{
	midiMaster.addAclip (midiClipDir + "/" + midiClipLs[0]);
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


void audioInit ()
{
	Stk::setSampleRate (GLOBAL_SAMPLE_RATE);
	Stk::showWarnings (true);
	
	// Figure out how many bytes in an StkFloat and setup the RtAudio stream.
	RtAudio::StreamParameters parameters;
	parameters.deviceId = dac.getDefaultOutputDevice();
	parameters.nChannels = 1;
	RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
	//RtAudioFormat format = RTAUDIO_SINT16;

	unsigned int bufferFrames = RT_BUFFER_SIZE;

	audioMaster.addAclip (sampleDir + "/" + sampleLs[0]);
	audioMaster.addAclip (sampleDir + "/" + sampleLs[1]);
	audioMaster.addAclip (sampleDir + "/" + sampleLs[2]);

	try {
		dac.openStream( &parameters, NULL, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)&audioMaster );
	}
	catch ( RtError &error ) {
		error.printMessage();
	}

	try {
		dac.startStream();
	}
	catch ( RtError &error ) {
		error.printMessage();
	}

} 

void audioClose ()
{
	try {
		dac.stopStream();
	}
	catch ( RtError &error ) {
		error.printMessage();
	}	
	
	try {
		dac.closeStream();
	}
	catch ( RtError &error ) {
		error.printMessage();
	}
}


//----------------------------------------------------------------------

int main( int argc, char* args[] )
{

	Clock daClock; // main clock
	daClock.init();
	audioInit();
	midiInit();

#ifdef WITH_GUI
	if (GUI_Init() != 0)	return -1;
#endif
	
	// Main loop
	
	bool go_on = true;
	unsigned int midi_ticks = 0;
	
	while (go_on)
	{
		if (daClock.getState()) midi_ticks = daClock.update();
		for (unsigned int i=0; i<midi_ticks; i++) 
			midiMaster.tick();

#ifdef WITH_GUI
		GUI_Main (&go_on, &daClock, &audioMaster, &midiMaster);
#endif

	}
	
	// Close application
#ifdef WITH_GUI
	GUI_Close();
#endif
	audioClose();
	return 0; /* ISO C requires main to return int. */

}

