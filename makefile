# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = g++
CFLAGS  = -g -Wall -Weffc++ -std=c++11
LFLAGS  = -L/usr/lib

# All Targets
all: studio


# Tool invocations
# Executable "studio" depends on the files Studio.o Action.o Trainer.o Customer.o Workout.o main.o
studio: bin/Studio.o bin/Action.o bin/Trainer.o bin/Customer.o bin/Workout.o bin/main.o
	@echo 'Building target: studio'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/studio bin/Studio.o bin/Action.o  bin/Trainer.o bin/Customer.o bin/Workout.o bin/main.o $(LFLAGS)
	@echo 'Finished building target: studio'
	@echo ' '

# Depends on the source and header files
bin/Studio.o: src/Studio.cpp include/Studio.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Studio.o src/Studio.cpp

# Depends on the source and header files
bin/Action.o: src/Action.cpp include/Action.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Action.o src/Action.cpp

# Depends on the source and header files
bin/Trainer.o: src/Trainer.cpp include/Trainer.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Trainer.o src/Trainer.cpp

# Depends on the source and header files
bin/Customer.o: src/Customer.cpp include/Customer.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Customer.o src/Customer.cpp

# Depends on the source and header files
bin/Workout.o: src/Workout.cpp include/Workout.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Workout.o src/Workout.cpp

# Depends on the source and header files
bin/main.o: src/main.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/main.o src/main.cpp

#Clean the build directory
clean:
	rm -f bin/*
