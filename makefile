CC         =g++
SOURCES    =src/main.cpp src/Line.cpp src/Point.cpp src/utils.cpp

UNAME      :=$(shell uname -o)
ifeq ($(UNAME), GNU/Linux)
CFLAGS     =-c -g -O2 -Wall
INCLUDES   =-I"./include"
LINKFLAGS  =-static-libgcc # need g++ >= 4.5 for -static-libstdc++
LIB        =-lglut -lGLU
BUILDDIR   =make-build-linux
endif

ifeq ($(UNAME), Msys)
CFLAGS     =-c -g -O2 -Wall -DFREEGLUT_STATIC
INCLUDES   =-I"./include" -I"./freeglut/include"
LDFLAGS    =-L"./freeglut/lib"
LINKFLAGS  =-static-libgcc -static-libstdc++ -W1,subsystem,windows
LIB        =-lfreeglut_static -lopengl32 -lgdi32 -lglu32 -lwinmm
BUILDDIR   =make-build-win32
endif

MKV        :=$(shell python scripts/describe.py) # create VERSION file
EXE_NAME   :=$(shell python scripts/exe_name.py)
EXECUTABLE =$(BUILDDIR)/$(EXE_NAME).exe

OBJECTS    =$(patsubst src/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(LINKFLAGS) $(OBJECTS) -o $@ $(LIB)
	cp VERSION $(BUILDDIR)
	python scripts/append_date.py $(BUILDDIR)/VERSION

$(OBJECTS):$(BUILDDIR)/%.o:src/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@ $(INCLUDES)
clean:
	rm -rf $(BUILDDIR)
	rm VERSION
