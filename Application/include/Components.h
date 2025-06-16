#pragma once

#include <raylib/raylib.h>

struct Position
{
	float x = 0, y = 0;
};

struct Size
{
	float width, height;
};

struct Scale
{
	float x = 1.0f, y = 1.0f;
};

struct Velocity
{
	float x = 0, y = 0;
};

struct World
{
	Size size = { 800.0f, 600.0f }; // default world size
	::Vector2 gravity = { 0.0f, 1000.0f }; // default gravity
};



// a component to identify the player state
struct PlayerController
{
	bool isGrounded = false;
	bool isJumping = false;
};

// A tag component to identify terrain entities
// https://docs.unity3d.com/Packages/com.unity.entities@1.0/manual/components-tag.html
struct Terrain
{
};


struct TextureComponent
{
	::Texture2D texture;
};

struct TexturedStatic // for static sprites
{
	::Texture2D texture;
	::Rectangle sourceRect; // source rectangle in the texture atlas
	::Rectangle destRect;   // destination rectangle on the screen
};

struct TexturedDynamic // for animated sprites
{
	::Rectangle sourceRect; // source rectangle in the texture atlas
	::Rectangle destRect;   // destination rectangle on the screen
	::Texture2D texture;
	int offset = 0;
};