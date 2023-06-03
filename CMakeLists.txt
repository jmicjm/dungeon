cmake_minimum_required (VERSION 3.8)
project ("dungeon")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32)
    set (SFML_DIR "C:/SFML-2.5.1/lib/cmake/SFML/")
    set(PYTHON_EXECUTABLE python)
else()
    set(PYTHON_EXECUTABLE python3)
endif()

find_package(SFML 2 COMPONENTS graphics REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(Boost REQUIRED)


add_executable(dungeon
    src/game.cpp
    src/asset_storage/texture_bank.cpp
    src/asset_storage/font_bank.cpp
    src/asset_storage/tile_sprite_storage.cpp
    src/asset_storage/entity_animation_storage.cpp
    src/asset_storage/item_rc_storage.cpp
    src/gfx/primitive_sprite.cpp
    src/gfx/animated_sprite/animated_sprite.cpp
    src/gfx/animated_sprite/animated_sprite_frames.cpp
    src/gfx/level_tile_map/level_tile_map.cpp
    src/gfx/level_tile_map/level_tile_map_chunk.cpp
    src/gfx/combined_texture.cpp
    src/gfx/view_follower.cpp
    src/gfx/view_range_overlay.cpp
    src/gfx/copyable_rendertexture.cpp
    src/gfx/utils/setQuad.cpp
    src/gfx/utils/mergeTexture.cpp
    src/gfx/utils/visibleAreaBounds.cpp
    src/level/level.cpp
    src/level/level_structure.cpp
    src/level/tile_reveal_mask.cpp
    src/level/tile_visibility_info.cpp
    src/level/visibleTiles.cpp 
    src/level/moveEntity.cpp 
    src/level_gen/level_structure_generator.cpp 
    src/level_gen/level_structure_decorator.cpp
    src/level_gen/createLevelStructure.cpp
    src/entities/cloneEntity.cpp
    src/entities/player.cpp
    src/entities/door.cpp
    src/entities/gate.cpp
    src/entities/portal.cpp
    src/entities/chest.cpp
    src/entities/npc/smuggler.cpp
    src/entities/npc/infected_zombie.cpp
    src/entities/items/createItem.cpp
    src/gui/gui_component.cpp 
    src/gui/surface.cpp
    src/gui/primitives/button/button.cpp   
    src/gui/primitives/gauge_bar/gauge_bar.cpp
    src/gui/primitives/frame/frame.cpp  
    src/gui/primitives/scroll/scroll.cpp
    src/gui/primitives/text/text.cpp
    src/gui/primitives/list/list.cpp
    src/gui/hud/player_frame.cpp 
    src/gui/hud/stat_bar.cpp
    src/gui/hud/quick_select.cpp 
    src/gui/hud/controls.cpp 
    src/gui/hud/hud.cpp
    src/gui/component_stack/component_stack.cpp
    src/gui/component_stack/component_stack_helper.cpp
    src/gui/inventory/player_inventory.cpp
    src/gui/inventory/inventory.cpp
    src/gui/inventory/dual_inventory.cpp
    src/gui/inventory/item_swapper.cpp
    src/gui/inventory/common/drawItem.cpp
    src/gui/world_context_menu/world_context_menu.cpp
    src/gui/world_context_menu/entity_context_menu.cpp
    src/input/input.cpp
    src/utils/utf8ToUtf32.cpp
    src/world/world.cpp 
    src/components/position.cpp  
    src/components/inventory.cpp
    src/components/stackable_item.cpp 
    src/components/body/body_part.cpp
)

add_custom_target(
        run ALL
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/embed_assets.py ${CMAKE_SOURCE_DIR}
        BYPRODUCTS src/generated_assets/textures.h
        COMMENT "Generating assets"
)

add_dependencies(dungeon run)

target_link_libraries(dungeon sfml-graphics EnTT::EnTT Boost::boost)