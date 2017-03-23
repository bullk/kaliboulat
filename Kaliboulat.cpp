#define WITH_GUI

#include <unistd.h> // sleep

#include <stdio.h>
#include <iomanip>

//#include <RtError.h>
#include <RtAudio.h>
#include <RtMidi.h>
#include "spdlog/spdlog.h"

#include "midi.hpp"
#include "Clock.hpp"
#include "State.hpp"
#include "Project.hpp"
#include "Listener.hpp"
#include "AudioTrack.hpp"

#ifdef WITH_GUI
#include "GUI.hpp"
#endif

using namespace stk;
using namespace std;


// Audio

void audioInit (RtAudio * dac, std::shared_ptr<Project> project);
void audioClose (RtAudio * dac);

//----------------------------------------------------------------------


//----------------------------------------------------------------------
// AUDIO FUNCTIONS
//----------------------------------------------------------------------

int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
		 double streamTime, RtAudioStreamStatus status, void *dataPointer)
{
	register StkFloat * samples = (StkFloat *) outputBuffer;

	for ( unsigned int i=0; i<nBufferFrames; i++ )
	{
		*samples = 0;
		if ( State::getProject()->getClock()->isStarted() ) 
		{
			for ( unsigned int j = 0; j < State::getProject()->nTracks(); j++ )
			{
				if ( State::getProject()->getTrack (j)->dataType () == AUDIO )
				{
					std::shared_ptr<AudioTrack> daTrack = std::static_pointer_cast<AudioTrack>(State::getProject()->getTrack (j));
					if ( daTrack->isPlaying () )
						*samples += daTrack->tick () * *(daTrack->getVolume ());
				}
			}
		}
		*samples += Listener::getInstance()->tick();
		samples++;
	}
	return 0;
}



void audioInit (RtAudio * dac)
{
	Stk::setSampleRate (GLOBAL_SAMPLE_RATE);
	Stk::showWarnings (true);
	
	// Figure out how many bytes in an StkFloat and setup the RtAudio stream.
	RtAudio::StreamParameters input_parameters;
	input_parameters.deviceId = dac->getDefaultInputDevice ();
	input_parameters.nChannels = AUDIO_INPUTS;
	
	RtAudio::StreamParameters output_parameters;
	output_parameters.deviceId = dac->getDefaultOutputDevice ();
	output_parameters.nChannels = AUDIO_OUTPUTS;
	
	RtAudioFormat format = ( sizeof (StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
	
	RtAudio::StreamOptions options;
	options.streamName = AUDIO_MODULE_NAME;
	
	unsigned int bufferFrames = RT_BUFFER_SIZE;

	try { dac->openStream ( &output_parameters, &input_parameters, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)NULL, &options ); }
	catch ( RtAudioError &error ) { error.printMessage (); }

	try { dac->startStream (); }
	catch ( RtAudioError &error ) { error.printMessage (); }
	//sleep ( 1 );	cout << "bonjour" << endl;
	//cout << "bonjour" << endl;
} 

void audioClose (RtAudio * dac)
{
	try { dac->stopStream (); }
	catch ( RtAudioError &error ) { error.printMessage (); }
	
	try { dac->closeStream (); }
	catch ( RtAudioError &error ) { error.printMessage (); }
}


//----------------------------------------------------------------------

int main( int argc, char* args[] )
{
	try
	{         
		// trace, debug, info, warn, error, critical
		//spdlog::level::level_enum LOGLEVEL = spdlog::level::debug;
		spdlog::level::level_enum LOGLEVEL = spdlog::level::info;
		
		//auto mainlog = spdlog::basic_logger_mt("main", "main.log");
		auto mainlog = spdlog::stdout_color_mt("main");
		mainlog->set_level(LOGLEVEL);
		mainlog->info("Kaliboulat version 0.1 pre-alpha");
	}
	catch (const spdlog::spdlog_ex& ex)
	{
		cout << "Log failed: " << ex.what() << endl;
	}

	auto mainlog= spdlog::get("main");	
	mainlog->info("----- engine init -----");
	Waiter * waiter = Waiter::getInstance ();
	State * state = State::getInstance ();
	Listener * listener = Listener::getInstance ();

	// INIT PROJECT
	mainlog->info("----- project init -----");
	waiter->newProject(".newproject");
	state->shared();
	
	mainlog->info("----- audio init -----");
	// AUDIO INIT
	#ifdef __UNIX_JACK__
		RtAudio * dac = new RtAudio (RtAudio::UNIX_JACK); // main audio output
	#else
		RtAudio * dac = new RtAudio (); // main audio output
	#endif
	audioInit (dac);

	midi_init();
	MidiWaiter * midi_waiter = MidiWaiter::getInstance();
	
	mainlog->info("----- GUI init -----");
	// GUI INIT
	#ifdef WITH_GUI
		if ( GUI_Init () != 0 )	return -1;
	#endif
	
	// MAIN LOOP
	
	mainlog->info("----- starting main loop -----");
	unsigned int midi_ticks = 0;
	
	while ( state->isOn () )
	{
		// Clock update
		if ( State::getProject()->getClock()->isStarted() ) 
		{
			midi_ticks = State::getProject()->getClock()->update ();
			for (unsigned int i=0; i<midi_ticks; i++)
				Waiter::getInstance()->tick();
		}
		
		// Waiters
		waiter->main ();
		
		// GUI
		#ifdef WITH_GUI
			GUI_Main();
		#endif
		//usleep(500000 / State::getProject()->getClock()->getTicksPerBeat());
		sleep(0);
	}
	
	state->shared();
	// CLOSE APP
	
	#ifdef WITH_GUI
		GUI_Close ();
	#endif
	
	midiPanic( State::getInstance()->getMidiOut() );

	mainlog->info("closing MidiOut port");
	State::getInstance()->getMidiOut()->closePort();
	mainlog->info("closing MidiIn port");
	State::getInstance()->getMidiIn()->closePort();
	mainlog->info("canceling MidiIn callback");
	State::getInstance()->getMidiIn()->cancelCallback ();
	mainlog->info("closing Audio ports");
	audioClose (dac);

	//QUI SE CHARGE DE DETRUIRE LE PROJET ?
	//mainlog->info("deleting project");
	//delete project;
	//waiter->closeProject();
	midi_waiter->kill ();
	mainlog->info("deleting RtAudio");
	delete dac;
	//delete diApp;
	listener->kill ();
	state->kill ();
	waiter->kill ();
	sleep(1);
	mainlog->info("Done");
	
	//spdlog::drop_all();
	
	return 0; /* ISO C requires main to return int. */

}

