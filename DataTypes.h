#pragma once

struct Vec2
{
	float X, Y;

	constexpr Vec2() : X(0.0f), Y(0.0f) { }
	constexpr Vec2(float _X, float _Y) : X(_X), Y(_Y) { }
};

struct Vec3
{
	float X, Y, Z;
};

struct Matrix
{
	float matrix[16];
};