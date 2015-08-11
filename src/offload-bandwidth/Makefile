CC = icpc
CFLAGS = -lrt

# typing 'make' will invoke the first target entry in the file (in this case
# the default target entry) you can name this target entry anything, but
# "default" or "all" are the most commonly used names by convention
#
default: offload-bandwidth

offload-bandwidth:
	$(CC) $(CFLAGS) main.cpp -o offload-bandwidth.exe

# To start over from scratch, type 'make clean'.  This removes the executable
# file, as well as old .o object files and *~ backup files:
#
clean:
	$(RM) offload-bandwidth.exe *.o *~
