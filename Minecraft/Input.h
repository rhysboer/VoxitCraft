#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

class Engine;

class Input {
	friend Engine;
public:
	Input();
	~Input();

	static glm::vec2 MousePosition();
	static glm::vec2 MouseDeltaPosition();

	static bool IsMouseKeyDown(const int button);
	static bool IsKeyDown(const int key);			// Key held down
	static bool IsKeyPressed(const int key);		// Key pressed 

protected:
	static void SetCallbacks(GLFWwindow* window);

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void MousePositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void Update();

private:

	static unsigned int keyStates[GLFW_KEY_LAST];
	static unsigned int keyPrevStates[GLFW_KEY_LAST];
	static unsigned int mouseButtonStates[GLFW_MOUSE_BUTTON_LAST];
	static glm::vec2 mousePosition;
	static glm::vec2 mouseLastPosition;
	static glm::vec2 mouseDelta;
};

