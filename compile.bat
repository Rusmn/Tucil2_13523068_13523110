@echo off
mkdir bin 2>nul
g++ -std=c++11 -Wall -o bin\main src\error_metrics.cpp src\image_utils.cpp src\main.cpp src\quadtree.cpp -lm
echo Compilation complete!
echo Run with: bin\main