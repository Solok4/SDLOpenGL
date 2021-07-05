cmake_minimum_required(VERSION 2.6)

project(SDLOpenGL)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE GAME_SRC ${PROJECT_SOURCE_DIR}/SDLUITest/*.cpp ${PROJECT_SOURCE_DIR}/SDLUITest/*.h )
list(REMOVE_ITEM GAME_SRC ${PROJECT_SOURCE_DIR}/SDLUITest/glm/detail/dummy.cpp)

set(GLEW_LIBS "${PROJECT_SOURCE_DIR}/3rd Party/glew-2.1.0/lib/Release/Win32/glew32.lib" "${PROJECT_SOURCE_DIR}/3rd Party/glew-2.1.0/lib/Release/Win32/glew32s.lib")
set(SDL2IMAGE_LIBS "${PROJECT_SOURCE_DIR}/3rd Party/SDL2_image-2.0.4/lib/x86/SDL2_image.lib")
set(SDL2TTF_LIBS "${PROJECT_SOURCE_DIR}/3rd Party/SDL2_ttf-2.0.14/lib/x86/SDL2_ttf.lib")
set(SDL2_LIBS "${PROJECT_SOURCE_DIR}/3rd Party/SDL2-2.0.8/lib/x86/SDL2.lib" "${PROJECT_SOURCE_DIR}/3rd Party/SDL2-2.0.8/lib/x86/SDL2main.lib" "${PROJECT_SOURCE_DIR}/3rd Party/SDL2-2.0.8/lib/x86/SDL2test.lib")
set(LIBS ${GLEW_LIBS} ${SDL2IMAGE_LIBS} ${SDL2TTF_LIBS} ${SDL2_LIBS})

set(GLEW_INC "3rd Party/glew-2.1.0/include/")
set(SDL2IMAGE_INC "3rd Party/SDL2_image-2.0.4/include/")
set(SDL2TTF_INC "3rd Party/SDL2_ttf-2.0.14/include/")
set(SDL2_INC "3rd Party/SDL2-2.0.8/include/")
set(INCS ${GLEW_INC} ${SDL2IMAGE_INC} ${SDL2TTF_INC} ${SDL2_INC})

add_executable(SDLUITest ${GAME_SRC})
include_directories(SDLUITest ${PROJECT_SOURCE_DIR} ${INCS})
target_link_libraries(SDLUITest ${LIBS})