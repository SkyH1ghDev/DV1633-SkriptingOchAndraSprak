#pragma once

namespace Library::GameBase
{
    class GameLoop
    {
    public:
        GameLoop() = default;
        ~GameLoop() = default;
        GameLoop(const GameLoop& other) = default;
        GameLoop& operator=(const GameLoop& other) = default;
        GameLoop(GameLoop&& other) noexcept = default;
        GameLoop& operator=(GameLoop&& other) noexcept = default;

    public:
        /**
         * Updates systems every frame
         */
        void UpdateSystems(); // TODO: Abstract away into "SystemHandler"
    };
}