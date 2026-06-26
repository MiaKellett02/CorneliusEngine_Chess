//Project includes.
#include "Application.h"
#include "MainChessScene.h"

//Functions.
int main(int argc, char* argv)
{

	//Startup the application.
	Application app("Isometric Chess Game");

	//Move the camera by half the screen width.
	Vector2Int cameraPos = Vector2Int(app.SCREEN_WIDTH / 4, app.SCREEN_HEIGHT / 16);
	app.GetRenderer().SetCameraOffset(cameraPos);

	//Create the desired scenes.
	MainChessScene testScene("ChessScene", "resources/IsoFlatTile.png");
	//MainChessScene testScene("ChessScene", "resources/IsoShadedCube.png");

	//Run the program..
	app.AddScene(&testScene);

	app.RunApplication();

	//Application end.
	return 0;
}