TARGET_EXEC := program

BUILD_DIR := ./obj
SRC_DIRS := ./src

# Find all the C and C++ files we want to compile
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' | grep -o '[^/]*\.cpp')
SRCS += glad.c

# String substitution for every C/C++ file.
# As an example, hello.cpp turns into ./build/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := $(INC_FLAGS) -g


all: $(TARGET_EXEC) shader
	
$(TARGET_EXEC): bin/$(TARGET_EXEC)
	./bin/$(TARGET_EXEC)

# The final build step.
#bin/$(TARGET_EXEC): $(OBJS)
#	$(CC) $(OBJS) -o $@ $(LDFLAGS)

bin/$(TARGET_EXEC): $(OBJS)
	echo $(SRCS)
	g++ $(OBJS) -o $@ -W -Wall -Werror -lGLEW -lglut -lglfw -lGL -ldl -I./include

# Build step for C source
$(BUILD_DIR)/%.c.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@ -I./include

# Build step for C++ source
$(BUILD_DIR)/%.cpp.o: src/%.cpp
	#mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ -I./include

shader: parametrizacion.in
	cd ./procesador/ && $(MAKE)
	cd ..

clean:
	rm -rf bin/* obj/*
	cd ./procesador/ && $(MAKE) clean
	cd ..
