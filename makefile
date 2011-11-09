CC         =g++
SOURCES    =src/main.cpp src/Line.cpp src/Point.cpp

UNAME      :=$(shell uname)
ifeq ($(UNAME), Linux)
CFLAGS     =-c -g -O2 -Wall
INCLUDES   =-I"./include"
LINKFLAGS  =-static-libgcc # need g++ >= 4.5 for -static-libstdc++
LIB        =-lglut -lGLU
BUILDDIR   =make-build-linux
endif

ifeq ($(UNAME), MINGW32_NT-5.1)
CFLAGS     =-c -g -O2 -Wall -DFREEGLUT_STATIC
INCLUDES   =-I"./include" -I"./freeglut/include"
LDFLAGS    =-L"./freeglut/lib"
LINKFLAGS  =-static-libgcc -static-libstdc++ -W1,subsystem,windows
LIB        =-lfreeglut_static -lopengl32 -lgdi32 -lglu32 -lwinmm
BUILDDIR   =make-build
endif

OBJECTS    =$(patsubst src/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))
EXECUTABLE =$(BUILDDIR)/wf-viz.exe

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(LINKFLAGS) $(OBJECTS) -o $@ $(LIB)

$(OBJECTS):$(BUILDDIR)/%.o:src/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@ $(INCLUDES)
clean:
	rm -f $(BUILDDIR)/*.o $(BUILDDIR)/*~
	rm $(BUILDDIR)/wf-viz.exe
