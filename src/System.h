#ifndef _SYSTEM_H
#define _SYSTEM_H

// Rory Clark
// http://rorywebdev.com
// 2016

#include "Scene.h"

class System
{
public:
	System();
	~System();

	void Update();
	void Draw();

	void CloseProgram();

	void StartSDL();

	void StartWindow();
	void StartRenderer();

	void StartTTF();

	void StartSubsystems();

	void StartTime();

	void LoadFonts();

	bool GetRunning() { return m_running; };

	SDL_Window *GetWindow() { return m_window; };
	SDL_Renderer *GetRenderer() { return m_render; };

	float GetDelta() { return m_deltaTime; };

private:

	bool m_running;

	SDL_Window *m_window;
	int m_windowH, m_windowW;
	SDL_Renderer *m_render;

	Uint32 m_newTick, m_oldTick;
	double m_deltaTime;
	int m_frames;
	float m_frameTime;

	std::string m_fontLoc;
	TTF_Font* m_smallFont;
	SDL_Color m_textColor;
	Text* m_testText;

	Scene* m_scene;

	int m_mouseX, m_mouseY, m_mouseState;

	float m_cfgUpdate, m_reFocusTimer;
	bool m_reFocus, m_heldTexture, m_usingKeyboard;

	template<typename T> std::string StringAnyInput(T _inp);

	void UpdateInput();

};


#endif