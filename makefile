# compiler
CXX = g++
# flags: C++17 standard, -I<dir>
CXXFLAGS = -std=c++17 -Iinclude
# target executable name
TARGET = final
# source dir
SRC_DIR = src

# find all .cpp files in src_dir
SRC = $(wildcard $(SRC_DIR)/*.cpp)
# convert to objects
OBJ = $(SRC:.cpp=.o)

# default build target
all: $(TARGET)

# pattern for building the executable from object files
# $@ evaluates to target name / $^ .o
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# pattern for compiling .cpp to .o
# $< evaluates to src file name / $@ target file name
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# remove all object files
clean:
	rm -f $(OBJ) $(TARGET)