# Copyright 2021 Danilo Chiarlone
# LICENSE: GNU AFFERO GENERAL PUBLIC LICENSE
# NOTE: This Makefile was written for POSIX-compliant systems

P=dcw
OBJECTS=src/dcw.c src/dcw.js src/dcw.h
EMCC=emcc
EMCC_CFLAGS=-s SIDE_MODULE=2 -O1

$(P): $(OBJECTS)
	mkdir -p dcw_latest
	cp src/$(P).js dcw_latest/$(P).js
	cp src/$(P).h dcw_latest/$(P).h
	$(EMCC) $(EMCC_CFLAGS) src/$(P).c -o dcw_latest/$(P).wasm