cmake_minimum_required(VERSION 3.10)
project(car-tracker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(car-tracker src/main.cpp)
target_link_libraries(car-tracker PRIVATE ${OpenCV_LIBS})

# cmake_minimum_required(VERSION 3.11.3)

# # set(CMAKE_CXX_STANDARD 17)
# project(car-tracker)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

# find_package(OpenCV 4.1 REQUIRED)

# include_directories(${OpenCV_INCLUDE_DIRS})
# link_directories(${OpenCV_LIBRARY_DIRS})
# add_definitions(${OpenCV_DEFINITIONS})

# # Find all executables
# file(GLOB project_SRCS src/*.cpp) #src/*.h

# # Add project executable
# add_executable(car-tracker ${project_SRCS})
# target_link_libraries(car-tracker ${OpenCV_LIBRARIES})