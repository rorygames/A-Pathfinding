#include "System.h"

// Rory Clark
// http://rorywebdev.com
// 2016

System::System()
{
	// Set our default values
	m_running = false;
	m_cfgUpdate = 0.0f;
	m_reFocus = false;
	m_reFocusTimer = 0.0f;
	m_heldTexture = false;
	m_usingKeyboard = false;
	m_fontLoc = "Roboto.ttf";
	m_windowW = 1280;
	m_windowH = 720;
	m_frames = 0;
	m_frameTime = 0;
}

System::~System()
{
	this->CloseProgram();
}

void System::CloseProgram()
{
	// Delete the contexts, renders and windows created by SDL
	SDL_DestroyRenderer(m_render);
	SDL_DestroyWindow(m_window);
	// Quit our plugins and SDL itself
	TTF_Quit();
	SDL_Quit();
}

void System::StartSDL()
{
	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw("ERROR: SDL could not be initialised.");
	}
}

void System::StartWindow()
{
	// Create our normal window
	m_window = SDL_CreateWindow("Pathfinder", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_windowW, m_windowH, SDL_WINDOW_SHOWN);
	int x, y;
	SDL_GetWindowPosition(m_window, &x, &y);
	// Create our config window below our main
	if (m_window == NULL)
	{
		throw("Unable to create window.");
	}
}

void System::StartRenderer()
{
	// Start our two renderers for the main and config window
	m_render = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	if (m_render == NULL)
	{
		throw("Unable to create renderer.");
	}
	// Allow SDL to do alpha blending on our config window
	SDL_SetRenderDrawBlendMode(m_render, SDL_BLENDMODE_BLEND);
}

void System::StartTTF()
{
	// Start the TTF plugin
	if (TTF_Init() == -1)
	{
		throw("Unable to start SDL TTF.\nIs sdl2_ttf.dll & zlib1.dll in the folder?\nError: %s", TTF_GetError());
	}
}

void System::StartTime()
{
	// Set all our delta times to 0 and start the ticks
	m_newTick = SDL_GetTicks();
	m_oldTick = m_newTick;
	m_deltaTime = 0.0f;
	m_running = true;
	SDL_Color white = { 255, 255, 255 };
	m_textColor = white;
	m_testText = new Text(m_render);
	m_testText->LoadText("FPS: ", m_textColor, m_smallFont);
}

void System::StartSubsystems()
{
	srand(time(NULL));
	m_scene = new Scene(m_render, m_windowW, m_windowH, m_smallFont);
}

void System::Update()
{
	// Clear the screen
	SDL_SetRenderDrawColor(m_render, 0, 0, 0, 255);
	SDL_RenderClear(m_render);

	// Update the delta time
	m_oldTick = m_newTick;
	m_newTick = SDL_GetTicks();
	m_deltaTime = (m_newTick - m_oldTick) / 1000.0f;
	// Update the events/input
	this->UpdateInput();

	m_frameTime += m_deltaTime;
	m_frames++;
	if (m_frameTime >= 1)
	{
		m_testText->ChangeText("FPS: " + std::to_string(m_frames));
		m_frames = 0;
		m_frameTime -= 1;
	}

	m_scene->Update(m_deltaTime, m_mouseX, m_mouseY, m_mouseState);

}

void System::UpdateInput()
{
	SDL_Event ev;
	while (SDL_PollEvent(&ev) != 0)
	{
		switch (ev.type)
		{
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&m_mouseX, &m_mouseY);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_mouseState = 1;
			break;
		case SDL_MOUSEBUTTONUP:
			m_mouseState = 0;
			break;
		case SDL_QUIT:
			m_running = false;
			break;
		case SDL_KEYDOWN:
			break;
		case SDL_KEYUP:
			switch (ev.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				// Quit the program!
				m_running = false;
				break;
			}
			break;
		}
	}
}

void System::Draw()
{
	m_testText->Draw();

	m_scene->Draw();

	SDL_RenderPresent(m_render);
}

void System::LoadFonts()
{
	// Load our fonts, although we only have one it's not difficult to have more
	m_smallFont = TTF_OpenFont(m_fontLoc.c_str(), 13);
	if (m_smallFont == NULL)
	{
		throw("Unable to load font file.\nAre you sure there is a font file in the folder?");
	}
}

// Read in any input from anything and return a string that we can actually work with
template<typename T> std::string System::StringAnyInput(T _inp)
{
	std::stringstream input;
	input << _inp;
	return input.str();
}