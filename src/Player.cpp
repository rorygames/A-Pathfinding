#include "Player.h"

Player::Player(SDL_Renderer* _render, Map* _map, PathAlgorithm* _algo)
{
	m_render = _render;
	m_map = _map;
	m_algorithm = _algo;
	m_path = new Path;
	SDL_Color green = { 50, 50, 200 };
	m_color = green;
	SDL_Color red = { 200, 50, 50 };
	m_endColor = red;
	m_setStart = false;
	m_setEnd = false;
	m_startMoving = false;
	m_finishedMoving = false;
	m_showPath = true;
	m_resetAllow = false;
	m_resetStarted = false;
	m_diagonal = true;
}

Player::~Player()
{
	m_algorithm->ResetPath(m_path);
}

void Player::Update(float _delta)
{
	// If we've set the start and end tiles
	if (m_setStart && m_setEnd)
	{
		// Make sure we've calculated a path, if not then calculate one!
		if (!m_path->calculated)
		{
			m_algorithm->CalculatePath(m_path);
			if (m_path->calculated)
			{
				if (m_resetStarted)
				{
					// If we started a mid-update reset
					m_resetStarted = false;
				}
				else
				{
					// If not then set the movement values to 0
					m_moveToX = 0;
					m_moveToY = 0;
					if (m_path->positions.size() > 0)
					{
						m_newMoveTile = m_path->positions.at(m_path->index - 1);
					}
				}
				m_finishedMoving = false;
			}
		}
		// Move our rectangle if the user has enabled movement
		UpdateMove(_delta);
	}
}

void Player::Draw()
{
	// Draw our path if calculated
	if (m_path->reached && m_path->calculated && m_showPath)
	{
		SDL_SetRenderDrawColor(m_render, 255, 0, 0, 255);
		SDL_RenderDrawLines(m_render, &m_path->points[0], m_path->points.size());
	}
	// Draw the start and end points if set
	if (m_setEnd)
	{
		SDL_SetRenderDrawColor(m_render, m_endColor.r, m_endColor.g, m_endColor.b, 255);
		SDL_RenderFillRect(m_render, &m_endRect);
		m_endText->Draw();
	}
	if (m_setStart)
	{
		SDL_SetRenderDrawColor(m_render, m_color.r, m_color.g, m_color.b,255);
		SDL_RenderFillRect(m_render, &m_rect);
		m_text->Draw();
	}
}

void Player::UpdateMove(float _delta)
{
	if (m_resetAllow)
	{
		// Mid-update reset call
		// Resets our path but keeps our initial values of start and finish (used for mouse movement primarily)
		m_resetAllow = false;
		m_resetStarted = true;
		Tile* end = m_path->end;
		bool reached = m_path->reached;
		ResetPath();
		m_path->start = m_newMoveTile;
		m_path->open.push_back(m_path->start);
		m_path->reached = reached;
		m_path->end = end;
		m_path->diagonal = m_diagonal;
	}
	// If we have a path, we've allowed movement to begin and the player rectangle is not at the end of the path
	if (m_path->reached && m_startMoving && !m_finishedMoving)
	{
		if (m_moveToX == 0 && m_moveToY == 0)
		{
			// Move to our next tile and make sure to "reset" our starting positions so we don't "overflow" to the next tile if frame time is huge
			m_path->index--;
			if (m_path->index < 0)
			{
				m_finishedMoving = true;
				m_rect.x = m_newMoveTile->x;
				m_rect.y = m_newMoveTile->y;
			}
			else
			{
				m_newMoveTile = m_path->positions.at(m_path->index);
				m_oldMoveTile = m_path->positions.at(m_path->index + 1);
				m_rect.x = m_oldMoveTile->x;
				m_rect.y = m_oldMoveTile->y;
				m_moveToX = (m_oldMoveTile->x - m_newMoveTile->x) / m_newMoveTile->w;
				m_moveToY = (m_oldMoveTile->y - m_newMoveTile->y) / m_newMoveTile->h;
				m_moveToCost = m_newMoveTile->parameter;
				if (m_moveToX > 0)
				{
					m_moveXPos = false;
				}
				else
				{
					m_moveXPos = true;
				}
				if (m_moveToY > 0)
				{
					m_moveYPos = false;
				}
				else
				{
					m_moveYPos = true;
				}
				m_moveToX = abs(m_moveToX);
				m_moveToY = abs(m_moveToY);
				m_moveDiffX = 0;
				m_moveDiffY = 0;
			}
		}
		// Based on the cost of the tile's parameter value we move at either a faster or slower rate
		if (!m_finishedMoving)
		{
			if (m_moveToX > 0)
			{
				float moveVal = (_delta * 2) / m_moveToCost;
				m_moveToX -= moveVal;
				if (m_moveToX < 0)
				{
					moveVal += (m_moveToX * -1);
					m_moveToX = 0;
				}
				m_moveDiffX += moveVal * m_oldMoveTile->w;
				if (m_moveXPos)
				{
					m_rect.x = m_oldMoveTile->x + m_moveDiffX;
				}
				else
				{
					m_rect.x = m_oldMoveTile->x - m_moveDiffX;
				}
			}
			if (m_moveToY > 0)
			{
				float moveVal = (_delta * 2) / m_moveToCost;
				m_moveToY -= moveVal;
				if (m_moveToY < 0)
				{
					moveVal += (m_moveToY * -1);
					m_moveToY = 0;
				}
				m_moveDiffY += moveVal * m_oldMoveTile->h;
				if (m_moveYPos)
				{
					m_rect.y = m_oldMoveTile->y + m_moveDiffY;
				}
				else
				{
					m_rect.y = m_oldMoveTile->y - m_moveDiffY;
				}
			}
			// Update the "P" text on our rectangle
			m_text->ChangePos(m_rect.x + ((m_rect.w / 2) - (m_text->GetWidth() / 2)), m_rect.y + ((m_rect.h / 2) - (m_text->GetHeight() / 2)));
		}
	}
}

void Player::SetStartPosition(Tile* _startTile)
{
	// Set our path values and the positions for our player rectangle
	m_path->positions.clear();
	m_path->open.clear();
	m_path->open.push_back(_startTile);
	m_path->start = _startTile;
	m_path->start->open = true;
	m_rect.x = m_path->start->x;
	m_rect.y = m_path->start->y;
	m_rect.w = m_path->start->w;
	m_rect.h = m_path->start->h;
	m_text->ChangePos(m_rect.x + ((m_rect.w / 2) - (m_text->GetWidth() / 2)), m_rect.y + ((m_rect.h / 2) - (m_text->GetHeight() / 2)));
	m_setStart = true;
	m_resetAllow = false;
}

void Player::RemoveStartPosition()
{
	// Reset the start
	m_path->start = NULL;
	m_setStart = false;
}

void Player::SetDestination(Tile* _destTile)
{
	// Set our end tile and the text for it
	m_path->end = _destTile;
	m_endRect.x = m_path->end->x;
	m_endRect.y = m_path->end->y;
	m_endRect.w = m_path->end->w;
	m_endRect.h = m_path->end->h;
	m_endText->ChangePos(m_endRect.x + ((m_endRect.w / 2) - (m_text->GetWidth() / 2)), m_endRect.y + ((m_endRect.h / 2) - (m_text->GetHeight() / 2)));
	m_setEnd = true;
	m_resetAllow = false;
}

void Player::RemoveDestination()
{
	// Reset the end
	m_path->end = NULL;
	m_setEnd = false;
}

void Player::ResetPath()
{
	// Reset our path object
	m_algorithm->ResetPath(m_path);
}

bool Player::CheckFailurePath()
{
	// Check to make sure that the path hasn't failed in reaching it's target
	if (m_path->calculated && m_path->routed && !m_path->reached)
	{
		return true;
	}
	return false;
}

bool Player::CheckCompletePath()
{
	// Check to make sure we've reached the target after being calculated
	if (m_path->calculated && m_path->routed && m_path->reached)
	{
		return true;
	}
	return false;
}

float Player::GetCalculateTime()
{
	// Return how long it took to calculate the path
	return m_path->time;
}

void Player::QueueMidReset()
{
	// Set it so that the path will be recalculated on the next update cycle
	m_resetAllow = true;
}

void Player::SetDiagonal(bool _t)
{
	// Change whether the path will allow for diagonal movement, on change we need to re-calculate the path so we queue it
	m_path->diagonal = _t;
	m_diagonal = _t;
	this->QueueMidReset();
}