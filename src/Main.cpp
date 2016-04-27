#include "System.h"

int main(int argc, char *argv[])
{
	System system;

	// Run throught each start function
	// Splitting them down allows for more information to be displayed to the user in case of error
	try
	{
		// We start SDL first so that we can use the message boxes that SDL provides
		system.StartSDL();
	}
	catch (char* error)
	{
		printf(error);
		return -1;
	}

	try
	{
		// Create the SDL windows and bind the OpenGL contexts
		system.StartWindow();
	}
	catch (char* error)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Window Error", error, NULL);
		return -1;
	}

	// Start the renders for SDL, both for the main OpenGL window and the SDL settings window
	try
	{
		system.StartRenderer();
	}
	catch (char* error)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Renderer Error", error, NULL);
		return -1;
	}

	// Start SDL_TTF plugin
	try
	{
		system.StartTTF();
	}
	catch (char* error)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL TTF Error", error, NULL);
		return -1;
	}

	// Finally load the font
	try
	{
		system.LoadFonts();
	}
	catch (char* error)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Font Error", error, NULL);
		return -1;
	}

	// Create the map and initialise all of the player (entity values)
	system.StartSubsystems();
	// Reset our time to ignore the start
	system.StartTime();
	
	// General system update and draw
	// System holds ALL sub-systems such as scene, player, map and algorithm
	// Every update required for these happens within the system or below
	while (system.GetRunning())
	{
		system.Update();
		system.Draw();
	}

	return 0;
}