CC = g++
#CFLAGS = -Wall -Os -D__LITTLE_ENDIAN__ -I.
#CFLAGS = -Wall -O0 -g -D__LITTLE_ENDIAN__ -I.
CFLAGS = -Wall -O1 -D__LITTLE_ENDIAN__ -I.
#CFLAGS = -Wall -O2 -D__LITTLE_ENDIAN__ -I.
#CFLAGS = -Wall -O3 -D__LITTLE_ENDIAN__ -I.
CFLAGS += -std=c++0x
#CFLAGS += -std=c++11
JACKFLAGS = -D__UNIX_JACK__
ALSAFLAGS = -D__LINUX_ALSA__ -D__LINUX_ALSASEQ__
LDFLAGS = `sdl2-config --cflags` -I/usr/include/stk
LDLIBS = `sdl2-config --libs` -lGL -lstk -lpthread -lrtaudio -lrtmidi
ALSALIBS = -lasound -lpthread
JACKLIBS = -ljack
EXT_OBJS = imgui.o imgui_draw.o imgui_demo.o imgui_impl_sdl.o
GUI_OBJS = GUI.o
MAIN_OBJS = globals.o RessourceFile.o Clock.o Clip.o Scheduled.o Track.o midi.o MidiFile.o MidiClip.o MidiTrack.o AudioFile.o AudioClip.o AudioTrack.o Project.o Modules.o State.o Waiter.o Listener.o
OBJS = $(EXT_OBJS) $(MAIN_OBJS) $(GUI_OBJS) Kaliboulat.o

SUFFIXES = .cpp .o
.SUFFIXES: $(SUFFIXES) .

all: Kaliboulat
#all: Kaliboulat wavrepair
#new: Kaliboulat-rtmidi2.0.1
#old: Kaliboulat-rtmidi1.0.15

Kaliboulat: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(JACKFLAGS) -o $@ $(OBJS) $(LDLIBS) $(JACKLIBS)

wavrepair: wavrepair.cpp
	$(CC) $(CFLAGS) -I/usr/include/stk $@.cpp -o $@ -lstk

#Kaliboulat-rtmidi1.0.15: $(OBJS)
#	$(CC) $(CFLAGS) $(LDFLAGS) $(ALSAFLAGS) -o $@ $(OBJS) $(LDLIBS) $(ALSALIBS)

#tests: MidiClip.o tests.o
#	$(CC) $(CFLAGS) $(LDFLAGS) $(JACKFLAGS) -o $@ MidiClip.o tests.o $(LDLIBS) $(JACKLIBS)
#
#tests.o: MidiClip.hpp
	
#AUTODEPS
AudioClip.o: AudioClip.hpp State.hpp Project.hpp Clip.hpp 
AudioFile.o: AudioFile.hpp 
AudioTrack.o: AudioTrack.hpp AudioClip.hpp Track.hpp 
Clip.o: Clip.hpp midi.hpp globals.hpp midi.hpp 
Clock.o: globals.hpp Clock.hpp Waiter.hpp 
globals.o: globals.hpp spdlog/spdlog.h 
GUI.o: GUI.hpp State.hpp Project.hpp MidiFile.hpp MidiClip.hpp AudioFile.hpp AudioClip.hpp Listener.hpp Clock.hpp imgui/imgui.h imgui/imgui_impl_sdl.h 
Kaliboulat.o: midi.hpp Clock.hpp State.hpp Project.hpp Modules.hpp Listener.hpp GUI.hpp Clock.hpp AudioTrack.hpp MidiTrack.hpp MidiFile.hpp Project.hpp Modules.hpp GUI.hpp 
Listener.o: Listener.hpp spdlog/spdlog.h 
MidiClip.o: MidiClip.hpp midi.hpp Project.hpp State.hpp MidiFile.hpp Clock.hpp Clip.hpp midi.hpp 
midi.o: midi.hpp spdlog/spdlog.h Clip.hpp Scheduled.hpp Waiter.hpp 
MidiFile.o: MidiFile.hpp MidiClip.hpp RessourceFile.hpp 
MidiTrack.o: MidiTrack.hpp State.hpp MidiClip.hpp Track.hpp 
Modules.o: Modules.hpp AudioTrack.hpp MidiTrack.hpp 
Project.o: Project.hpp Clock.hpp State.hpp Modules.hpp AudioFile.hpp MidiFile.hpp Track.hpp 
RessourceFile.o: RessourceFile.hpp globals.hpp 
Scheduled.o: Scheduled.hpp 
State.o: State.hpp spdlog/spdlog.h cereal/types/vector.hpp cereal/types/string.hpp 
Track.o: Track.hpp Clip.hpp 
Waiter.o: spdlog/spdlog.h Waiter.hpp Project.hpp State.hpp Clock.hpp midi.hpp 
#AUTODEPS

.cpp.o:
	$(CC) $(CFLAGS) $(JACKFLAGS) -c $*.cpp

clean:
	rm $(MAIN_OBJS) $(GUI_OBJS) Kaliboulat.o
