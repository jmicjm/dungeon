#include "cloneEntity.h"

entt::entity cloneEntity(entt::registry& registry, entt::entity to_clone)
{
    auto new_entity = registry.create();
    for (auto&& [id, storage] : registry.storage())
    {
        if (storage.contains(to_clone))
        {
            storage.emplace(new_entity, storage.get(to_clone));
        }
    }
    return new_entity;
}
