#include <string>
#include <vector>
#include <cmath>
#include <stk/FileWvIn.h>
#include <stk/FileWvOut.h>

using namespace std;
using namespace stk;

int main( int argc, char* argv[] )
{
	cout << "*** Arguments ***" << endl;
	for ( int a = 0; a < argc; a++ )
		cout <<  a << " : " << argv[a] << endl;
	if ( argc == 1 ) return 1;
	string ifname = string( argv[1] );
	FileWvIn filewvin = FileWvIn( ifname, false, true );
	//size_t pos = ifname.rfind("/");
	//string dirname = ifname.substr(0, pos);
	size_t extpos = ifname.rfind(".wav");
	string basename = ifname.substr(0, extpos);
	for ( unsigned int i=0; i<filewvin.channelsOut(); i++ )
	{
		string ofname = basename +  "-" + to_string(i) + ".wav";
		FileWvOut filewvout = FileWvOut( ofname );
		cout << "channel " << i << endl;
		vector<StkFloat> data;
		unsigned int sn = 0;
		int mod = 0;
		float delta = 0.0f;
		float max = 0.0f;
		float corrected;
		filewvin.reset();
		StkFloat previous;
		StkFloat value = filewvin.tick(i);
		//~ filewvout.tick( previous );
		while ( not( filewvin.isFinished() ) )
		{
			//~ cout << "value : " << value << endl;
			corrected = value + mod;
			data.push_back( corrected );
			if ( abs( corrected ) > max ) max = abs( corrected );
			previous = value;
			sn++;
			value = filewvin.tick(i);
			delta = value - previous;
			if ( delta > 1.7 )
			{
				if ( mod == 0 ) mod = -2;
				else mod = 0;
				cout << sn <<  " | value : " << value << " | delta : " << delta << endl;
			}
			else if ( delta < -1.7 )
			{
				if ( mod == 0 ) mod = 2;
				else mod = 0;
				cout << sn <<  " | value : " << value << " | delta : " << delta << endl;
			}
			
		}
		cout << "maximum amp : " << max << endl;
		for ( vector<StkFloat>::iterator f=data.begin(); f != data.end(); f++ )
			filewvout.tick( *f / max );
		filewvout.closeFile();
	}
	filewvin.closeFile();
	return 0;
}
