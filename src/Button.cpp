#include "Button.h"

// Rory Clark
// https://rorywebdev.com
// 2016

// Button Class used for UI elements
// Allows custom colouring, text and has built in transitions
// Validation for actual clicks can differ from holding down to a simple return when actually activated

Button::Button(SDL_Renderer* _render, std::string _text, TTF_Font* _font, SDL_Color _color, int _topX, int _topY, int _width, int _height, float _returnCode, bool _hold)
{
	// Position the text and assign the variables
	m_state = 0;
	m_topX = _topX;
	m_topY = _topY;
	m_width = _width;
	m_height = _height;
	m_returnCode = _returnCode;

	// Create the text
	m_text = new Text(_render);
	m_text->LoadText(_text, _color, _font);
	m_text->ChangePos(m_topX + ((m_width / 2) - (m_text->GetWidth() / 2)), m_topY + ((m_height / 2) - (m_text->GetHeight() / 2)));

	m_clickState = 0.0f;
	m_hoverState = 0.0f;
	m_buttonRed = 78;
	m_buttonGreen = 153;
	m_buttonBlue = 227;
	SDL_Rect button = { _topX, _topY, _width, _height };
	m_buttonRect = button;
	m_render = _render;
	m_hold = _hold;
}

Button::~Button()
{
	// Destroy all values
	m_state = NULL;
	m_topX = NULL;
	m_topY = NULL;
	m_width = NULL;
	m_height = NULL;
	m_returnCode = NULL;
	delete m_text;
	m_render = NULL;
}

void Button::SetColor(int _r, int _g, int _b)
{
	// Allow custom colours for buttons
	m_buttonRed = _r;
	m_buttonGreen = _g;
	m_buttonBlue = _b;
}

void Button::ChangeText(std::string _text)
{
	// Change the text being used
	m_text->ChangeText(_text);
	m_text->ChangePos(m_topX + ((m_width / 2) - (m_text->GetWidth() / 2)), m_topY + ((m_height / 2) - (m_text->GetHeight() / 2)));
}

void Button::UpdateTextSizes()
{
	// Reset the text held inside the button (for if we wanted separate text pointers)
	m_text->ChangePos(m_topX + ((m_width / 2) - (m_text->GetWidth() / 2)), m_topY + ((m_height / 2) - (m_text->GetHeight() / 2)));
}

float Button::Update(float _delta, int _mouseX, int _mouseY, int _mouseState)
{
	float returnCode = 0.0f;
	if ((_mouseX >= m_topX && _mouseX < (m_topX + m_width)) && (_mouseY >= m_topY && _mouseY < (m_topY + m_height)))
	{
		switch (_mouseState)
		{
		case 0:
			// If the user moves the mouse off of the X & Y then we don't want to be sending the return value when they let go of the mouse
			if (m_state == 2)
			{
				returnCode = m_returnCode;
				m_state = 0;
			}
			else
			{
				m_state = 1;
			}
			break;
		case 1:
			if (m_state != -1)
			{
				m_state = 2;
				if (m_hold)
				{
					// If the user is holding the mouse down and we've enabled it for this button then constantly return the value
					returnCode = m_returnCode;
				}
			}
			break;
		}
	}
	else
	{
		// Prevent the mouse from drifting between buttons when held
		if (_mouseState > 0)
		{
			m_state = -1;
		}
		else
		{
			m_state = 0;
		}
	}

	// Transition the mouse states so we can fade colours in and out
	switch (m_state)
	{
	case -1:
	case 0:
		m_hoverState -= _delta;
		m_clickState -= _delta;
		m_hoverState = (m_hoverState < 0) ? 0 : m_hoverState;
		m_clickState = (m_clickState < 0) ? 0 : m_clickState;
		break;
	case 1:
		m_hoverState += _delta;
		m_clickState -= _delta;
		m_hoverState = (m_hoverState > 0.1f) ? 0.1f : m_hoverState;
		m_clickState = (m_clickState < 0) ? 0 : m_clickState;
		break;
	case 2:
		m_hoverState -= _delta;
		m_clickState += _delta;
		m_hoverState = (m_hoverState < 0) ? 0 : m_hoverState;
		m_clickState = (m_clickState > 0.1f) ? 0.1f : m_clickState;
		break;
	}
	return returnCode;
}

void Button::Draw()
{
	// Draw the buttons (SDL filled rectangles that change the current SDL render color)
	SDL_SetRenderDrawColor(m_render, m_buttonRed, m_buttonGreen, m_buttonBlue, 255);
	SDL_RenderFillRect(m_render, &m_buttonRect);
	if (m_hoverState > 0.0f)
	{
		SDL_SetRenderDrawColor(m_render, 255, 255, 255, (int)(m_hoverState * 510));
		SDL_RenderFillRect(m_render, &m_buttonRect);
	}
	if (m_clickState > 0.0f)
	{
		SDL_SetRenderDrawColor(m_render, 0, 0, 0, (int)(m_clickState * 510));
		SDL_RenderFillRect(m_render, &m_buttonRect);
	}
	m_text->Draw();
}