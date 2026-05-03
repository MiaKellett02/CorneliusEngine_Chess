//Project includes.
#include "Application.h"
#include "MainMenuScene.h"
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
	MainMenuScene mainMenu("MainMenu", "resources/IsoFlatTile.png");
	MainChessScene chessScene("ChessScene", "resources/IsoFlatTile.png");
	//MainChessScene chessScene("ChessScene", "resources/IsoShadedCube.png");

	//Register scenes - main menu loads first, chess scene is loaded on player input.
	app.AddScene(&mainMenu);
	app.AddScene(&chessScene);

	app.RunApplication();

	//Application end.
	return 0;
}