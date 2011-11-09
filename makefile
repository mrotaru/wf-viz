CC=g++
CFLAGS     =-c -g -O2 -Wall -DFREEGLUT_STATIC
INCLUDES   =-I"./include" -I"./freeglut/include"
LDFLAGS    =-L"./freeglut/lib"
SOURCES    =src/main.cpp src/Line.cpp src/Point.cpp
LINKFLAGS  =-static-libgcc -static-libstdc++ -W1,subsystem,windows
LIB        =-lfreeglut_static -lopengl32 -lgdi32 -lglu32 -lwinmm
BUILDDIR   =make-build
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
	rm $(BUILDDIR)/wf-viz-make.exe