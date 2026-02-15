# Compiler and flags
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
LIBS = -lglfw.3.4

# Directories
SRC_DIR = Source
BUILD_DIR = Build
DEPS_DIR = Dependencies
INCLUDE_DIRS = -I$(DEPS_DIR)/include
LIB_DIRS = -L$(DEPS_DIR)/lib

# Files
TARGET = $(BUILD_DIR)/Program
SOURCES = $(SRC_DIR)/Main.cpp $(SRC_DIR)/Shader.cpp $(SRC_DIR)/Texture.cpp $(SRC_DIR)/Camera.cpp
OBJECTS = $(BUILD_DIR)/Main.o $(BUILD_DIR)/Shader.o $(BUILD_DIR)/Texture.o $(BUILD_DIR)/Camera.o $(BUILD_DIR)/glad.o $(BUILD_DIR)/stb_image.o 

# Default target
all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# CPP FILES

$(BUILD_DIR)/Main.o: $(SRC_DIR)/Main.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

$(BUILD_DIR)/Shader.o: $(SRC_DIR)/Shader.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

$(BUILD_DIR)/Texture.o: $(SRC_DIR)/Texture.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

$(BUILD_DIR)/Camera.o: $(SRC_DIR)/Camera.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

# THIRD PARTY CPP FILES

$(BUILD_DIR)/glad.o: $(DEPS_DIR)/glad.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

$(BUILD_DIR)/stb_image.o: $(DEPS_DIR)/stb_image.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

# Link the program
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LIB_DIRS) $(LDFLAGS) $(LIBS) -o $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

# Rebuild everything
rebuild: clean all

.PHONY: all run clean rebuild