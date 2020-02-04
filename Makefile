build:
	gcc src/main.c -o bin/OBJ-Loader

clean:
	del bin\OBJ-Loader.exe

run:
	bin/OBJ-Loader