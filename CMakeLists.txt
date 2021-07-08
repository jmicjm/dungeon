﻿cmake_minimum_required (VERSION 3.8)
project ("dungeon")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32)
	set (SFML_DIR "C:/SFML-2.5.1/lib/cmake/SFML/")
endif()

find_package(SFML 2 COMPONENTS graphics REQUIRED)

add_executable(dungeon
	src/game.cpp 
	src/asset_storage/texture_bank.cpp
	src/asset_storage/tile_sprite_storage.cpp 
	src/gfx/animated_sprite.cpp
	src/gfx/animated_sprite.cpp
	src/gfx/animated_sprite_frames.cpp 
	src/level/level_structure.cpp
	src/level_gen/level_structure_generator.cpp 
	src/level_gen/level_structure_decorator.cpp
	src/utils/rand.cpp
)

target_link_libraries(dungeon sfml-graphics)