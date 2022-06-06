#pragma once
#include <GLFW/glfw3.h>
#include "MemMan.h"
#include "DataTypes.h"
#include "Options.h"
#include "Offsets.h"
#include "Values.h"
#include "Addr.h"

void updateMenu(GLFWwindow* window);

void DrawLine(Vec2& start, Vec2& end);

void ConvertToRange(Vec2& Point);

bool WorldToScreen(const Vec3& VecOrigin, Vec2& VecScreen, float* Matrix);
