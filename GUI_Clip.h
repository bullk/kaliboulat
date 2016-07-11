#ifndef INC_GUI_CLIP_H
#define INC_GUI_CLIP_H

#include "GUI_const.h"
#include "Clip.h"


class GUI_Clip : public Clickable
{
	
public:
	GUI_Clip();
	~GUI_Clip();
	virtual void display(SDL_Renderer*) = 0;
   
protected:    
	Clip * m_clip;
	Uint32 m_color;
	SDL_Surface* labelSurface;
	SDL_Texture* labelTexture;
	SDL_Rect labelRect;
};


class GUI_AudioClip : public GUI_Clip
{
	
public:
	GUI_AudioClip(AudioClip *, unsigned short);
	~GUI_AudioClip();
	void display(SDL_Renderer*);
	
protected:


};


#endif
