build:
	gcc src/main.c -o bin/OBJ-Loader

clean:
	del bin\OBJ-Loader.exe

debug:
	gdb bin/OBJ-Loader

run:
	bin/OBJ-Loader