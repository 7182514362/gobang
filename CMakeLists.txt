cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(GobangApp VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# debug
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_subdirectory(src)
list(APPEND EXTRA_LIBS Gobang)

# add the executable
add_executable(app main.cpp)

# install app to bin
INSTALL(FILES ${PROJECT_SOURCE_DIR}/build/app.exe DESTINATION ${PROJECT_SOURCE_DIR}/bin)

target_link_libraries(app PUBLIC ${EXTRA_LIBS})
target_include_directories(app PUBLIC ${PROJECT_BINARY_DIR} "${PROJECT_SOURCE_DIR}/src")