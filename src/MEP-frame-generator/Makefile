ICXX = icpc
GCXX = /usr/linux-k1om-4.7/bin/x86_64-k1om-linux-g++
CXX=$(ICXX)
INC=-I../../include
CFLAGS=-lrt $(INC)
CXXFILES=eventFrameGenerator.cpp fragmentGeneratorTest.cpp
PROG_NAME=mep-frame-gen
GXX_NAME=$(PROG_NAME)-gxx

# typing 'make' will invoke the first target entry in the file (in this case
# the default target entry) you can name this target entry anything, but
# "default" or "all" are the most commonly used names by convention
#
default: $(PROG_NAME)

$(PROG_NAME):
	$(CXX) $(CFLAGS) $(CXXFILES) -o $(PROG_NAME).exe

$(GXX_NAME):
	$(GCXX) $(CFLAGS) $(CXXFILES) -o $(GXX_NAME).exe

# To start over from scratch, type 'make clean'.  This removes the executable
# file, as well as old .o object files and *~ backup files:
#
clean:
	$(RM) $(PROG_NAME).exe $(GXX_NAME).exe *.o *~
