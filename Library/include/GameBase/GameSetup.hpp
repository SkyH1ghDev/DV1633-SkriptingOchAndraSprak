#pragma once

namespace Library::GameBase
{
    class GameSetup
    {
    public:
        GameSetup() = default;
        ~GameSetup() = default;
        GameSetup(const GameSetup& other) = default;
        GameSetup& operator=(const GameSetup& other) = default;
        GameSetup(GameSetup&& other) noexcept = default;
        GameSetup& operator=(GameSetup&& other) noexcept = default;
        
    };
}