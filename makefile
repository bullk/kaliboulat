Kaliboulat: Kaliboulat.cpp AudioGroup.h AudioGroup.cpp Clip.h Clip.cpp GUI_Clip.h GUI_Clip.cpp GUI.h GUI.cpp GUI_const.h
	g++ -Wall -D__LITTLE_ENDIAN__ -D__LINUX_ALSA__ -D__LINUX_ALSASEQ__ -I/usr/include/stk -Os -o Kaliboulat Kaliboulat.cpp GUI.cpp AudioGroup.cpp Clip.cpp GUI_Clip.cpp -lSDL2 -lSDL2_image -lSDL2_gfx -lSDL2_ttf -lstk
	
testinitsdl: testinitsdl.cpp
	g++ -Os -o testinitsdl testinitsdl.cpp -lSDL2

teststreaming: teststreaming.c
	g++ -Os -o teststreaming teststreaming.c -lSDL2
