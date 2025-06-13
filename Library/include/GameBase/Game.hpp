#pragma once
#include <entt/entity/registry.hpp>

namespace Library::GameBase
{
    class Game
    {
    public:
        Game() = default;
        ~Game() = default;
        Game(const Game& other) = delete;
        Game& operator=(const Game& other) = delete;
        Game(Game&& other) noexcept = default;
        Game& operator=(Game&& other) noexcept = default;
        
    public:
        /**
         * Entry point for the game
         */
        static void Run();

    private:
        entt::registry m_registry;
    };
}
