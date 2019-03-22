# 
# To build on RaspberryPi:
# 1. Install WiringPi: http://wiringpi.com/download-and-install/
# 2. Run:
#    make target=raspberry
#    sudo blynk --token=YourAuthToken
#

CC = clang
CXX = clang++
CXXFLAGS += -I Sources -DLINUX -DRASPBERRY
LDFLAGS += -lrt -lpthread -lwiringPi

ifeq ($(build),release)
	CXXFLAGS += -c -O3 -w
	LDFLAGS += -s
else
	CXXFLAGS += -c -g2 -O0 -Wall -Wextra -pedantic -Werror
	CXXFLAGS += -Wno-vla-extension -Wno-format-security -Wno-unused-function # to make Blynk compile
endif

CXXFLAGS += -I Libraries/Blynk/src -I Libraries/Blynk/linux

SOURCES = Libraries/Blynk/src/utility/BlynkDebug.cpp \
	Libraries/Blynk/src/utility/BlynkHandlers.cpp \
	Libraries/Blynk/src/utility/BlynkTimer.cpp \
	\
	Sources/main.cpp \
	Sources/Core/Sequencer.cpp \
	Sources/Core/Pattern.cpp \
	Sources/Core/Note.cpp \
	Sources/Blynk/BlynkApp.cpp
	
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=sequencer

all: $(SOURCES) $(EXECUTABLE)

clean:
	-rm $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@
