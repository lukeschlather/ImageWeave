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
SIM = Similar
endif

OBJS =  ImageSet$(OBJ) Loom$(OBJ) LJFS_Utils$(OBJ)
CC = g++
DEBUG = -g
OPTIMIZE = -O3
RUN = $(OPTIMIZE)
#RUN = $(DEBUG)
CFLAGS = -Wall -Wextra  -c $(RUN) $(OSFLAGS) $(OSCFLAGS)
LFLAGS = -Wall -Wextra  $(RUN) $(OSFLAGS) $(OSLFLAGS)

# ifeq ($(OSTYPE),win32)
# CC = cl
# CFLAGS = /c /DWIN32
# LFLAGS = /Fe$(TARGET) ../platforms/win/freeglut_static.lib opengl32.lib user32.lib gdi32.lib kernel32.lib
# endif


$(TARGET) : $(OBJS) main$(OBJ)
ifeq ($(OSTYPE),win32)
	$(CC) $(LFLAGS) $(OBJS) 
else
	$(CC)  $(OBJS) main$(OBJ) -o $(TARGET) $(LFLAGS) 
endif

$(SIM) : $(OBJS) Similarity.cpp
	$(CC)  $(OBJS) Similarity.cpp -o $(SIM) $(LFLAGS) 

main$(OBJ) : main.cpp ImageSet.h
	$(CC) $(CFLAGS) main.cpp

Loom$(OBJ) : Loom.h Loom.cpp
	$(CC) $(CFLAGS) Loom.cpp

ImageSet$(OBJ) : ImageSet.h ImageSet.cpp
	$(CC) $(CFLAGS) ImageSet.cpp

LJFS_Utils$(OBJ): LJFS_Utils.h LJFS_Utils.cpp
	$(CC) $(CFLAGS) LJFS_Utils.cpp
clean:
	-$(RM) -f *$(OBJ) *~ $(TARGET) $(SIM)





#g++ -o ImageWeave ImageWeave.cpp -O2

#-o hello_word hello_world.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
