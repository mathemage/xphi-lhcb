CXX=g++
CFLAGS=-std=c++0x -g
ICXX=icpc
ICFLAGS=-std=c++14 -g
PROG_NAME=median-calculator

# typing 'make' will invoke the first target entry in the file (in this case
# the default target entry) you can name this target entry anything, but
# "default" or "all" are the most commonly used names by convention
#
default: $(PROG_NAME)-icpc

$(PROG_NAME)-gcc:
	$(CXX) $(CFLAGS) main.cpp -o $(PROG_NAME)-gcc

$(PROG_NAME)-icpc:
	$(ICXX) $(ICFLAGS) main.cpp -o $(PROG_NAME)-icpc

# To start over from scratch, type 'make clean'.  This removes the executable
# file, as well as old .o object files and *~ backup files:
#
clean:
	$(RM) $(PROG_NAME)-gcc $(PROG_NAME)-icpc  *.o *~
