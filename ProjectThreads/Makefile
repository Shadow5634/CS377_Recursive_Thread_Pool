run_main:
	g++ -pthread -Wall src/main.cpp src/recurMutex.cpp src/condVar.cpp
# Wall is to enable all the warnings
# -f flag ignores case where specified files do not exist
clean:
	rm -f *.o
	rm -f *.out

.PHONY: clean

# want to make object files first - faster and easier compilation
# check dependencies - might help to get rid of relative paths in main