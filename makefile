CC = g++
CFLAGS = -Wall -D__LITTLE_ENDIAN__ -D__LINUX_ALSA__ -D__LINUX_ALSASEQ__ -Os
LDFLAGS = `sdl2-config --cflags` -I/usr/include/stk 
LDLIBS = `sdl2-config --libs` -lGL -lstk
CFILES = Kaliboulat.cpp Clip.cpp AudioGroup.cpp GUI.cpp imgui.cpp imgui_draw.cpp imgui_demo.cpp imgui_impl_sdl.cpp
 
Kaliboulat: Kaliboulat.cpp Clip.h Clip.cpp AudioGroup.h AudioGroup.cpp GUI.h GUI.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o Kaliboulat $(CFILES) $(LDLIBS) 
