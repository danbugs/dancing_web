P=form
OBJECTS=src/main.c
EMCC=emcc
EMCC_CFLAGS=-s MAIN_MODULE=1 --js-library dependencies/dcw/dcw.js -s "EXPORTED_RUNTIME_METHODS=['allocateUTF8']"
BUILDDIR=build
DEPENDENCIES=dependencies/dcw/dcw.o 

$(P): $(OBJECTS)
	$(EMCC) $(EMCC_CFLAGS) $(DEPENDENCIES) src/main.c -o $(BUILDDIR)/$(P).js