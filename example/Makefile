P=hello_world
OBJECTS=hello_world.c
EMCC=emcc
EMCC_CFLAGS=-s MAIN_MODULE=1 --pre-js pre.js --js-library build/dcw.js

$(P): $(OBJECTS)
	$(EMCC) $(P).c $(EMCC_CFLAGS)