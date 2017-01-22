#define WITH_GUI

#include <unistd.h> // sleep

#include <stdio.h>
#include <iomanip>

//#include <RtError.h>
#include <RtAudio.h>
#include <RtMidi.h>

#include "midi.hpp"
#include "Clock.hpp"
#include "State.hpp"
#include "Modules.hpp"
#include "Listener.hpp"

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
		 double streamTime, RtAudioStreamStatus status, void *dataPointer )
{
	AudioModule * audio_module = (AudioModule *) dataPointer;
	register StkFloat * samples = (StkFloat *) outputBuffer;
	
	for ( unsigned int i=0; i<nBufferFrames; i++ )
	{
		*samples = 0;
		if ( State::getProject() -> getClock() -> isStarted() ) 
		{
			std::vector<std::shared_ptr<AudioTrack>> * trackset = audio_module -> getTrackSet ();
			for ( unsigned int j = 0; j < trackset -> size (); j++ )
			{
				std::shared_ptr<AudioTrack> daTrack = trackset -> at (j);
				if ( daTrack -> isPlaying () )
					*samples += daTrack -> tick () * *(daTrack -> getVolume ());
			}
		}
		//std::vector<std::shared_ptr<AudioClip>> * clipset = project -> getAudio () -> getClipSet ();
		//for ( unsigned int j = 0; j < clipset -> size (); j++ )
		//{
			//std::shared_ptr<AudioClip> clip = clipset -> at (j);
			//if ( clip -> isPlaying () )
				//*samples += clip -> tick () * *(clip -> getVolume ());
		//}
		*samples += Listener::getInstance() -> tick();
		samples++;
	}
	return 0;
}


void audioInit (RtAudio * dac, std::shared_ptr<Project> project)
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

	try { dac->openStream ( &output_parameters, &input_parameters, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)project->getAudio(), &options ); }
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
		// Create basic file logger (not rotated)
		// trace, debug, info, warn, error, critical
		auto mainlog = spdlog::stdout_color_mt("main");
		//auto mainlog = spdlog::basic_logger_mt("main", "main.log");
		mainlog->info("Kaliboulat version 0.1 pre-alpha");
	}
	catch (const spdlog::spdlog_ex& ex)
	{
		cout << "Log failed: " << ex.what() << endl;
	}

	auto mainlog= spdlog::get("main");	
	mainlog->info("----- engine init -----");
	mainlog->info("creating Waiter");
	Waiter * waiter = Waiter::getInstance ();
	mainlog->info("creating State");
	State * state = State::getInstance ();
	State::getInstance()->scanAudioFiles ();
	//State::scanMidiFiles ();
	mainlog->info("creating Listener");
	Listener * listener = Listener::getInstance ();

	mainlog->info("----- project init -----");
	// INIT PROJECT
	waiter->newProject(".newproject");
	state->shared();
	
	mainlog->info("----- audio init -----");
	// AUDIO INIT
	#ifdef __UNIX_JACK__
		RtAudio * dac = new RtAudio (RtAudio::UNIX_JACK); // main audio output
	#else
		RtAudio * dac = new RtAudio (); // main audio output
	#endif
	audioInit (dac, state->getProject());

	// MIDI INIT
	mainlog->info("----- MIDI init -----");
	RtMidiOut * midiout = NULL;
	RtMidiIn * midiin = NULL;
	try { 
	#ifdef __UNIX_JACK__
		mainlog->info("creating RtMidiOut with JACK");
		midiout = new RtMidiOut (RtMidi::UNIX_JACK, MIDIOUT_MODULE_NAME);
		midiin = new RtMidiIn (RtMidi::UNIX_JACK, MIDIIN_MODULE_NAME);
	#else
		mainlog->info("creating RtMidiOut without JACK !!!");
		midiout = new RtMidiOut (MIDI_MODULE_NAME);
		midiin = new RtMidiIn (MIDI_MODULE_NAME);
	#endif
	}
	catch ( RtMidiError &error ) 
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
	mainlog->info("opening virtual MIDI out port");
	try { 
		midiout -> openVirtualPort ();
	}
	catch ( RtMidiError &error ) 
	{
		error.printMessage();
		exit( EXIT_FAILURE );
	}
	
	mainlog->info("opening virtual MIDI in port");
	try { 
		midiin -> openVirtualPort ();
	}
	catch ( RtMidiError &error ) 
	{
		error.printMessage();
		exit( EXIT_FAILURE );
	}
	
	//mainlog->info("...MIDI module");
	//midiInit ();

	mainlog->info("----- GUI init -----");
	// GUI INIT
	#ifdef WITH_GUI
		if ( GUI_Init () != 0 )	return -1;
	#endif
	
	// MAIN LOOP
	
	mainlog->info("----- starting main loop -----");
	unsigned int midi_ticks = 0;
	
	while ( state -> isOn () )
	{
		// Clock update
		if ( State::getProject() -> getClock () -> getState () ) 
		{
			midi_ticks = State::getProject() -> getClock () -> update ();
			// MIDI flow
			for (unsigned int i=0; i<midi_ticks; i++) 
				State::getProject() -> getMIDI () -> tick (midiout);
		}
		
		// Waiter
		waiter -> main ();
		
		// GUI
		#ifdef WITH_GUI
			GUI_Main (State::getProject());
		#endif
	}
	
	state->shared();
	// CLOSE APP
	//waiter -> closeProject();
	
	#ifdef WITH_GUI
		GUI_Close ();
	#endif
	
	midiPanic (midiout);

	mainlog->info("closing MidiOut port");
	midiout -> closePort ();
	mainlog->info("closing MidiIn port");
	midiin -> closePort ();
	mainlog->info("canceling MidiIn callback");
	midiin -> cancelCallback ();
	mainlog->info("closing Audio ports");
	audioClose (dac);

	//QUI SE CHARGE DE DETRUIRE LE PROJET ?
	//mainlog->info("deleting project");
	//delete project;
	mainlog->info("deleting RtMidiIn");
	delete midiin;
	mainlog->info("deleting RtMidiOut");
	delete midiout;
	mainlog->info("deleting RtAudio");
	delete dac;
	//delete diApp;
	mainlog->info("killing Listener");
	listener -> kill ();
	mainlog->info("killing State");
	state -> kill ();
	mainlog->info("killing Waiter");
	waiter -> kill ();
	mainlog->info("Done");
	
	return 0; /* ISO C requires main to return int. */

}

