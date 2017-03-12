#include <string>
#include <vector>
#include <cmath>
#include <stk/FileWvIn.h>
#include <stk/FileWvOut.h>

#define FILEIN "/home/bullk/test_06.wav"

int main( int argc, char* args[] )
{
	stk::FileWvIn filewvin = stk::FileWvIn( FILEIN, false, true );
	for ( unsigned int i=0; i<filewvin.channelsOut(); i++ )
	{
		std::string fout = FILEIN + std::string("-") + std::to_string(i);
		stk::FileWvOut filewvout = stk::FileWvOut( fout );
		std::cout << "channel " << i << std::endl;
		std::vector<stk::StkFloat> data;
		int mod = 0;
		float delta = 0.0f;
		float max = 0.0f;
		float corrected;
		filewvin.reset();
		stk::StkFloat previous;
		stk::StkFloat value = filewvin.tick(i);
		//~ filewvout.tick( previous );
		while ( not( filewvin.isFinished() ) )
		{
			//~ std::cout << "value : " << value << std::endl;
			corrected = value + mod;
			data.push_back( corrected );
			if ( std::abs(corrected) > max ) max = std::abs(corrected);
			previous = value;
			value = filewvin.tick(i);
			delta = value - previous;
			if ( delta > 1 )
			{
				if ( mod == 0 ) mod = -2;
				else mod = 0;
				std::cout << "delta : " << delta << std::endl;
			}
			else if ( delta < -1 )
			{
				if ( mod == 0 ) mod = 2;
				else mod = 0;
				std::cout << "delta : " << delta << std::endl;
			}
			
		}
		std::cout << "maximum amp : " << max << std::endl;
		for ( std::vector<stk::StkFloat>::iterator f=data.begin(); f != data.end(); f++ )
			filewvout.tick( *f / max );
		filewvout.closeFile();
	}
	filewvin.closeFile();
	return 0;
}
