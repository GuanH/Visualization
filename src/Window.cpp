#include "Window.h"
#include "GLFW/glfw3.h"
#include "Utility.h"
void KeyboardCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
void WindowSizeCallBack(GLFWwindow* window, int width, int height);
void ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset);
Window::Window(int width, int height, const char* name)
	:m_width(width), m_height(height),m_scrollx(0),m_scrolly(0)
{
	m_keyPressed.fill(false);
	ERR_CHECK_BOOL(glfwInit());
	ERR_CHECK_BOOL(m_window = glfwCreateWindow(width, height, name, NULL, NULL));
	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, this);
	glfwSetKeyCallback(m_window, KeyboardCallBack);
	glfwSetWindowSizeCallback(m_window, WindowSizeCallBack);
	glfwSetScrollCallback(m_window, ScrollCallBack);
#ifdef PROFILE_FPS
	glfwSwapInterval(0);
#endif
	ERR_CHECK_BOOL(gladLoadGL());

}

bool Window::ProcessEvent()
{
	m_scrollx = m_scrolly = 0;
	glfwPollEvents();
	return glfwWindowShouldClose(m_window) == 0;
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(m_window);
}

int Window::GetWndWidth()
{
	return m_width;
}

int Window::GetWndHeight()
{
	return m_height;
}

bool Window::KeyDown(int key)
{
	return m_keyPressed[key];
}

void KeyboardCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Window* windowClass = (Window*)glfwGetWindowUserPointer(window);
	if (action == GLFW_PRESS) {
		windowClass->m_keyPressed[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		windowClass->m_keyPressed[key] = false;
	}
}

void WindowSizeCallBack(GLFWwindow* window, int width, int height) {
	Window* windowClass = (Window*)glfwGetWindowUserPointer(window);
	windowClass->m_width = width;
	windowClass->m_height = height;
    glViewport(0,0,width,height);
}

void ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset){
	Window* windowClass = (Window*)glfwGetWindowUserPointer(window);
	windowClass->m_scrollx+=xoffset;
	windowClass->m_scrolly+=yoffset;
}
