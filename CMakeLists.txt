cmake_minimum_required(VERSION 3.4)

project(SDLOpenGL)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BUILD_X64 OFF)
option(RPI4 "RPI4 build" ON)

file(GLOB_RECURSE GAME_SRC ${PROJECT_SOURCE_DIR}/SDLUITest/*.cpp ${PROJECT_SOURCE_DIR}/SDLUITest/*.h )
list(REMOVE_ITEM GAME_SRC ${PROJECT_SOURCE_DIR}/SDLUITest/glm/detail/dummy.cpp)

if(RPI4)
    message("Using RPI4 build params")
    set(GLEW_LIBS "${PROJECT_SOURCE_DIR}/3rdParty/glew-2.1.0/lib/Release/arm64/libGLEW.so")
    set(SDL2IMAGE_LIBS "${PROJECT_SOURCE_DIR}/3rdParty/SDL2_image-2.0.4/lib/arm64/libSDL2_image.so")
    set(SDL2TTF_LIBS "${PROJECT_SOURCE_DIR}/3rdParty/SDL2_ttf-2.0.14/lib/arm64/libSDL2_ttf.so")
    set(SDL2_LIBS "${PROJECT_SOURCE_DIR}/3rdParty/SDL2-2.0.8/lib/arm64/libSDL2.so" "${PROJECT_SOURCE_DIR}/3rdParty/SDL2-2.0.8/lib/arm64/libSDL2main.a" "${PROJECT_SOURCE_DIR}/3rdParty/SDL2-2.0.8/lib/arm64/libSDL2_test.a")
    set(GL_LIBS "/usr/lib/libGL.so.1")
elseif(BUILD_X64)
message("Using WINDOWS x64 build params")
    set(GLEW_LIBS "${PROJECT_SOURCE_DIR}/3rdParty/glew-2.1.0/lib/Release/x64/glew32.lib" "${PROJECT_SOURCE_DIR}/3rdParty/glew-2.1.0/lib/Release/x64/glew32s.lib")
    set(SDL2IMAGE_LIBS "${PROJECT_SOURCE_DIR}/3rdParty/SDL2_image-2.0.4/lib/x64/SDL2_image.lib")
    set(SDL2TTF_LIBS "${PROJECT_SOURCE_DIR}/3rdParty/SDL2_ttf-2.0.14/lib/x64/SDL2_ttf.lib")
    set(SDL2_LIBS "${PROJECT_SOURCE_DIR}/3rdParty/SDL2-2.0.8/lib/x64/SDL2.lib" "${PROJECT_SOURCE_DIR}/3rdParty/SDL2-2.0.8/lib/x64/SDL2main.lib" "${PROJECT_SOURCE_DIR}/3rdParty/SDL2-2.0.8/lib/x64/SDL2test.lib")
    set(GL_LIBS "")
else()
message("Using WINDOWS x86 build params")
    set(GLEW_LIBS "${PROJECT_SOURCE_DIR}/3rdParty/glew-2.1.0/lib/Release/Win32/glew32.lib" "${PROJECT_SOURCE_DIR}/3rdParty/glew-2.1.0/lib/Release/Win32/glew32s.lib")
    set(SDL2IMAGE_LIBS "${PROJECT_SOURCE_DIR}/3rdParty/SDL2_image-2.0.4/lib/x86/SDL2_image.lib")
    set(SDL2TTF_LIBS "${PROJECT_SOURCE_DIR}/3rdParty/SDL2_ttf-2.0.14/lib/x86/SDL2_ttf.lib")
    set(SDL2_LIBS "${PROJECT_SOURCE_DIR}/3rdParty/SDL2-2.0.8/lib/x86/SDL2.lib" "${PROJECT_SOURCE_DIR}/3rdParty/SDL2-2.0.8/lib/x86/SDL2main.lib" "${PROJECT_SOURCE_DIR}/3rdParty/SDL2-2.0.8/lib/x86/SDL2test.lib")
    set(GL_LIBS "")
endif()

set(LIBS ${GLEW_LIBS} ${SDL2IMAGE_LIBS} ${SDL2TTF_LIBS} ${SDL2_LIBS} ${GL_LIBS})

set(GLEW_INC "3rdParty/glew-2.1.0/include/")
set(SDL2IMAGE_INC "3rdParty/SDL2_image-2.0.4/include/")
set(SDL2TTF_INC "3rdParty/SDL2_ttf-2.0.14/include/")
set(SDL2_INC "3rdParty/SDL2-2.0.8/include/")
set(GLM_INC "3rdParty/glm/")


set(INCS ${GLEW_INC} ${SDL2IMAGE_INC} ${SDL2TTF_INC} ${SDL2_INC} ${GLM_INC})

unset(RPI4 CACHE)
unset(BUILD_X64 CACHE)

add_executable(SDLUITest ${GAME_SRC})
include_directories(SDLUITest ${PROJECT_SOURCE_DIR} ${INCS})
target_link_libraries(SDLUITest ${LIBS})