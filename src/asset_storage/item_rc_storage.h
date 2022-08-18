#pragma once
#include "../components/render_component.h"
#include "../entities/items/item_ids.h"
#include <unordered_map>

class Item_rc_storage
{
    static std::unordered_map<items::Item_id, Render_component> rc_map;

public:
    static Render_component getRc(const items::Item_id id);
};