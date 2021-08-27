cmake_minimum_required (VERSION 3.8)
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
    src/gfx/primitive_sprite.cpp
    src/gfx/animated_sprite/animated_sprite.cpp
    src/gfx/animated_sprite/animated_sprite_frames.cpp
    src/gfx/level_tile_map/level_tile_map.cpp
    src/gfx/level_tile_map/level_tile_map_chunk.cpp
    src/gfx/level_tile_map/level_tile_map_texture.cpp
    src/gfx/view_follower.cpp
    src/gfx/view_range_overlay.cpp
    src/gfx/utils/setQuad.cpp
    src/gfx/utils/mergeTexture.cpp
    src/level/level.cpp
    src/level/level_structure.cpp
    src/level/tile_reveal_mask.cpp
    src/level/tile_visibility_info.cpp
    src/level_gen/level_structure_generator.cpp 
    src/level_gen/level_structure_decorator.cpp
    src/entities/entity.cpp
    src/entities/player.cpp
    src/gui/gui_element.cpp 
    src/gui/surface.cpp
    src/gui/button.cpp   
    src/gui/gauge_bar.cpp
    src/gui/frame.cpp
    src/input/input.cpp
    src/gui/scroll.cpp
 )

target_link_libraries(dungeon sfml-graphics)