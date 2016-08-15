CC = g++
#CFLAGS = -Wall -Os -D__LITTLE_ENDIAN__ -D__LINUX_ALSA__ -D__LINUX_ALSASEQ__
CFLAGS = -Wall -Os -D__LITTLE_ENDIAN__ -D__UNIX_JACK__
CFLAGS += -std=c++0x
#CFLAGS += -std=c++11
LDFLAGS = `sdl2-config --cflags` -I/usr/include/stk 
LDLIBS = `sdl2-config --libs` -lGL -lstk -lrtaudio -lrtmidi
EXT_OBJS = imgui.o imgui_draw.o imgui_demo.o imgui_impl_sdl.o
GUI_OBJS = GUI.o
MAIN_OBJS = Clock.o Clip.o Scheduled.o midi.o MidiClip.o MidiGroup.o MidiFile.o AudioGroup.o Project.o Kaliboulat.o 
OBJS = $(EXT_OBJS) $(GUI_OBJS) $(MAIN_OBJS) 

Kaliboulat: $(OBJS) globals.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o Kaliboulat $(OBJS) $(LDLIBS)

.cpp.o: $*.h $*.hpp $*.cpp 
	$(CC) $(CFLAGS) -c $*.cpp

clean:
	rm $(MAIN_OBJS) $(GUI_OBJS)
