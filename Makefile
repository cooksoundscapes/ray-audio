CC := g++
C_FLAGS := -std=c++17 -Wall #compiler

CPP_FLAGS := -Irapidxml-1.13 -Iinclude -MMD -MP #compiler pre processor

# in case of a internal lib folder, declare it before LIBS!
LIBS := -lraylib -ljack -llo

FILES := $(wildcard src/*.cpp) 
OBJ := $(FILES:src/%.cpp=obj/%.o)

OUTPUT := ray-audio

.PHONY: all clean

all: $(OUTPUT)

clean:
	rm -rf obj && mkdir obj && rm -rf $(OUTPUT)

$(OUTPUT): $(OBJ) $(C_OBJ)
	$(CC) $^ $(LIBS) -o $@

obj/%.o: src/%.cpp | obj
	$(CC) $(CPP_FLAGS) $(C_FLAGS) -c $< -o $@

