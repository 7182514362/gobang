cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(GobangApp VERSION 1.0)

# set (CMAKE_CXX_FLAGS "-m32")

# debug
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_subdirectory(src)


# add the executable
add_executable(app main.cpp)

# install app to bin
INSTALL(FILES ${PROJECT_SOURCE_DIR}/build/app.exe DESTINATION ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/src)
target_link_libraries(app Gobang)
