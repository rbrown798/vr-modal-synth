CXX=g++
CXXFLAGS=-I../dsp/include -msse3 -Wall -O0 #-mavx -Wall -O0
LDFLAGS=-lportaudio -lportmidi
SOURCES=main.cpp ../dsp/src/ImpactForce.cpp ../dsp/src/ModalBank.cpp \
		../dsp/src/Voice.cpp ../dsp/src/Synthesizer.cpp \
		../dsp/src/MalletSpring.cpp ../dsp/src/DelayLine.cpp \
		../dsp/src/Tube.cpp ../dsp/src/Differentiator.cpp \
		../dsp/src/SinOsc.cpp ../dsp/src/Utils.cpp SynthEngine.cpp \
		../dsp/src/HighPass1P.cpp ../dsp/src/Vector3.cpp \
		../dsp/src/Spatializer.cpp ../dsp/src/LowPass1P.cpp
EXECUTABLE=build/app

all: $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) $(LDFLAGS) -o $(EXECUTABLE)
