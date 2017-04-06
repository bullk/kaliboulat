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
LDFLAGS = `sdl2-config --cflags` -I/usr/include/stk -I/usr/include/lo
LDLIBS = `sdl2-config --libs` -lGL -lstk -lpthread -lrtaudio -lrtmidi -llo 
ALSALIBS = -lasound -lpthread
JACKLIBS = -ljack
EXT_OBJS = imgui.o imgui_draw.o imgui_demo.o imgui_impl_sdl.o
GUI_OBJS = GUI.o
MAIN_OBJS = globals.o RessourceFile.o Clock.o Clip.o Scheduled.o Track.o midi.o MidiFile.o MidiClip.o MidiTrack.o AudioFile.o AudioClip.o AudioTrack.o SLClip.o SLBus.o Project.o State.o Waiter.o Listener.o
OBJS = $(EXT_OBJS) $(MAIN_OBJS) $(GUI_OBJS) Kaliboulat.o

SUFFIXES = .cpp .o
.SUFFIXES: $(SUFFIXES) .

all: Kaliboulat jacktest
#all: Kaliboulat wavrepair
#new: Kaliboulat-rtmidi2.0.1
#old: Kaliboulat-rtmidi1.0.15

Kaliboulat: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(JACKFLAGS) -o $@ $(OBJS) $(LDLIBS) $(JACKLIBS)

jacktest: jacktest.cpp
	g++ -I/usr/include/jack jacktest.cpp -o jacktest -ljack

#wavrepair: wavrepair.cpp
#	$(CC) $(CFLAGS) -I/usr/include/stk $@.cpp -o $@ -lstk

#Kaliboulat-rtmidi1.0.15: $(OBJS)
#	$(CC) $(CFLAGS) $(LDFLAGS) $(ALSAFLAGS) -o $@ $(OBJS) $(LDLIBS) $(ALSALIBS)

#tests: MidiClip.o tests.o
#	$(CC) $(CFLAGS) $(LDFLAGS) $(JACKFLAGS) -o $@ MidiClip.o tests.o $(LDLIBS) $(JACKLIBS)
#
#tests.o: MidiClip.hpp
	
#AUTODEPS
AudioClip.o: AudioClip.hpp State.hpp Project.hpp Clip.hpp 
AudioFile.o: AudioFile.hpp RessourceFile.hpp 
AudioTrack.o: spdlog/spdlog.h State.hpp AudioTrack.hpp AudioClip.hpp Track.hpp 
Clip.o: spdlog/spdlog.h Clip.hpp midi.hpp globals.hpp midi.hpp 
Clock.o: spdlog/spdlog.h Clock.hpp Waiter.hpp 
globals.o: spdlog/spdlog.h globals.hpp 
GUI.o: spdlog/spdlog.h GUI.hpp State.hpp Project.hpp MidiFile.hpp MidiClip.hpp AudioFile.hpp AudioClip.hpp Listener.hpp Clock.hpp imgui/imgui.h imgui/imgui_impl_sdl.h globals.hpp 
Kaliboulat.o: spdlog/spdlog.h midi.hpp Clock.hpp State.hpp Project.hpp Listener.hpp AudioTrack.hpp GUI.hpp Clock.hpp MidiTrack.hpp MidiFile.hpp Project.hpp GUI.hpp 
Listener.o: Listener.hpp spdlog/spdlog.h 
MidiClip.o: spdlog/spdlog.h Clock.hpp midi.hpp MidiClip.hpp MidiFile.hpp Project.hpp State.hpp Clip.hpp midi.hpp 
midi.o: spdlog/spdlog.h midi.hpp State.hpp Clip.hpp Scheduled.hpp Waiter.hpp 
MidiFile.o: spdlog/spdlog.h MidiFile.hpp MidiClip.hpp RessourceFile.hpp 
MidiTrack.o: MidiTrack.hpp State.hpp MidiClip.hpp Track.hpp 
Project.o: spdlog/spdlog.h Project.hpp Clock.hpp State.hpp AudioFile.hpp AudioTrack.hpp SLBus.hpp MidiFile.hpp MidiTrack.hpp Track.hpp 
RessourceFile.o: RessourceFile.hpp globals.hpp 
Scheduled.o: Scheduled.hpp 
SLBus.o: spdlog/spdlog.h State.hpp SLBus.hpp SLClip.hpp Track.hpp 
SLClip.o: spdlog/spdlog.h SLClip.hpp SLBus.hpp State.hpp Project.hpp Clip.hpp 
State.o: spdlog/spdlog.h globals.hpp State.hpp cereal/types/vector.hpp cereal/types/string.hpp 
Track.o: spdlog/spdlog.h Track.hpp Clip.hpp 
Waiter.o: spdlog/spdlog.h Waiter.hpp Project.hpp State.hpp Clock.hpp midi.hpp 
#AUTODEPS

.cpp.o:
	$(CC) $(CFLAGS) $(JACKFLAGS) -c $*.cpp

clean:
	rm $(MAIN_OBJS) $(GUI_OBJS) Kaliboulat.o
