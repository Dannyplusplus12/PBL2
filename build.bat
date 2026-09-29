@echo off
g++ -std=c++11 main.cpp -o main.exe
if errorlevel 1 (echo Build FAILED) else (echo Build OK: main.exe)
