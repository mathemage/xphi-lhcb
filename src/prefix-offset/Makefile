ICXX = icpc
GCXX = /usr/linux-k1om-4.7/bin/x86_64-k1om-linux-gcc
CXX=$(ICXX)
INC_DIR=../../include
INC=-I$(INC_DIR)
CFLAGS = -lrt $(INC)
CXXFILES=main.cpp ../utils.cpp ../prefix-sum.cpp

# typing 'make' will invoke the first target entry in the file (in this case
# the default target entry) you can name this target entry anything, but
# "default" or "all" are the most commonly used names by convention
#
default: prefix-offset

prefix-offset:
	$(CXX) $(CFLAGS) $(CXXFILES) -o prefix-offset.exe

mic:
	$(ICXX) $(CFLAGS) -mmic $(CXXFILES) -o mic-prefix-offset.exe

mic-gcc:
	$(GCXX) $(CFLAGS) $(CXXFILES) -o mic-gcc-prefix-offset.exe

# To start over from scratch, type 'make clean'.  This removes the executable
# file, as well as old .o object files and *~ backup files:
#
clean:
	$(RM) prefix-offset.exe *.o *~
