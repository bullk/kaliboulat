CC = g++
CFLAGS = -Wall -D__LITTLE_ENDIAN__ -D__LINUX_ALSA__ -D__LINUX_ALSASEQ__ -Os 
CFLAGS += -std=c++11
LDFLAGS = `sdl2-config --cflags` -I/usr/include/stk 
LDLIBS = `sdl2-config --libs` -lGL -lstk -lrtaudio 
#CFILES = Kaliboulat.cpp Clip.cpp AudioGroup.cpp GUI.cpp imgui.cpp imgui_draw.cpp imgui_demo.cpp imgui_impl_sdl.cpp
OBJS = Kaliboulat.o Clip.o AudioGroup.o MidiGroup.o imgui.o imgui_draw.o imgui_demo.o imgui_impl_sdl.o
 
Kaliboulat: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o Kaliboulat $(OBJS) $(LDLIBS) 

.cpp.o: $*.h $*.cpp 
	$(CC) $(CFLAGS) -c $*.cpp

clean:
	rm *.o
