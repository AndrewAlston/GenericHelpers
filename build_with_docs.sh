#!/bin/bash
cmake -G ninja .
ninja
cd docs/latex
make
cd ../..
