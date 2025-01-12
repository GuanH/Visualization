#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
class Window {
public:
	Window(int width, int height, const char* name);
	bool ProcessEvent();
	void SwapBuffers();
	int GetWndWidth();
	int GetWndHeight();
	bool KeyDown(int key);
	int GetScrollX()const{return m_scrollx;}
	int GetScrollY()const{return m_scrolly;}
    GLFWwindow* GetWnd() const {return m_window;}
private:
	
	GLFWwindow* m_window;
	int m_width;
	int m_height;
	std::array<bool, GLFW_KEY_LAST + 1> m_keyPressed;
	int m_scrollx;
	int m_scrolly;
	friend void KeyboardCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
	friend void WindowSizeCallBack(GLFWwindow* window, int width, int height);
	friend void ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset);

};
