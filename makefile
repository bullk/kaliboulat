CC = g++
#CFLAGS = -Wall -Os -D__LITTLE_ENDIAN__ -I.
CFLAGS = -Wall -O1 -D__LITTLE_ENDIAN__ -I.
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
MAIN_OBJS = Engine.o Clock.o Clip.o Scheduled.o Track.o midi.o MidiClip.o MidiTrack.o MidiFile.o AudioFile.o AudioClip.o AudioTrack.o Project.o Modules.o State.o Listener.o Kaliboulat.o
OBJS = $(EXT_OBJS) $(GUI_OBJS) $(MAIN_OBJS)

SUFFIXES = .cpp .o
.SUFFIXES: $(SUFFIXES) .

new: Kaliboulat-rtmidi2.0.1
old: Kaliboulat-rtmidi1.0.15

Kaliboulat-rtmidi2.0.1: $(OBJS) globals.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(JACKFLAGS) -o $@ $(OBJS) $(LDLIBS) $(JACKLIBS)

Kaliboulat-rtmidi1.0.15: $(OBJS) globals.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(ALSAFLAGS) -o $@ $(OBJS) $(LDLIBS) $(ALSALIBS)

#tests: MidiClip.o tests.o globals.h
	#$(CC) $(CFLAGS) $(LDFLAGS) $(JACKFLAGS) -o $@ MidiClip.o tests.o $(LDLIBS) $(JACKLIBS)
#
#tests.o: MidiClip.hpp
	
#AUTODEPS
AudioClip.o: AudioClip.hpp Clip.hpp 
AudioFile.o: AudioFile.hpp Engine.hpp 
AudioTrack.o: AudioTrack.hpp AudioClip.hpp Track.hpp 
Clip.o: Clip.hpp globals.h Engine.hpp 
Clock.o: Clock.hpp Engine.hpp 
Engine.o: Engine.hpp State.hpp 
GUI.o: GUI.hpp Clock.hpp State.hpp midi.hpp imgui/imgui.h imgui/imgui_impl_sdl.h Project.hpp Listener.hpp 
Kaliboulat.o: globals.h Clock.hpp MidiFile.hpp State.hpp Modules.hpp Listener.hpp GUI.hpp globals.h Clock.hpp AudioTrack.hpp MidiTrack.hpp MidiFile.hpp Project.hpp Modules.hpp GUI.hpp 
Listener.o: Listener.hpp AudioClip.hpp MidiClip.hpp 
MidiClip.o: MidiClip.hpp midi.hpp Clip.hpp 
midi.o: midi.hpp Scheduled.hpp 
MidiFile.o: MidiFile.hpp MidiTrack.hpp 
MidiTrack.o: MidiTrack.hpp spdlog/spdlog.h MidiClip.hpp Track.hpp 
Modules.o: Modules.hpp globals.h AudioTrack.hpp MidiTrack.hpp 
Project.o: Project.hpp Clock.hpp State.hpp Modules.hpp globals.h AudioFile.hpp MidiFile.hpp 
Scheduled.o: Scheduled.hpp 
State.o: State.hpp Project.hpp 
Track.o: Track.hpp globals.h Clip.hpp 
#AUTODEPS

.cpp.o:
	$(CC) $(CFLAGS) $(JACKFLAGS) -c $*.cpp

clean:
	rm $(MAIN_OBJS) $(GUI_OBJS)
