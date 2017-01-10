CC = g++
FLAGS = -g -std=c++11

INCLUDE = -I/usr/X11R6/include -I/usr/include/GL -I/usr/include
LIBDIR = -L/usr/X11R6/lib -L/usr/local/lib
SOURCES = *.cpp
LIBS = -lGLEW -lGL -lGLU -lglut -lm -lpng

EXENAME = smoother

smoother: $(SOURCES)
	$(CC) $(FLAGS) -o $(EXENAME) $(INCLUDE) $(LIBDIR) $(SOURCES) $(LIBS)

.PHONY: run
run:
	./$(EXENAME)

.PHONY: clean
clean:
	rm -f *.o $(EXENAME) core smoother

.PHONY: all
all: clean smoother
