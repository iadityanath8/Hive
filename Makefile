CXX = g++ 
VER = --std=c++20
CXXFLAGS = -fno-exceptions 

# Target
TARGET = main

# Source files
SRC = main.cc
# LINK = hive.cpp
build:
	$(CXX) $(VER) $(CXXFLAGS) -o $(TARGET) $(SRC) && ./$(TARGET)

run:
	./$(TARGET)
# Clean up
clean:
	rm -f $(TARGET) $(OBJ)
