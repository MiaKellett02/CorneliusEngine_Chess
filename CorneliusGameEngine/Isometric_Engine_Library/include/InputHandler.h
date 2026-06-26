#pragma once
//Project includes.
#include <Vector2.h>

//Class Declaration.
class InputHandler {
public:
	

	static Vector2Int GetMousePosition();
	static bool LeftMouseButtonPressed();
	static bool RightMouseButtonPressed();
};