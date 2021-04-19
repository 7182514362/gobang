

# set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
# find_package(SDL2 REQUIRED)
# target_include_directories(Gobang ${SDL2_INCLUDE_DIR})
# target_link_libraries(Gobang ${SDL2_LIBRARY})


#set(SDL2_LIBRARY "D:/Dev/SDL/MinGW/SDL2-dev/lib/libSDL2main.a"
#                "D:/Dev/SDL/MinGW/SDL2-dev/bin/SDL2.dll"
#                "D:/Dev/SDL/MinGW/SDL2-dev/bin/SDL2_image.dll"
#                "D:/Dev/SDL/MinGW/SDL2-dev/bin/SDL2_ttf.dll")

# SET(MINGW32_LIBRARY mingw32 CACHE STRING "mwindows for MinGW")

# set(SDL2_INCLUDE_DIR "D:/Dev/SDL/MinGW/SDL2-dev/include")
# target_include_directories(Gobang PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${SDL2_INCLUDE_DIR})
# target_link_directories(Gobang PUBLIC "D:/Dev/SDL/SDL2-dev/lib/")

# SDL2main SDL2 SDL2_image SDL2_ttf SDL2_mixer
# target_link_libraries(Gobang ${MINGW32_LIBRARY} ${SDL2_LIBRARY})

add_library(Gobang Gobang.cpp Texture.cpp Window.cpp Board.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set(extralib D:/Dev/SDL/MinGW/SDL2-dev-64)

#include_directories(${extralib}/include)
target_include_directories(Gobang PUBLIC ${extralib}/include)
target_link_directories(Gobang PUBLIC ${extralib}/lib)

target_link_libraries(Gobang mingw32 SDL2main SDL2 SDL2_image)

