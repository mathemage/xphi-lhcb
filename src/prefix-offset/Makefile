CXX = icpc
CFLAGS = -lrt
CXXFILES=main.cpp utils.cpp prefix-sum.cpp

# typing 'make' will invoke the first target entry in the file (in this case
# the default target entry) you can name this target entry anything, but
# "default" or "all" are the most commonly used names by convention
#
default: prefix-offset

prefix-offset:
	$(CXX) $(CFLAGS) $(CXXFILES) -o prefix-offset.exe

mic:
	$(CXX) $(CFLAGS) -mmic $(CXXFILES) -o mic-prefix-offset.exe

# To start over from scratch, type 'make clean'.  This removes the executable
# file, as well as old .o object files and *~ backup files:
#
clean:
	$(RM) prefix-offset.exe *.o *~
