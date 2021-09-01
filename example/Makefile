P=hello_world
OBJECTS=src/hello_world.c
EMCC=emcc
EMCC_CFLAGS=-s MAIN_MODULE=1 --js-library dependencies/dcw/dcw.js
BUILDDIR=build
DEPENDENCIES=dependencies/dcw/dcw.o 

$(P): $(OBJECTS)
	$(EMCC) $(EMCC_CFLAGS) $(DEPENDENCIES) src/hello_world.c -o $(BUILDDIR)/$(P).js