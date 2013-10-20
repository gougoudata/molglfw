UNAME := $(shell uname)

CC=g++

# CFLAGS for Linux
ifeq ($(UNAME), Linux)
LFLAGS=-lglut -lGL -lGLU -lglfw -g
endif

# CFLAGS for Mac OS
ifeq ($(UNAME), Darwin)
LFLAGS=-framework IOKit -framework Cocoa -framework OpenGL -lglfw \
-framework GLUT -I/opt/X11/include
endif

CFLAGS=-Wall -O3 -c -g
SOURCES=strfunc.cpp readfile.cpp molecule.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=molglfw

all: $(EXECUTABLE)

$(EXECUTABLE): main.cpp $(OBJECTS)
	$(CC) main.cpp -o $(EXECUTABLE) $(LFLAGS) $(OBJECTS)

%.o: %.cpp
	$(CC) $(LIBS) $(CFLAGS) $(DEBUG) $< -o $@

clean:
	rm molglfw *.o
