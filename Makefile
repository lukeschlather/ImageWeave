# Haderach
# Makefile

OBJ = .o
RM = rm

ifndef OSTYPE
OSTYPE=linux
endif


ifeq ($(OSTYPE),win32)
TARGET = ImageWeave.exe
OBJ = .obj
RM = del
endif

ifeq ($(OSTYPE),linux)
BOOSTFLAGS = -I/usr/include/boost/filesystem -l:/usr/lib/libboost_filesystem-mt.a -l:/usr/lib/libboost_system-mt.a 
CIMGFLAGS = -L/usr/X11R6/lib -lm -lpthread -lX11 
OSLFLAGS =  ${CIMGFLAGS} ${BOOSTFLAGS}
TARGET = ImageWeave
USEMAGICK = 1
endif

OBJS = main$(OBJ) ImageSet$(OBJ) Loom$(OBJ)
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG) $(OSFLAGS) $(OSCFLAGS)
LFLAGS = -Wall $(DEBUG) $(OSFLAGS) $(OSLFLAGS)

# ifeq ($(OSTYPE),win32)
# CC = cl
# CFLAGS = /c /DWIN32
# LFLAGS = /Fe$(TARGET) ../platforms/win/freeglut_static.lib opengl32.lib user32.lib gdi32.lib kernel32.lib
# endif

# ifdef USEMAGICK
# MAGICKFLAGS = `Magick++-config --cppflags --cxxflags --ldflags --libs` 
# endif

$(TARGET) : $(OBJS)
ifeq ($(OSTYPE),win32)
	$(CC) $(LFLAGS) $(OBJS)
else
	$(CC)  $(OBJS) -o $(TARGET) $(LFLAGS) $(MAGICKFLAGS)
endif

 main$(OBJ) : main.cpp ImageSet.h
	$(CC) $(CFLAGS) main.cpp

# Application$(OBJ) : Application.cpp Application.h Tesselator.h polygon.h fim.h
# 	$(CC) $(CFLAGS) Application.cpp

Loom$(OBJ) : Loom.h Loom.cpp
	$(CC) $(CFLAGS) Loom.cpp
ImageSet$(OBJ) : ImageSet.h ImageSet.cpp
	$(CC) $(CFLAGS) ImageSet.cpp

clean:
	-$(RM) -f *$(OBJ) *~ $(TARGET)





#g++ -o ImageWeave ImageWeave.cpp -O2

#-o hello_word hello_world.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
