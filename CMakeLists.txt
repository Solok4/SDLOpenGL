cmake_minimum_required(VERSION 3.4)

project(SDLOpenGL)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

<<<<<<< HEAD
option(BUILD_X64 OFF)

file(GLOB_RECURSE GAME_SRC ${PROJECT_SOURCE_DIR}/SDLUITest/*.cpp ${PROJECT_SOURCE_DIR}/SDLUITest/*.h )
list(REMOVE_ITEM GAME_SRC ${PROJECT_SOURCE_DIR}/SDLUITest/glm/detail/dummy.cpp)

if(BUILD_X64)
message("Using x64 build")
set(GLEW_LIBS "${PROJECT_SOURCE_DIR}/3rdParty/glew-2.1.0/lib/Release/x64/glew32.lib" "${PROJECT_SOURCE_DIR}/3rdParty/glew-2.1.0/lib/Release/x64/glew32s.lib")
set(SDL2IMAGE_LIBS "${PROJECT_SOURCE_DIR}/3rdParty/SDL2_image-2.0.4/lib/x64/SDL2_image.lib")
set(SDL2TTF_LIBS "${PROJECT_SOURCE_DIR}/3rdParty/SDL2_ttf-2.0.14/lib/x64/SDL2_ttf.lib")
set(SDL2_LIBS "${PROJECT_SOURCE_DIR}/3rdParty/SDL2-2.0.8/lib/x64/SDL2.lib" "${PROJECT_SOURCE_DIR}/3rdParty/SDL2-2.0.8/lib/x64/SDL2main.lib" "${PROJECT_SOURCE_DIR}/3rdParty/SDL2-2.0.8/lib/x64/SDL2test.lib")
else()
message("Using x86 build")
set(GLEW_LIBS "${PROJECT_SOURCE_DIR}/3rdParty/glew-2.1.0/lib/Release/Win32/glew32.lib" "${PROJECT_SOURCE_DIR}/3rdParty/glew-2.1.0/lib/Release/Win32/glew32s.lib")
set(SDL2IMAGE_LIBS "${PROJECT_SOURCE_DIR}/3rdParty/SDL2_image-2.0.4/lib/x86/SDL2_image.lib")
set(SDL2TTF_LIBS "${PROJECT_SOURCE_DIR}/3rdParty/SDL2_ttf-2.0.14/lib/x86/SDL2_ttf.lib")
set(SDL2_LIBS "${PROJECT_SOURCE_DIR}/3rdParty/SDL2-2.0.8/lib/x86/SDL2.lib" "${PROJECT_SOURCE_DIR}/3rdParty/SDL2-2.0.8/lib/x86/SDL2main.lib" "${PROJECT_SOURCE_DIR}/3rdParty/SDL2-2.0.8/lib/x86/SDL2test.lib")
endif(BUILD_X64)


set(LIBS ${GLEW_LIBS} ${SDL2IMAGE_LIBS} ${SDL2TTF_LIBS} ${SDL2_LIBS})

set(GLEW_INC "3rdParty/glew-2.1.0/include/")
set(SDL2IMAGE_INC "3rdParty/SDL2_image-2.0.4/include/")
set(SDL2TTF_INC "3rdParty/SDL2_ttf-2.0.14/include/")
set(SDL2_INC "3rdParty/SDL2-2.0.8/include/")
=======
option(RPI4 "RPI4 build" OFF)

file(GLOB_RECURSE GAME_SRC ${PROJECT_SOURCE_DIR}/SDLUITest/*.cpp ${PROJECT_SOURCE_DIR}/SDLUITest/*.h )
list(REMOVE_ITEM GAME_SRC ${PROJECT_SOURCE_DIR}/SDLUITest/glm/detail/dummy.cpp)
if(RPI4)
    message("RPI4 build")
    set(GLEW_LIBS "${PROJECT_SOURCE_DIR}/3rd Party/glew-2.1.0/lib/arm64/libGLEW.so")
    set(SDL2IMAGE_LIBS "${PROJECT_SOURCE_DIR}/3rd Party/SDL2_image-2.0.4/lib/arm64/libSDL2_image.so")
    set(SDL2TTF_LIBS "${PROJECT_SOURCE_DIR}/3rd Party/SDL2_ttf-2.0.14/lib/arm64/libSDL2_ttf.so")
    set(SDL2_LIBS "${PROJECT_SOURCE_DIR}/3rd Party/SDL2-2.0.8/lib/arm64/libSDL2.so" "${PROJECT_SOURCE_DIR}/3rd Party/SDL2-2.0.8/lib/arm64/libSDL2main.a" "${PROJECT_SOURCE_DIR}/3rd Party/SDL2-2.0.8/lib/arm64/libSDL2_test.a")
    set(GL_LIBS "/usr/lib/libGL.so.1")
else()
    message("Regular build")
    set(GLEW_LIBS "${PROJECT_SOURCE_DIR}/3rd Party/glew-2.1.0/lib/Win32/glew32.lib" "${PROJECT_SOURCE_DIR}/3rd Party/glew-2.1.0/lib/Win32/glew32s.lib")
    set(SDL2IMAGE_LIBS "${PROJECT_SOURCE_DIR}/3rd Party/SDL2_image-2.0.4/lib/x86/SDL2_image.lib")
    set(SDL2TTF_LIBS "${PROJECT_SOURCE_DIR}/3rd Party/SDL2_ttf-2.0.14/lib/x86/SDL2_ttf.lib")
    set(SDL2_LIBS "${PROJECT_SOURCE_DIR}/3rd Party/SDL2-2.0.8/lib/x86/SDL2.lib" "${PROJECT_SOURCE_DIR}/3rd Party/SDL2-2.0.8/lib/x86/SDL2main.lib" "${PROJECT_SOURCE_DIR}/3rd Party/SDL2-2.0.8/lib/x86/SDL2test.lib")
    set(GL_LIBS "")
endif()
set(LIBS ${GLEW_LIBS} ${SDL2IMAGE_LIBS} ${SDL2TTF_LIBS} ${SDL2_LIBS} ${GL_LIBS})

set(GLEW_INC "3rd Party/glew-2.1.0/include/")
set(SDL2IMAGE_INC "3rd Party/SDL2_image-2.0.4/include/")
set(SDL2TTF_INC "3rd Party/SDL2_ttf-2.0.14/include/")
set(SDL2_INC "3rd Party/SDL2-2.0.8/include/")

>>>>>>> 92a497c (Rpi4 tests - first successful compilation)
set(INCS ${GLEW_INC} ${SDL2IMAGE_INC} ${SDL2TTF_INC} ${SDL2_INC})

unset(RPI4 CACHE)

add_executable(SDLUITest ${GAME_SRC})
include_directories(SDLUITest ${PROJECT_SOURCE_DIR} ${INCS})
target_link_libraries(SDLUITest ${LIBS})