CXX=g++
CXXFLAGS=-I../include -msse3 -Wall -O0 #-mavx -Wall -O0
LDFLAGS=-lportaudio -lportmidi
SOURCES=main.cpp ../src/ImpactForce.cpp ../src/ModalBank.cpp \
		../src/Voice.cpp ../src/Synthesizer.cpp ../src/MalletSpring.cpp \
		../src/DelayLine.cpp ../src/Tube.cpp ../src/Differentiator.cpp \
		../src/SinOsc.cpp ../src/Utils.cpp SynthEngine.cpp \
		../src/HighPass1P.cpp ../src/Vector3.cpp ../src/Spatializer.cpp
EXECUTABLE=build/app

all: $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) $(LDFLAGS) -o $(EXECUTABLE)
