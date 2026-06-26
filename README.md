# Cornelius Game Engine

## This is a basic 2D/Isometric Game engine developed to help improve my C++ Technical Skills and lower level programming knowledge.

### Description
A basic 2D/Isometric Game engine  that will focus on creating a framework to make it as easy as possible to create 2D/Isometric games. This will be used as research for me to learn how to make a good game engine framework before moving onto 3D.

### Technologies Used (List may expand as project is developed.):
All external libraries used are included in a dedicate "ExternalDependencies" folder.
 - Visual Studio 2022 has been used for development and compilation up to C++20 and C standard 17.
 - SDL2 is going to be used to load textures for it's wide variety of cross platform support and it's native compatibility to work within C++.

### Design Documentation
https://docs.google.com/document/d/1ms8ovwWcb1dTRgAjilfISpnSTy8yPO_m2SWJwLhE6II/edit?usp=sharing

## Visual Studio Projects contained within this repository.
### Isometric_Engine_Library
 - The engine framework project contains all the generic functionality that any game or simulation might require to be made.
 - This project builds into a static library that allows any projects using the engine to access the function definitions of all project header files.

### IsometricGameProject
 - This is the example project that I will use to develop an Isometric game in.
 - If creating your own simulation/game using the engine, look at this project to see how you can use the engine.

## How to install/run/use this project.
### Installation and setup instructions.
 1. Clone the repository to your machine.
 2. Open the Visual Studio 2022 Solution Named "CorneliusGameEngine.sln".
 3. Set the "IsometricGameProject" project as the startup project.
 4. Ensure the "Isometric_Engine_Library" project is set as a dependency of the sandbox project. (This project builds the engine library that the sandbox relies on.)

### How to run the project.
 1. Build the "CorneliusGameEngine" Solution. The engine will compile into a static library (.lib) file you can find in the solution directory's 'lib' followed by configuration and platform subdirectory. The example executable project will compile into the solution' directory of 'bin' followed by the configuration and platform once again.
 3. Go to the Solution directory, open the binary folder and locate the generated executable.
 4. Run the generated Executable (You should at minimum be greeted by a console window with any messages that the engine has for you).

### How to use this project.


## License and Credits.
### License
 - This project is under the MIT License.

### Credits
 - 
