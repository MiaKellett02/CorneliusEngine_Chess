//Library includes.
#include <iostream>

//Project includes.
#include "Application.h"
#include "IsometricUnitMovementTestScene.h"

//Functions.
int main(int argc, char* argv)
{
	//Startup the application.
	Application app("Isometric Game Test", 1280, 720, false , true);

	//Move the camera by half the screen width.
	//Vector2Int cameraPos = Vector2Int(app.SCREEN_WIDTH / 4, app.SCREEN_HEIGHT / 16);
	//app.GetRenderer().SetCameraOffset(cameraPos);

	//Create the desired scenes and run the program..
	//Scene testScene("ArmyScene");
	IsometricUnitMovementTestScene testScene("IsoTestScene", 100, 100, "resources/IsoFlatTile.png");
	app.AddScene(&testScene);

	app.RunApplication();

	//Application end.
	return 0;
}