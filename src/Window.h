#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <array>
#include "Utility.h"
class Window {
public:
	Window(int width, int height, const char* name);
	bool ProcessEvent();
	void SwapBuffers();
	int GetWndWidth();
	int GetWndHeight();
	bool KeyDown(int key);
    GLFWwindow* GetWnd() const {return m_window;}
private:
	
	GLFWwindow* m_window;
	int m_width;
	int m_height;
	std::array<bool, GLFW_KEY_LAST + 1> m_keyPressed;
	friend void KeyboardCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
	friend void WindowSizeCallBack(GLFWwindow* window, int width, int height);
};
