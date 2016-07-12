Kaliboulat: Kaliboulat.cpp AudioGroup.h AudioGroup.cpp Clip.h Clip.cpp imgui/imgui.h imgui/imgui.cpp
	g++ -Wall -D__LITTLE_ENDIAN__ -D__LINUX_ALSA__ -D__LINUX_ALSASEQ__ -I/usr/include/stk -Os -o Kaliboulat Kaliboulat.cpp imgui/imgui.cpp AudioGroup.cpp Clip.cpp -lstk
	
testinitsdl: testinitsdl.cpp
	g++ -Os -o testinitsdl testinitsdl.cpp -lSDL2

teststreaming: teststreaming.c
	g++ -Os -o teststreaming teststreaming.c -lSDL2
