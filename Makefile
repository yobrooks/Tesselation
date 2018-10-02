#COMMENTS
#YASMINE BROOKS
#DR.POUNDS CSC 315
#OCTOBER 2, 2018

CC = gcc
C++ = g++
LIBDIRS = -L/usr/lib64
INCDIRS = -I/usr/include
LDLIBS =  -lglut -lGL -lGLU -lX11 -lm
.c:
	$(CC)  $@.c $(INCDIRS) $(LIBDIRS) $(LDLIBS) -o $@
.cpp:
	$(C++)  -O $@.cpp -g $(INCDIRS) $(LIBDIRS) $(LDLIBS) -o $@
