cmake_minimum_required(VERSION 3.11)
project(SDLOpenGL CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# SDL
FetchContent_Declare(
	sdl
	GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
	GIT_TAG	release-2.0.14
	GIT_PROGRESS TRUE
)

FetchContent_GetProperties(sdl)
if(NOT sdl_POPULATED)
	FetchContent_Populate(sdl)
	add_subdirectory(${sdl_SOURCE_DIR} ${sdl_BINARY_DIR})
endif()

set(SDL_INCLUDE_ITEMS ${sdl_SOURCE_DIR}/include/*.h)
set(SDL_LIBRARY_ITEMS ${sdl_BINARY_DIR}/${CMAKE_BUILD_TYPE}/*.lib)

# SDL_IMAGE
FetchContent_Declare(
	sdl_image
	GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
	GIT_TAG	main
	GIT_PROGRESS TRUE
)

FetchContent_GetProperties(sdl_image)
if(NOT sdl_image_POPULATED)
	FetchContent_Populate(sdl_image)
	add_subdirectory(${sdl_image_SOURCE_DIR} ${sdl_image_BINARY_DIR})
endif()


set(SDL_IMAGE_INCLUDE_ITEMS ${sdl_image_SOURCE_DIR}/include/*.h)
set(SDL_IMAGE_LIBRARY_ITEMS ${sdl_image_BINARY_DIR}/${CMAKE_BUILD_TYPE}/*.lib)

# FREETYPE
FetchContent_Declare(
	freetype
	GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
	GIT_TAG	master
	GIT_PROGRESS TRUE
)

FetchContent_GetProperties(freetype)
if(NOT freetype_POPULATED)
	FetchContent_Populate(freetype)
	add_subdirectory(${freetype_SOURCE_DIR} ${freetype_BINARY_DIR})
endif()

set(FREETYPE_INCLUDE_DIRS ${freetype_SOURCE_DIR}/include/*.h)
set(FREETYPE_LIBRARY ${freetype_BINARY_DIR}/${CMAKE_BUILD_TYPE}/*.lib)

# SDL_TTF
FetchContent_Declare(
	sdl_ttf
	GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
	GIT_TAG	main
	GIT_PROGRESS TRUE
)

FetchContent_GetProperties(sdl_ttf)
if(NOT sdl_ttf_POPULATED)
	FetchContent_Populate(sdl_ttf)
	add_subdirectory(${sdl_ttf_SOURCE_DIR} ${sdl_ttf_BINARY_DIR})
endif()

set(SDL_TTF_INCLUDE_ITEMS ${sdl_ttf_SOURCE_DIR}/include/*.h)
set(SDL_TTF_LIBRARY_ITEMS ${sdl_ttf_BINARY_DIR}/${CMAKE_BUILD_TYPE}/*.lib)

#GLEW

set(GLEW_LIBS "${PROJECT_SOURCE_DIR}/3rd Party/glew-2.1.0/lib/Release/Win32/glew32.lib" "${PROJECT_SOURCE_DIR}/3rd Party/glew-2.1.0/lib/Release/Win32/glew32s.lib")
set(GLEW_INC "${PROJECT_SOURCE_DIR}/3rd Party/glew-2.1.0/include/")

# source files
file(GLOB_RECURSE GAME_SRC ${PROJECT_SOURCE_DIR}/SDLUITest/*.cpp ${PROJECT_SOURCE_DIR}/SDLUITest/*.h )
list(REMOVE_ITEM GAME_SRC ${PROJECT_SOURCE_DIR}/SDLUITest/glm/detail/dummy.cpp)

#combining

set(INCLUDE_LIST ${SDL_INCLUDE_ITEMS} ${SDL_IMAGE_INCLUDE_ITEMS} ${FREETYPE_INCLUDE_DIRS} ${SDL_TTF_INCLUDE_ITEMS} ${GLEW_INC})
set(LIB_LIST ${SDL_LIBRARY_ITEMS} ${SDL_IMAGE_LIBRARY_ITEMS} ${FREETYPE_LIBRARY} ${SDL_TTF_LIBRARY_ITEMS} ${GLEW_LIBS})

#lib defs
target_link_directories(libsdl-org/SDL_image ${SDL_INCLUDE_ITEMS})
target_link_libraries(libsdl-org/SDL_image libsdl-org/SDL)
add_library(libfreetype SHARED ${freetype_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${GAME_SRC})
include_directories(${PROJECT_NAME} ${INCLUDE_LIST})
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${LIB_LIST})


