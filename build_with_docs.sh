#!/bin/bash
rm -rf docs/latex
cmake -G Ninja .
ninja clean
ninja
cd docs/latex
make
cd ../..
