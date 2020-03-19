ROOT = $(realpath .)
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

PROJECT_NAME = Engine
BINARIES_PATH = $(ROOT)/binary
SRC_PATH = $(ROOT)/src

# Define the names of key files
SOURCE_FILE = $(call rwildcard, $(SRC_PATH),*.cpp)
OBJECT_FILE = $(BINARIES_PATH)/*.o 
EXECUTABLE = $(PROJECT_NAME)
WINDOW_SOURCE = libs/sdw/DrawingWindow.cpp
WINDOW_OBJECT = libs/sdw/DrawingWindow.o

# Build settings
COMPILER = g++
COMPILER_OPTIONS = -c -pipe -Wall -std=c++11 -I./src
DEBUG_OPTIONS = -ggdb -g3
FUSSY_OPTIONS = -pedantic
# FUSSY_OPTIONS = -Werror -pedantic
SANITIZER_OPTIONS = -O1 -fsanitize=undefined -fsanitize=address -fno-omit-frame-pointer
SPEEDY_OPTIONS = -Ofast -funsafe-math-optimizations -march=native
LINKER_OPTIONS =

# Set up flags
SDW_COMPILER_FLAGS := -I./libs/sdw
GLM_COMPILER_FLAGS := -I./libs/glm
SDL_COMPILER_FLAGS := $(shell sdl2-config --cflags)
SDL_LINKER_FLAGS := $(shell sdl2-config --libs)
SDW_LINKER_FLAGS := $(WINDOW_OBJECT)

default: diagnostic

# Rule to help find errors (when you get a segmentation fault)
diagnostic: window
	$(COMPILER) $(COMPILER_OPTIONS) $(FUSSY_OPTIONS) $(SANITIZER_OPTIONS) $(SOURCE_FILE) $(SDL_COMPILER_FLAGS) $(SDW_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)
	mv *.o binary/
	$(COMPILER) $(LINKER_OPTIONS) $(FUSSY_OPTIONS) $(SANITIZER_OPTIONS) -o $(EXECUTABLE) $(OBJECT_FILE) $(SDL_LINKER_FLAGS) $(SDW_LINKER_FLAGS)
	./$(EXECUTABLE)

# Rule to compile and link for production release
production: window
	$(COMPILER) $(COMPILER_OPTIONS) -o $(OBJECT_FILE) $(SOURCE_FILE) $(SDL_COMPILER_FLAGS) $(SDW_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)
	$(COMPILER) $(LINKER_OPTIONS) -o $(EXECUTABLE) $(OBJECT_FILE) $(SDL_LINKER_FLAGS) $(SDW_LINKER_FLAGS)
	.$(EXECUTABLE)

# Rule to compile and link for use with a debugger
debug: window
	$(COMPILER) $(COMPILER_OPTIONS) $(DEBUG_OPTIONS) -o $(OBJECT_FILE) $(SOURCE_FILE) $(SDL_COMPILER_FLAGS) $(SDW_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)
	$(COMPILER) $(LINKER_OPTIONS) $(DEBUG_OPTIONS) -o $(EXECUTABLE) $(OBJECT_FILE) $(SDL_LINKER_FLAGS) $(SDW_LINKER_FLAGS)
	./$(EXECUTABLE)

# Rule to build for high performance executable
speedy: window
	$(COMPILER) $(COMPILER_OPTIONS) $(SPEEDY_OPTIONS) -o $(OBJECT_FILE) $(SOURCE_FILE) $(SDL_COMPILER_FLAGS) $(SDW_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)
	$(COMPILER) $(LINKER_OPTIONS) $(SPEEDY_OPTIONS) -o $(EXECUTABLE) $(OBJECT_FILE) $(SDL_LINKER_FLAGS) $(SDW_LINKER_FLAGS)
	./$(EXECUTABLE)

# Rule for building the DisplayWindow
window:
	$(COMPILER) $(COMPILER_OPTIONS) -o $(WINDOW_OBJECT) $(WINDOW_SOURCE) $(SDL_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)

# Files to remove during clean
clean:
	rm $(OBJECT_FILE)
	rm $(EXECUTABLE)
