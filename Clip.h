#ifndef INC_CLIP_H
#define INC_CLIP_H

#include <string>
//#include <stk/FileRead.h>
#include <stk/FileLoop.h>

/*		Classes Clip, AudioClip, MidiClip
 * 
 *
 */

using namespace std;
using namespace stk;

enum ClipType { CT_AUDIO, CT_MIDI, CT_OSC, CT_COM };

class Clip
{
	
public:
	Clip();
	~Clip();
	virtual string getName(void) = 0;
	virtual unsigned long getLength(void) = 0;
	virtual int getAngle(void) = 0;
   
protected:    
	ClipType m_type;
	string m_name;
	string m_location;
	int m_angle;
	
};


class AudioClip : public Clip, public FileLoop
{
	
public:
	AudioClip(string path);
	~AudioClip();
	string getName(void);
	long unsigned int getLength(void);
	int getAngle(void);
	//StkFloat getTime(void);

private:    
	int m_state;
	
};


class MidiClip : public Clip
{

};

#endif
