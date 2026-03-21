#!/bin/bash
mkdir -p build
mkdir -p bin

rm -rf build/*
cd build || exit 1

cmake ..
bear -- make

echo "Building project complete!"
