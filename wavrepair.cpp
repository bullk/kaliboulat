#include <stk/FileWvIn.h>
#include <stk/FileWvOut.h>

#define FILEIN "/home/jeandamme/jack_capture_01.wav"
#define FILEOUT "/home/jeandamme/repaired.wav"
//#define FILEIN "/home/jeandamme/white.aiff"

int main( int argc, char* args[] )
{
	stk::FileWvIn filewvin = stk::FileWvIn( FILEIN, true );
	stk::FileWvOut filewvout = stk::FileWvOut( FILEOUT );
	std::cout << "channels : " << filewvin.channelsOut() << std::endl;
	for ( unsigned int i=0; i<1; i++ )
	{
		filewvin.reset();
		stk::StkFloat previous = filewvin.tick(i);
		filewvout.tick(previous);
		filewvin.tick(i);
	}
	filewvin.closeFile();
	filewvout.closeFile();
	return 0;
}
