#ifndef _PLAYER_H
#define _PLAYER_H

// Rory Clark
// http://rorywebdev.com
// 2016

#include "PathAlgorithm.h"
#include "Map.h"
#include "Text.h"

class Player
{
public:
	Player(SDL_Renderer* _render, Map* _map, PathAlgorithm* _algo);
	~Player();

	void Update(float _delta);
	void Draw();

	void SetText(Text* _text) { m_text = _text; };
	void SetEndText(Text* _text) { m_endText = _text; };
	
	void SetStartPosition(Tile* _startTile);
	void RemoveStartPosition();

	void SetDestination(Tile* _destTile);
	Tile* GetDestination() { return m_path->end; };
	void RemoveDestination();

	void ResetPath();

	bool CheckFailurePath();
	bool CheckCompletePath();

	void AddOpenTile(Tile* _tile);
	void AddClosedTile(Tile* _tile);

	bool GetShowPath() { return m_showPath; };
	void SetShowPath(bool _t) { m_showPath = _t; };

	bool GetMoving(){ return m_startMoving; };
	void SetMoving(bool _t){ m_startMoving = _t; };

	bool GetDiagonal() { return m_diagonal; };
	void SetDiagonal(bool _t);

	void QueueMidReset();

	float GetCalculateTime();

private:

	bool m_setStart,m_setEnd;
	bool m_resetAllow, m_resetStarted;

	SDL_Renderer* m_render;
	Map* m_map;
	PathAlgorithm* m_algorithm;
	Path* m_path;

	bool m_startMoving,m_finishedMoving;
	bool m_showPath;

	bool m_diagonal;

	const float m_normalTileDist = 1.0f;
	const float m_diagTileDist = 1.4f;
	
	Tile* m_oldMoveTile;
	Tile* m_newMoveTile;

	float m_moveToX, m_moveToY, m_moveToCost, m_moveDiffX, m_moveDiffY;
	bool m_moveXPos, m_moveYPos;

	SDL_Color m_color, m_endColor;
	SDL_Rect m_rect,m_endRect;

	Text* m_text;
	Text* m_endText;

	void UpdateMove(float _delta);

};

#endif