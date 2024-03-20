# Compiler
CC = gcc

# Compiler Flags
CFLAGS = -Wall -Wextra -std=c99

# Source Files
SOURCES = character.c clan.c inventory.c item.c job.c main.c menu.c rarity.c type.c

# Object Files
OBJECTS = $(SOURCES:.c=.o)

# Executable Output
EXECUTABLE = Build/InteractiveDatabaseManager

all: $(EXECUTABLE)

$(shell mkdir -p Build)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c defs.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

