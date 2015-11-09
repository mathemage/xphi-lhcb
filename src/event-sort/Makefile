ICXX = icpc
GCXX = /usr/linux-k1om-4.7/bin/x86_64-k1om-linux-g++
CXX=$(ICXX)
INC_DIR=../../include
CFLAGS=-g -O2 -lrt -I$(INC_DIR) -openmp -std=c++14
ICFLAGS=$(CFLAGS) -qopt-report3 -qopt-report-phase=vec
PREF_OFF_DIR=..
CXXFILES=main.cpp $(PREF_OFF_DIR)/prefix-sum.cpp ../utils.cpp
PROG_NAME=event-sort
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

knc:
	$(CXX) $(ICFLAGS) -mmic $(CXXFILES) -o $(PROG_NAME).knc.exe

knc-gcc:
	$(GCXX) $(CFLAGS) $(CXXFILES) -o $(GXX_NAME).knc.exe

knl:
	$(CXX) $(ICFLAGS) -xMIC_AVX512 $(CXXFILES) -o $(PROG_NAME).knc.exe

# To start over from scratch, type 'make clean'.  This removes the executable
# file, as well as old .o object files and *~ backup files:
#
clean:
	$(RM) $(PROG_NAME).exe $(GXX_NAME).exe $(GXX_NAME).knc.exe \
	$(PROG_NAME).knc.exe *.o *.optrpt *~
