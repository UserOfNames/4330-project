# Large parts of this were derived from https://makefiletutorial.com/#makefile-cookbook
# ...which in turn credits https://spin.atomicobject.com/makefile-c-projects/

CC = gcc
LDFLAGS = -Wall -Wextra -Wpedantic

SOURCES := $(shell find src -name '*.c')

OBJECTS := $(patsubst src/%.c,obj/%.o,$(SOURCES))

DEPS := $(OBJECTS:.o=.d)

INC_DIRS := $(shell find ./src -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS := $(INC_FLAGS) -MMD -MP

$(shell mkdir -p bin obj)

bin/main: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

obj/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean
clean:
	rm -r bin
	rm -r obj

-include $(DEPS)
