#ifndef _BUTTON_H
#define _BUTTON_H

// Rory Clark
// https://rorywebdev.com
// 2016

#include "Text.h"

class Button
{
public:
	Button(SDL_Renderer* _render, std::string _text, TTF_Font* _font, SDL_Color _color, int _topX, int _topY, int _width, int _height, float _returnCode, bool _hold);
	~Button();

	void SetColor(int _r, int _g, int _b);
	void ChangeText(std::string _text);
	void UpdateTextSizes();

	float Update(float _delta, int _mouseX, int _mouseY, int _mouseState);
	void Draw();


private:
	int m_topX, m_topY, m_width, m_height;
	int m_textX, m_textY;
	int m_state;
	int m_oldMouseState;
	int m_mouseCode;
	float m_returnCode;

	int m_buttonRed, m_buttonGreen, m_buttonBlue;

	bool m_hold;

	float m_hoverState, m_clickState;

	SDL_Rect m_buttonRect;
	SDL_Renderer* m_render;

	Text* m_text;
};

#endif