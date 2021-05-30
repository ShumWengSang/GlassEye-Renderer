#!/bin/sh
# Entry point to building the engine + game.
echo off 

echo ================= Glass Eye Engine Init Script ============================

mkdir build -p

# git submodule clone all external third party libraries
git submodule update --init --recursive

# Run cmake with x64 in ./build folder
cmake . -A x64 -B build
