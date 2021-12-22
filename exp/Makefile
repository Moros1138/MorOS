PROJECT     := experiments

CXX         := g++

RELEASE     := -O3
DEBUG       := -ggdb3 -Og

STATIC      := -Bstatic -static-libgcc -static-libstdc++
SHARED      := -static-libstdc++

BUILD       := $(DEBUG) $(STATIC)
# BUILD      := $(RELEASE)

BIN         := bin
SRC         := src
INC         := include
LIB         := lib
OBJ         := obj
RES         := res

CXX_FLAGS   := $(BUILD) -I../STL/include -Wall -Wextra -Wpedantic -std=c++17

#LIBRARIES   := -lX11 -lGL -lpthread -lpng -lstdc++fs 

SOURCES     := $(wildcard $(SRC)/*.cpp)
OBJECTS     := $(patsubst $(SRC)/%,$(OBJ)/%,$(SOURCES:.cpp=.o))

.PHONY: clean all

all: dirs $(BIN)/$(PROJECT) 

dirs:
	@mkdir -p obj
	@mkdir -p bin

# Compile only
$(OBJ)/%.o : $(SRC)/%.cpp $(DEPENDENCIES)
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

# Link the object files and libraries
$(BIN)/$(PROJECT) : $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(BIN)/$(PROJECT) $(CXX_FLAGS) $(LIBRARIES)

clean:
	rm -rf $(BIN)/$(PROJECT) $(OBJ)/*.o
