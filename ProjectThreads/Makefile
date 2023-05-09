run_main:
	g++ src/main.cpp

# -f flag ignores case where specified files do not exist
clean:
	rm -f *.o
	rm -f *.out

.PHONY: clean

# want to make object files first - faster and easier compilation
# check dependencies - might help to get rid of relative paths in main