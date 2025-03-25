# Large parts of this were derived from https://makefiletutorial.com/#makefile-cookbook
# ...which in turn credits https://spin.atomicobject.com/makefile-c-projects/

CC := gcc
LDFLAGS := -Wall -Wextra -Wpedantic

MAIN_SOURCE := src/main.c
MAIN_OBJECT := obj/src/main.o

TEST_MAIN_SOURCE := tests/test_main.c
TEST_MAIN_OBJECT := obj/tests/test_main.o

# src/main.c is excluded so its main won't conflict with main() defined in
# tests/test_main.c
SOURCES := $(shell find src -name '*.c' -not -name 'main.c')
TEST_SOURCES := $(shell find tests -name '*.c')

OBJECTS := $(patsubst src/%.c,obj/src/%.o,$(SOURCES))
TEST_OBJECTS := $(patsubst tests/%.c,obj/tests/%.o,$(TEST_SOURCES))

DEPS := $(OBJECTS:.o=.d) $(TEST_OBJECTS:.o=.d) $(MAIN_OBJECT:.o=.d) $(TEST_MAIN_OBJECT:.o=.d)

INC_DIRS := $(shell find ./src -type d) $(shell find ./tests -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS := $(INC_FLAGS) -MMD -MP

$(shell mkdir -p bin obj)

bin/main: $(OBJECTS) $(MAIN_OBJECT)
	$(CC) $(LDFLAGS) $(OBJECTS) $(MAIN_OBJECT) -o $@

obj/src/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(MAIN_OBJECT): $(MAIN_SOURCE)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: test
test: bin/test
	bin/test

bin/test: $(OBJECTS) $(TEST_OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(TEST_OBJECTS) -o $@

obj/tests/%.o: tests/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean
clean:
	rm -r bin
	rm -r obj

-include $(DEPS)
