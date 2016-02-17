CXX=g++
CFLAGS=-std=c++0x
PROG_NAME=statistics-calculator

# typing 'make' will invoke the first target entry in the file (in this case
# the default target entry) you can name this target entry anything, but
# "default" or "all" are the most commonly used names by convention
#
default: statistics-calculator

statistics-calculator:
	$(CXX) $(CFLAGS) main.cpp -o $(PROG_NAME)

# To start over from scratch, type 'make clean'.  This removes the executable
# file, as well as old .o object files and *~ backup files:
#
clean:
	$(RM) $(PROG_NAME) *.o *~
