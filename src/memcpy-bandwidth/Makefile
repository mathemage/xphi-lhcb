ICXX = icpc
GCXX = /usr/linux-k1om-4.7/bin/x86_64-k1om-linux-g++
CXX=$(ICXX)
INC_DIR=../../include
CFLAGS=-g -O2 -lrt -I$(INC_DIR) -openmp -std=c++14
ICFLAGS=$(CFLAGS) -qopt-report3 -qopt-report-phase=vec
PREF_OFF_DIR=..
CXXFILES=main.cpp ../utils.cpp
PROG_NAME=memcpy-bandwidth
GXX_NAME=$(PROG_NAME).gxx

# typing 'make' will invoke the first target entry in the file (in this case
# the default target entry) you can name this target entry anything, but
# "default" or "all" are the most commonly used names by convention
#
default: $(PROG_NAME)

$(PROG_NAME):
	$(CXX) $(ICFLAGS) $(CXXFILES) -o $(PROG_NAME).exe

$(GXX_NAME):
	$(GCXX) $(CFLAGS) $(CXXFILES) -o $(GXX_NAME).exe

mic:
	$(CXX) $(ICFLAGS) -mmic $(CXXFILES) -o $(PROG_NAME).mic.exe

mic-gcc:
	$(GCXX) $(CFLAGS) $(CXXFILES) -o $(GXX_NAME).mic.exe

# To start over from scratch, type 'make clean'.  This removes the executable
# file, as well as old .o object files and *~ backup files:
#
clean:
	$(RM) $(PROG_NAME).exe $(GXX_NAME).exe $(GXX_NAME).mic.exe \
	$(PROG_NAME).mic.exe *.o *.optrpt *~
