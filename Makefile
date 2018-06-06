# A simple makefile for CSE 100 PA3

CC=g++
CXXFLAGS=-std=c++11 -g
LDFLAGS=-g

all: compress uncompress

compress: BitInputStream.o BitOutputStream.o HCNode.o HCTree.o

uncompress: BitInputStream.o BitOutputStream.o HCNode.o HCTree.o

HCTree.o: BitInputStream.h BitOutputStream.h HCNode.h HCTree.h

HCNode.o: HCNode.h

BitOutputStream.o: BitOutputStream.h

BitInputStream.o: BitInputStream.h

clean:
	rm -f compress uncompress *.o core*