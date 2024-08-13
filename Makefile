CC = g++
PROJECT = SmartScreen
OBJS = SmartScreen.o DetectSkin.o BackRemover.o DetectFace.o CountFinger.o Interface.o
CXXFLAGS = -std=c++17 -Wall -Wextra -fPIC $(shell pkg-config --cflags opencv4 Qt5Widgets)
LIBS = $(shell pkg-config --libs opencv4 Qt5Widgets)

all: $(PROJECT)

$(PROJECT): $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS)

SmartScreen.o: SmartScreen.cpp DetectSkin.h
	$(CC) $(CXXFLAGS) -c SmartScreen.cpp

DetectSkin.o: DetectSkin.cpp DetectSkin.h
	$(CC) $(CXXFLAGS) -c DetectSkin.cpp

BackRemover.o: BackRemover.cpp BackRemover.h
	$(CC) $(CXXFLAGS) -c BackRemover.cpp

DetectFace.o: DetectFace.cpp DetectFace.h
	$(CC) $(CXXFLAGS) -c DetectFace.cpp

CountFinger.o: CountFinger.cpp CountFinger.h
	$(CC) $(CXXFLAGS) -c CountFinger.cpp

Interface.o: Interface.cpp Interface.h
	$(CC) $(CXXFLAGS) -c Interface.cpp

clean:
	rm -f $(PROJECT) $(OBJS)
