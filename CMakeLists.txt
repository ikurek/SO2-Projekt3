cmake_minimum_required(VERSION 3.10)
project(SO2_Projekt3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-pthread -lncurses")

add_executable(SO2_Projekt3 main.cpp Person.cpp Person.h Utils.cpp Utils.h Statistics.cpp Statistics.h)