# Compiler
CXX = g++

# Compiler flags
#CXXFLAGS = -Wall -Wextra -std=c++17 -O3
CXXFLAGS = -std=c++17

# SFML paths and libraries
SFML_LIBDIR = -L/usr/lib/x86_64-linux-gnu/sfml
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Target executable
TARGET = main

# Source and object files
SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(SFML_LIBDIR) $(SFML_LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean

