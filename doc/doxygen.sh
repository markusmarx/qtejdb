#!/bin/sh
# Create *.png files from text @startuml code and output *.png images to ./doc-src/images folder
# Recusively search from current folder scanning *.c, *.cpp, *.h, and *.doc files
plantuml -v -o $PWD/images  "../src/**.(c|cpp|doc|h|php)"
doxygen
