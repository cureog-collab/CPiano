CC = gcc

TARGET = build/piano.exe

CPPFLAGS = -Iinclude -Ivendor
CFLAGS += -O3
CFLAGS += -Wall -Wextra
CFLAGS += -MMD -MP

LDLIBS = -luser32 -lm

SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c,build/%.o,$(SOURCES))
DEPENDENCIES = $(OBJECTS:.o=.d)

.PHONY: all run clean rebuild

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDLIBS)

build/%.o: src/%.c | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

build:
	mkdir build

run: $(TARGET)
	$(TARGET)

clean:
	$(RM) -r build

rebuild: clean all

-include $(DEPENDENCIES)