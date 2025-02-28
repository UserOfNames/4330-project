main: src/main.c interpret.o
	gcc src/main.c obj/interpret.o -Wall -Wextra -Wpedantic -o bin/main

interpret.o: src/interpret.c
	gcc -c src/interpret.c -Wall -Wextra -Wpedantic -o obj/interpret.o
