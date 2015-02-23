# 15-462 Computer Graphics
# Makefile for Assn2
# By Raphael Mun
# Updated by Tarun Agarwal
# Sept 22, 2008

GRAPHICS = /afs/cs/academic/class/15462

#INCLUDE = -I$(GRAPHICS)/include
#LIBRARYPATH = -L$(GRAPHICS)/lib -L/usr/X11R6/lib/ -L./
#LIBRARIES = $(LIBRARYPATH) -lGL -lGLU -lglut -lpicio -ltiff -ljpeg -lm -lXi -lXmu

LIBRARIES = -lGL -lGLU -lglut -lpicio -ltiff -ljpeg -lm -lXi -lXmu -L.

COMPILER = g++
COMPILERFLAGS = -O2

# ---------- BEGIN SECTION TO CHANGE AS NEEDED ----------

PROGRAM =	assn2

SOURCE =	starter.cpp

OBJECT =	starter.o

# ---------- END SECTION TO CHANGE AS NEEDED ----------

.cpp.o: 
	$(COMPILER) -c $(COMPILERFLAGS) $<

all: $(PROGRAM)

$(PROGRAM): $(OBJECT)
	$(COMPILER) $(COMPILERFLAGS) -o $(PROGRAM) $(OBJECT) $(LIBRARIES)

clean:
	-rm -rf core *.o *~ .*~ $(PROGRAM)
