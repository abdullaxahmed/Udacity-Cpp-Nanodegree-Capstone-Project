cmake_minimum_required(VERSION 3.10)
project(car-tracker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

file(GLOB project_SRCS src/*.cpp) 

add_executable(car-tracker ${project_SRCS})
target_link_libraries(car-tracker ${OpenCV_LIBS})