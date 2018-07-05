SOURCES = $(wildcard sources/*.cpp)
HEADERS = $(wildcard sources/*.h)
OBJECTS = $(SOURCES:sources/%.cpp=objects/%.o)
CXX_FLAGS = -Wall -std=c++11 -D NDEBUG -O3
# CXX_FLAGS = -Wall -std=c++11 -D DEBUG -g -g3
TARGET = target/main

.PHONY: all
all: directory $(TARGET)
.PHONY: directory
directory:
	mkdir -p objects
	mkdir -p target
$(TARGET): $(OBJECTS) objects/main.o
	g++ $(CXX_FLAGS) $^ -o $@
objects/%.o: sources/%.cpp sources/%.h
	@echo Compiling $<...
	g++ -c $(CXX_FLAGS) $< -o $@
objects/main.o: sources/main.cxx
	@echo Compiling $<...time 
	g++ -c $(CXX_FLAGS) $< -o $@
.PHONY: clean
.PHONY: cleanObjects
.PHONY: cleanTarget
clean:
	rm -rf objects/
	rm -rf target/
.PHONY: rebuild
rebuild: clean all
.PHONY: run
run: $(TARGET)
	./target/main
