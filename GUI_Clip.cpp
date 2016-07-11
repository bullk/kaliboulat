#include "GUI_Clip.h"

//------------ Classe GUI_Clip ------------
// Constructor 

GUI_Clip::GUI_Clip()
{
	m_clip = NULL;
}



// Destructor 


GUI_Clip::~GUI_Clip()
{
}



//------------ Classe GUI_AudioClip ------------

// Constructor 

GUI_AudioClip::GUI_AudioClip(AudioClip * c, unsigned short index)
{
	m_clip = c;
	m_pos_x = 20;
	m_pos_y = (index + 1) * 60 - 20;
	labelRect.x = m_pos_x + 60;  //controls the rect's x coordinate
	labelRect.y = m_pos_y - 10; // controls the rect's y coordinte
	labelRect.w = m_clip->getName().size() * 8; // controls the width of the rect
	labelRect.h = 20; // controls the height of the rect
}


// Destructor 

GUI_AudioClip::~GUI_AudioClip()
{
}

void GUI_AudioClip::display(SDL_Renderer* renderer)
{
	filledPieColor(renderer, m_pos_x + 20, m_pos_y, 20, -90, m_clip->getAngle()-90, gfxColorMid1);
	aacircleColor(renderer, m_pos_x + 20, m_pos_y, 20, gfxColorDark1);

	labelSurface = TTF_RenderText_Shaded(myFont, m_clip->getName().c_str(), TTFColorLight1, TTFBlack); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
	labelTexture = SDL_CreateTextureFromSurface(renderer, labelSurface); //now you can convert it into a texture

	//Mind you that (0,0) is on the top left of the window/screen, think a rect as the text's box, that way it would be very simple to understance
	//Now since it's a texture, you have to put RenderCopy in your game loop area, the area where the whole code executes
	SDL_RenderCopy(renderer, labelTexture, NULL, &labelRect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
}
