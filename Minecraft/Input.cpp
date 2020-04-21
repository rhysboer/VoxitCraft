#include "Input.h"
#include "Engine.h"

unsigned int Input::keyStates[GLFW_KEY_LAST] = { 0 };
unsigned int Input::keyPrevStates[GLFW_KEY_LAST] = { 0 };
unsigned int Input::mouseButtonStates[GLFW_MOUSE_BUTTON_LAST] = { 0 };
glm::vec2 Input::mousePosition = glm::vec2(0.0f);
glm::vec2 Input::mouseLastPosition = glm::vec2(0.0f);
glm::vec2 Input::mouseDelta = glm::vec2(0.0f);

Input::Input() { }

Input::~Input() { }

glm::vec2 Input::MousePosition() {
	return mousePosition;
}

glm::vec2 Input::MouseDeltaPosition() {
	return mouseDelta;
}

bool Input::IsMouseKeyDown(const int button) {
	if(button < 0 || button > GLFW_MOUSE_BUTTON_LAST) return false;
	if(mouseButtonStates[button] == GLFW_PRESS || mouseButtonStates[button] == GLFW_REPEAT)
		return true;
	return false;
}

bool Input::IsKeyDown(const int key) {
	if(key < 0 || key > GLFW_KEY_LAST) return false;
	if(keyStates[key] == GLFW_PRESS || keyStates[key] == GLFW_REPEAT)
		return true;
	return false;
}

bool Input::IsKeyPressed(const int key) {
	if(key < 0 || key > GLFW_KEY_LAST) return false;
	if(keyStates[key] == GLFW_PRESS && keyPrevStates[key] == GLFW_RELEASE)
		return true;
	return false;
}

void Input::SetCallbacks(GLFWwindow* window) {
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetCursorPosCallback(window, MousePositionCallback);
}

void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(key < 0 || key > GLFW_KEY_LAST) return;
	keyStates[key] = action;
}

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if((button < 0 || button > GLFW_MOUSE_BUTTON_LAST) /*|| ImGui::IsAnyWindowHovered() || ImGui::IsAnyItemHovered()*/) return;
	mouseButtonStates[button] = action;
}

void Input::MousePositionCallback(GLFWwindow* window, double xpos, double ypos) {
	mouseLastPosition = mousePosition;
	mousePosition = glm::vec2(xpos, ypos);

	mouseDelta = mousePosition - mouseLastPosition;
}

void Input::Update() {
	for(int i = 0; i < GLFW_KEY_LAST; i++) 
		keyPrevStates[i] = keyStates[i];

	mouseDelta = glm::vec2(0.0);
}
