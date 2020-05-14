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

	enum class Scroll {
		HORIZONTAL = 0,
		VERTICAL = 1
	};

	static glm::vec2 MousePosition();
	static glm::vec2 MouseDeltaPosition();

	static bool IsMouseKeyDown(const int& button);
	static bool IsMouseKeyPressed(const int& button);
	static bool IsKeyDown(const int& key);			// Key held down
	static bool IsKeyPressed(const int& key);		// Key pressed
	
	static double GetMouseScroll(Scroll direction = Scroll::VERTICAL);

	static void DisableCursor(const bool& disable);

protected:
	static void SetCallbacks(GLFWwindow* window);

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void MousePositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void Update();

private:

	static unsigned int keyStates[GLFW_KEY_LAST];
	static unsigned int keyPrevStates[GLFW_KEY_LAST];
	static unsigned int mouseButtonStates[GLFW_MOUSE_BUTTON_LAST];
	static unsigned int mouseButtonPrevStates[GLFW_MOUSE_BUTTON_LAST];
	static int mouseScrollState[2];

	static glm::vec2 mousePosition;
	static glm::vec2 mouseLastPosition;
	static glm::vec2 mouseDelta;
};

