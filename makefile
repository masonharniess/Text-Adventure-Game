CC=g++

CFLAGS=-std=c++11 #Compiler flags
SOURCES=main.cpp GameConfig.cpp GameEngine.cpp GameState.cpp
OUTPUT=main

# Target file
all: $(OUTPUT)

# Linking all files to the executableFile
$(OUTPUT): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(OUTPUT)

# clean functions
clean:
	rm -f $(OUTPUT)

deepclean:
	rm -f *~ $(OBJS) $(TARGET) *.stackdump