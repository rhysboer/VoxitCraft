#include "Application.h"

int main() {

	Application* app = new Application();
	app->CreateEngine("Dick", 1280, 720);
	app->Run();

	delete app;
	return 0;
}