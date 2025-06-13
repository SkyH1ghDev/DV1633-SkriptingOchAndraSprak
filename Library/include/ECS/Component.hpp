#pragma once

namespace Library::ECS
{
    
}
    class Component
    {
    public:
        Component() = default;
        ~Component() = default;
        Component(const Component& other) = default;
        Component& operator=(const Component& other) = default;
        Component(Component&& other) noexcept = default;
        Component& operator=(Component&& other) noexcept = default;
        
};
