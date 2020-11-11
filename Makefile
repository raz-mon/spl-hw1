# All Targets
all: CoronaVirus

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
CoronaVirus:  clean bin/main.o bin/Session.o bin/Graph.o bin/Agent.o bin/Tree.o
	@echo 'Building target: cTree'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/cTrace  bin/main.o bin/Session.o bin/Graph.o bin/Agent.o bin/Tree.o
	@echo 'Finished building target: cTree'
	@echo ' '

# Depends on the source and header files
bin/main.o: src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp

# Depends on the source and header files
bin/Session.o: src/Session.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Session.o src/Session.cpp

# Depends on the source and header files
bin/Graph.o: src/Graph.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Graph.o src/Graph.cpp

# Depends on the source and header files
bin/Agent.o: src/Agent.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Agent.o src/Agent.cpp

# Depends on the source and header files
bin/Tree.o: src/Tree.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Tree.o src/Tree.cpp

#Clean the build directory
clean:
	rm -f bin/*

# Testing
