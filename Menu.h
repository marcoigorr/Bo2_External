#pragma once
#include <iostream>
#include <GLFW/glfw3.h>

void updateMenu(GLFWwindow* window)
{
	if (Options::bMenu)
	{
		glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, false);
	}
	else
	{
		glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, true);
	}
	
}