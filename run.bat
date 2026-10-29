@echo off

cmake -S . -B build -G "MinGW Makefiles" && cmake --build build
./build/bin/mp3

