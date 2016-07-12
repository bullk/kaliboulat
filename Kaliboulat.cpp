#include <stdio.h>
#include <cmath>

//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL2_gfxPrimitives.h>
//#include <SDL2/SDL_ttf.h>

#include "globals.h"
#include "imgui/imgui.h"
//#include "GUI.h"


using namespace stk;
using namespace std;


//Audio Rendering
int tick();
void GUI();

//----------------------------------------------------------------------
// AUDIO FUNCTIONS
//----------------------------------------------------------------------

int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
		 double streamTime, RtAudioStreamStatus status, void *dataPointer )
{
	AudioGroup * audiogroup = (AudioGroup *) dataPointer;
	register StkFloat *samples = (StkFloat *) outputBuffer;

	for ( unsigned int i=0; i<nBufferFrames; i++ )
	{
		*samples = 0;
		for ( unsigned int j = 0; j < audiogroup->getClipSet()->size(); j++)
			*samples += audiogroup->getClipSet()->at(j)->tick();
		samples++;
	}
	
	return 0;
}


void audioInit (void)
{
	sampleLs[0] = "bar.wav";
	sampleLs[1] = "hellosine.wav";

	Stk::setSampleRate (44100.0);
	Stk::showWarnings (true);
	
} 

//----------------------------------------------------------------------
// GUI
//----------------------------------------------------------------------

void GUI (void)
{
	// Application init
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = 1920.0f;
	io.DisplaySize.y = 1280.0f;
	io.IniFilename = "imgui.ini";
	//#io.RenderDrawListsFn = my_render_function;  // Setup a render function, or set to NULL and call GetDrawData() after Render() to access the render data.
	// TODO: Fill others settings of the io structure

	// Load texture atlas
	// There is a default font so you don't need to care about choosing a font yet
	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(pixels, &width, &height);
	// TODO: At this points you've got a texture pointed to by 'pixels' and you need to upload that your your graphic system
	// TODO: Store your texture pointer/identifier (whatever your engine uses) in 'io.Fonts->TexID'

	// Application main loop
	while (true)
	{
		// 1) get low-level inputs (e.g. on Win32, GetKeyboardState(), or poll your events, etc.)
		// TODO: fill all fields of IO structure and call NewFrame
		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = 1.0f/60.0f;
		//#io.MousePos = mouse_pos;
		//#io.MouseDown[0] = mouse_button_0;
		//#io.MouseDown[1] = mouse_button_1;
		//#io.KeysDown[i] = ...

		// 2) call NewFrame(), after this point you can use ImGui::* functions anytime
		ImGui::NewFrame();

		// 3) most of your application code here
		ImGui::Begin("My window");
		ImGui::Text("Hello, world.");
		ImGui::End();
		//#MyGameUpdate(); // may use ImGui functions
		//#MyGameRender(); // may use ImGui functions

		// 4) render & swap video buffers
		ImGui::Render();
		//#SwapBuffers();
	}
}


//----------------------------------------------------------------------

int main( int argc, char* args[] )
{

	audioInit();

	// Figure out how many bytes in an StkFloat and setup the RtAudio stream.
	RtAudio::StreamParameters parameters;
	parameters.deviceId = dac.getDefaultOutputDevice();
	parameters.nChannels = 1;
	RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
	//RtAudioFormat format = RTAUDIO_SINT16;

	unsigned int bufferFrames = RT_BUFFER_SIZE;

	try {
		dac.openStream( &parameters, NULL, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)&audioMaster );
	}
	catch ( RtError &error ) {
		error.printMessage();
		goto cleanup;
	}

	try {
		dac.startStream();
	}
	catch ( RtError &error ) {
		error.printMessage();
		goto cleanup;
	}

	audioMaster.addAclip (sampleDir + "/" + sampleLs[0]);


	// Shut down the output stream.
	try {
		dac.closeStream();
	}
	catch ( RtError &error ) {
		error.printMessage();
	}

	cleanup:
		return 0; /* ISO C requires main to return int. */
}


