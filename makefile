CC = g++
CFLAGS = -Wall -Os -D__LITTLE_ENDIAN__ 
CFLAGS += -std=c++0x
#CFLAGS += -std=c++11
JACKFLAGS = -D__UNIX_JACK__
ALSAFLAGS = -D__LINUX_ALSA__ -D__LINUX_ALSASEQ__
LDFLAGS = `sdl2-config --cflags` -I/usr/include/stk 
LDLIBS = `sdl2-config --libs` -lGL -lstk -lrtaudio -lrtmidi
ALSALIBS = -lasound -lpthread
JACKLIBS = -ljack
EXT_OBJS = imgui.o imgui_draw.o imgui_demo.o imgui_impl_sdl.o
GUI_OBJS = GUI.o
MAIN_OBJS = Clock.o Clip.o Scheduled.o midi.o MidiClip.o MidiGroup.o MidiFile.o AudioTrack.o Project.o Kaliboulat.o
OBJS = $(EXT_OBJS) $(GUI_OBJS) $(MAIN_OBJS) 

new: Kaliboulat-rtmidi2.0.1
old: Kaliboulat-rtmidi1.0.15

Kaliboulat-rtmidi2.0.1: $(OBJS) globals.h
	$(CC) $(CFLAGS) $(JACKFLAGS) $(LDFLAGS) -o Kaliboulat $(OBJS) $(LDLIBS) $(JACKLIBS)

Kaliboulat-rtmidi1.0.15: $(OBJS) globals.h
	$(CC) $(CFLAGS) $(ALSAFLAGS) $(LDFLAGS) -o Kaliboulat $(OBJS) $(LDLIBS) $(ALSALIBS)

.cpp.o: $*.h $*.hpp $*.cpp 
	$(CC) $(CFLAGS) $(JACKFLAGS) -c $*.cpp
	
clean:
	rm $(MAIN_OBJS) $(GUI_OBJS)
