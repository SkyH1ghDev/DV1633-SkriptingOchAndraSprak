#pragma once
#include <entt/entity/registry.hpp>

namespace Library::ECS
{
    class Registry
    {
    public:
        Registry() = default;
        ~Registry() = default;
        Registry(const Registry& other) = delete;
        Registry& operator=(const Registry& other) = delete;
        Registry(Registry&& other) noexcept = default;
        Registry& operator=(Registry&& other) noexcept = default;

    public:
        /**
         * Returns the amount of entities currently present in the entity registry
         * @return Entity Count
         */
        int GetEntityCount();

        /**
         * Creates an entity and appends it to the registry
         */
        void CreateEntity();

        /**
         * Returns whether an entity is linked to the specified index
         * @param pEntityIndex The index of an entity
         * @return true when entity exists, else false
         */
        bool IsEntity(const int pEntityIndex);

        /**
         * Removes the entity at the specified index
         * @param pEntityIndex The index of an entity
         */
        void RemoveEntity(const int pEntityIndex);
        
    private:
        entt::registry registry;
    };
}
