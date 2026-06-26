//Library includes.
#include <SDL.h>

//Project includes.
#include "InputHandler.h"

Vector2Int InputHandler::GetMousePosition()
{
	int x, y;
	Uint32 buttons = SDL_GetMouseState(&x, &y);
	return Vector2Int(x, y);
}

bool InputHandler::LeftMouseButtonPressed()
{
	int x, y;
	Uint32 buttons = SDL_GetMouseState(&x, &y);

	return buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
}

bool InputHandler::RightMouseButtonPressed()
{
	int x, y;
	Uint32 buttons = SDL_GetMouseState(&x, &y);

	return buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
}
