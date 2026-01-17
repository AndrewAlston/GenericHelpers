#!/bin/bash
cmake -G ninja .
ninja clean
ninja
cd docs/latex
make
cd ../..
