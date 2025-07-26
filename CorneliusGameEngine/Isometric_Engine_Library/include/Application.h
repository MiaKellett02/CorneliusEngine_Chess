#pragma once

//Forward Declarations.
class Entity;

//Project includes.
#include "Renderer.h"
#include "Vector2.h"
#include "Scene.h"

//Library includes.
#include <iostream>
#include <vector>
#include <string>

//Class definition.
class Application {
public:
	//Singleton class as Application needs to be globally accessible
	//	so that entities can interact with other entities.
	static Application* Instance() {
		return instance;
	}

	//Constructor and deconstructor.
	Application(const std::string& a_appName)
	{
		//Application setup variables.
		APP_NAME = a_appName;
		bool isFullScreen = false;
		bool runAtMonitorResolution = false;
		int scrWidth = -1;
		int scrHeight = -1;

		//If no configuration file exists, we will ask the user for the configuration.
		//Otherwise, we will load the configuration from the file.
		bool configExists = false; // Placeholder for configuration file check.
		if (configExists) {

		}
		else {
			std::cout << "No configuration file found. Please enter the following settings:" << std::endl;

			//Should we run in fullscreen mode?
			char fullScreenInput;
			std::cout << "Run in fullscreen mode? (y for yes, n for no): ";
			while (true) {
				std::cin >> fullScreenInput;
				if (fullScreenInput == 'y' || fullScreenInput == 'Y') {
					isFullScreen = true;
					break;
				}
				else if (fullScreenInput == 'n' || fullScreenInput == 'N') {
					isFullScreen = false;
					break;
				}
				else {
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cout << "Invalid input. Please enter 'y' for yes or 'n' for no: ";
				}
			}

			//Should we run at the monitor resolution?
			char monitorResInput;
			std::cout << "Run at monitor resolution? \n - if yes and not in fullscreen mode, the screen will be borderless - \n(y for yes, n for no): ";
			while (true) {
				std::cin >> monitorResInput;
				if (monitorResInput == 'y' || monitorResInput == 'Y') {
					runAtMonitorResolution = true;
					break;
				}
				else if (monitorResInput == 'n' || monitorResInput == 'N') {
					runAtMonitorResolution = false;
					break;
				}
				else {
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cout << "Invalid input. Please enter 'y' for yes or 'n' for no: ";
				}
			}

			//Get the screen width and height if not running at the monitor resolution..
			if (!runAtMonitorResolution) {
				std::cout << "Enter screen width (in pixels): ";
				while (!(std::cin >> scrWidth) || scrWidth < 640) {
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cout << "Invalid input for width. Please enter an integer and make sure the width is atleast 640px: ";
				}
				std::cout << "Enter screen height (in pixels): ";
				while (!(std::cin >> scrHeight) || scrHeight < 480) {
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cout << "Invalid input for height. Please enter an integer and make sure the height is atleast 480px: ";
				}
			}

			//Ensure there is an instance set for the class.
			//We assign the instance in the constructor so initialisation is kept explicit and not lazy.
			instance = this;

			RUN_AT_MONITOR_RESOLUTION = runAtMonitorResolution;

			// Will be overwritten by monitor width&height, 
			// inside the renderer initialisation if "a_runAtMonitorResolution" is true.
			SCREEN_WIDTH = scrWidth;
			SCREEN_HEIGHT = scrHeight;

			IS_FULLSCREEN = isFullScreen;
		}

		//Ensure the application windows has been created.
		StartApplicationWindow(APP_NAME, RUN_AT_MONITOR_RESOLUTION);
	};
	~Application() { Shutdown(); };


	//Application handling functions.
	void RunApplication();
	void QuitApplication() { m_hasQuit = true; }

	//Scene Handling Functions.
	void AddScene(Scene* a_newScene) { m_scenes.push_back(a_newScene); }
	void LoadSceneByName(const std::string& a_sceneName);
	void LoadSceneByIndex(const int& a_index);
	Scene* GetActiveScene() { return m_activeScene; }

	//Utility.
	bool IsPositionOutOfBound(Vector2& a_pos);
	Renderer& GetRenderer() { return m_renderer; }

	//Public variables.
	std::string APP_NAME;
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	bool IS_FULLSCREEN;
	bool RUN_AT_MONITOR_RESOLUTION;

private:
	//singleton instance.
	static Application* instance;

	//Backend Engine systems.
	Renderer m_renderer;

	//Scene handling variables.
	std::vector<Scene*> m_scenes;
	Scene* m_activeScene = nullptr;

	//Functions.
	void StartApplicationWindow(const std::string& a_appName, bool a_runAtMonitorResolution);
	void Shutdown();

	//Consts.
	int m_targetFPS = 100000000;
	bool m_hasQuit = false;
};