# necessary files to get the executables
_DEPS = condVar.h recurMutex.h
_OBJ = condVar.o recurMutex.o
_MOBJ = main.o
_TOBJ = test.o

# the executables needed for running and testing
APPBIN = lock_app
TESTBIN = lock_test

IDIR = include
CC = g++
# normal compilation include flags needed + include path???
# $() works similar to define to extract what to put in place of IDIR
CFLAGS = -I$(IDIR) -Wall -Wextra -g -pthread
ODIR = obj
SDIR = src
LDIR = lib
TDIR = test
LIBS = -lm
# testing include flags needed
XXLIBS = $(LIBS) -lstdc++ -lgtest -lgtest_main -lpthread

# getting a list of header files, object files, main object file, test object file
# patsubst stands for pattern substitution, % substitures everything (WHY IS THIS NEEDED HOWEVER)
# the % functions like the * in *.o
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
MOBJ = $(patsubst %,$(ODIR)/%,$(_MOBJ))
TOBJ = $(patsubst %,$(ODIR)/%,$(_TOBJ)) 

# 1 - object files in src directory (doesn't form them until these files activated as dependencies)
$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# 2 - object files in test directory (doesn't form them until these files activated as dependencies)
$(ODIR)/%.o: $(TDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# 5 - FIRST TARGET: forces formation of both lock_app and lock_test
all: $(APPBIN) $(TESTBIN)

# 3 - main executable
$(APPBIN): $(OBJ) $(MOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# 4 - test executable
$(TESTBIN): $(TOBJ) $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(XXLIBS)

# -f flag ignores case where specified files do not exist
clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
	rm -f $(APPBIN) $(TESTBIN)
	rm -f submission.zip

.PHONY: clean

# check that dependency tree is correct to limit #recompilation