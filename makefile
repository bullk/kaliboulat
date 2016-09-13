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
MAIN_OBJS = Clock.o Clip.o Scheduled.o Track.o midi.o MidiClip.o MidiTrack.o MidiFile.o AudioTrack.o Project.o Modules.o Kaliboulat.o
OBJS = $(EXT_OBJS) $(GUI_OBJS) $(MAIN_OBJS) 

SUFFIXES = .cpp .o 
.SUFFIXES: $(SUFFIXES) .

new: Kaliboulat-rtmidi2.0.1
old: Kaliboulat-rtmidi1.0.15

Kaliboulat-rtmidi2.0.1: $(OBJS) globals.h
	$(CC) $(CFLAGS) $(JACKFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS) $(JACKLIBS)

Kaliboulat-rtmidi1.0.15: $(OBJS) globals.h
	$(CC) $(CFLAGS) $(ALSAFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS) $(ALSALIBS)

AudioGroup.o: AudioGroup.hpp AudioTrack.hpp Clip.hpp Track.hpp
Clip.o: Clip.hpp
Clock.o: Clock.hpp
GUI.o: GUI.hpp Clock.hpp Modules.hpp MidiTrack.hpp Project.hpp MidiFile.hpp
GUI.o: imgui/imgui.h imgui/imgui_impl_sdl.h
midi.o: midi.hpp Scheduled.hpp
MidiClip.o: MidiClip.hpp midi.hpp Clip.hpp MidiFile.hpp MidiTrack.hpp
MidiTrack.o: MidiTrack.hpp MidiClip.hpp Track.hpp
Modules.o: Modules.hpp AudioTrack.hpp MidiTrack.hpp
Project.o: Project.hpp globals.h Clock.hpp Modules.hpp
Scheduled.o: Scheduled.hpp
Track.o: Track.hpp Clip.hpp

.cpp.o:
	$(CC) $(CFLAGS) $(JACKFLAGS) -c $*.cpp
	
clean:
	rm $(MAIN_OBJS) $(GUI_OBJS)

#AudioTrack
#Clip
#Clock
#GUI
#midi
#MidiClip
#MidiFile
#MidiTrack
#Modules
#Project
#Scheduled
#Track
