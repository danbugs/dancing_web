# Copyright 2021 Danilo Chiarlone
# LICENSE: GNU AFFERO GENERAL PUBLIC LICENSE
# NOTE: This Makefile was written for POSIX-compliant systems

P=dcw
OBJECTS=src/dcw.c src/dcw.js src/dcw.h
EMCC=emcc
EMCC_CFLAGS=-s SIDE_MODULE=2 -c
BUILDDIR=build
DEPENDENCIES=

$(P): $(OBJECTS)
	rm -rf $(BUILDDIR)
	mkdir -p $(BUILDDIR)
	cp src/$(P).js $(BUILDDIR)/$(P).js
	cp src/$(P).h $(BUILDDIR)/$(P).h
	$(EMCC) $(EMCC_CFLAGS) $(DEPENDENCIES) src/$(P).c -o $(BUILDDIR)/$(P).o