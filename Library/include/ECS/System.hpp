#pragma once

namespace Library::ECS
{
    class System
    {
    public:
        System() = default;
        virtual ~System() = default;
        System(const System& other) = default;
        System& operator=(const System& other) = default;
        System(System&& other) noexcept = default;
        System& operator=(System&& other) noexcept = default;

    public:
        /**
         * Performs update logic of system
         */
        virtual void Update() = 0;
        
    };   
}

