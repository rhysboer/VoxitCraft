#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Application.h"

int main() {

	{
		Application* app = new Application();
		app->CreateEngine("Dick", 1280, 720);
		app->Run();

		delete app;
	}

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	return 0;
}