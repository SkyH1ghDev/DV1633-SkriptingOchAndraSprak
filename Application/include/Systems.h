#pragma once

#include <LuaJIT/lua.hpp>
#include <entt/entt.hpp>
#include <raylib/raylib.h>

#include "Components.h"
#include "Utility.h"


// === SYSTEMS ===

inline
void PhysicsSystem(entt::registry& registry, float delta)
{
    // Apply gravity to any entity that has a velocity
    constexpr float gravity = 1000.0f;
    const auto view = registry.view<Position, Velocity>();

    view.each([&](entt::entity entity, Position& pos, Velocity& vel)
    {
        // Apply gravity
        vel.y += gravity * delta;

        // Update position
        pos.x += vel.x * delta;
        pos.y += vel.y * delta;
    });
}

inline
void CollisionSystem(entt::registry& registry)
{
    const auto playerView = registry.view<Position, Velocity, Size, PlayerController>();
    const auto terrainView = registry.view<const Position, const Size, const Terrain>();

    // for each player entity (scales with mplayer)
    playerView.each([&](Position& playerPos, Velocity& playerVel, Size& playerSize, PlayerController& controller)
    {
        controller.isGrounded = false;
        // todo: register a constant collision rect for player
		Rectangle collRect = { playerPos.x, playerPos.y, playerSize.width, playerSize.height };

        // ...check against every terrain entity
        // TODO: implement some kind of proximity culling
        terrainView.each([&](const Position& terrainPos, const Size& terrainSize)
        {
            Rectangle terrainRect = {terrainPos.x, terrainPos.y, terrainSize.width, terrainSize.height};
            if (CheckCollisionRecs(collRect, terrainRect))
            {
                // Simple collision resolution:
                // If the player is moving down and collides, stop them on top of the platform.
                if (playerVel.y > 0)
                {
                    playerVel.y = 0;
                    playerPos.y = terrainRect.y - playerSize.height; // Snap to top
                    controller.isGrounded = true;
                }
            }
        });
    });
}

inline
void RenderSystem(entt::registry& registry)
{
    BeginDrawing();
    ClearBackground(SKYBLUE);

 //   // draw untextured objects
 //   auto untextured_view = registry.view<const Position, const Size>(entt::exclude<const TextureComponent>);
	//untextured_view.each([](const Position& pos, const Size& size)
	//	{
	//		::DrawRectangle(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(size.width),
	//			static_cast<int>(size.height), GRAY);
	//	});

 //   // draw textured objects
	//auto textured_view = registry.view<const Position, const Size, const TextureComponent>();


    registry.view<const Position, const Size, const Terrain>()
            .each([](const Position& pos, const Size& size)
            {
                ::DrawRectangle(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(size.width),
                                static_cast<int>(size.height), BROWN);
            });

    // Draw player
    registry.view<const Position, const Size, const PlayerController>()
            .each([](const Position& pos, const Size& size, const PlayerController&)
            {
                ::DrawRectangle(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(size.width),
                                static_cast<int>(size.height), GREEN);
            });

    // Draw instructions
    DrawText("Use Arrow Keys to Move, SPACE to Jump", 10, 10, 20, BLACK);

    // DEBUG: Draw player position
    auto playerView = registry.view<const Position, const PlayerController>();
    playerView.each([](const Position& pos, const PlayerController&)
    {
        DrawText(TextFormat("Player Position: (%.2f, %.2f)", pos.x, pos.y), 10, 40, 20, BLACK);
    });


    EndDrawing();
}
