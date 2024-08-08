CC = "g++"
PROJECT= SmartScreen
OBJS = SmartScreen.o DetectSkin.o BackRemover.o DetectFace.o CountFinger.o
SRC= SmartScreen.cpp 
CXXFLAGS = -std=c++17 -Wall -Wextra

LIBS= `pkg-config opencv4 --cflags --libs`

all: $(PROJECT)

$(PROJECT) : $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS)

SmartScreen.o: SmartScreen.cpp DetectSkin.h
	$(CXX) $(CXXFLAGS) -c SmartScreen.cpp

DetectSkin.o: DetectSkin.cpp DetectSkin.h
	$(CXX) $(CXXFLAGS) -c DetectSkin.cpp

BackRemover.o: BackRemover.cpp BackRemover.h
	$(CXX) $(CXXFLAGS) -c BackRemover.cpp

DetectFace.o: DetectFace.cpp DetectFace.h
	$(CXX) $(CXXFLAGS) -c DetectFace.cpp

CountFinger.o: CountFinger.cpp CountFinger.h
	$(CXX) $(CXXFLAGS) -c CountFinger.cpp

clean:
	rm -f $(TARGET) $(OBJS)