#!/bin/bash
cmake -G Ninja .
ninja clean
ninja
cd docs/latex
make
cd ../..
