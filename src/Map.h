#ifndef _MAP_H
#define _MAP_H

// Rory Clark
// http://rorywebdev.com
// 2016

#include "Text.h"
#include "Tile.h"

struct TileRow
{
	// Generice tile row struct, a vector of these tile rows will act as the Y values, the row itself as the X
	std::vector<Tile*> tiles;
};

static const float GRASS_VALUE = 0.9f;
static const float STONE_VALUE = 1.2f;
static const float FOREST_VALUE = 1.8f;
static const float WATER_VALUE = 2.5f;

class Map
{
public:
	Map(SDL_Renderer* _render);
	~Map();

	void Update(float _delta, int _mouseX, int _mouseY, int _mouseState);
	void Draw();

	void SetBounds(int _width, int _height, int _x, int _y, int _countX, int _countY);

	Tile* GetTileInfo(int _x, int _y);

	Tile* GetSelectedTile();

	void SetMouseChoice(int _c) { m_mouseChoice = _c; };

	void ResetStartEnd();

	int GetWallWeight() { return m_wallWeight; };
	int GetGrassWeight() { return m_grassWeight; };
	int GetStoneWeight() { return m_stoneWeight; };
	int GetForestWeight() { return m_forestWeight; };
	int GetWaterWeight() { return m_waterWeight; };

	void SetWallWeight(int _w) { m_wallWeight = _w; };
	void SetGrassWeight(int _w) { m_grassWeight = _w; };
	void SetStoneWeight(int _w) { m_stoneWeight = _w; };
	void SetForestWeight(int _w) { m_forestWeight = _w; };
	void SetWaterWeight(int _w) { m_waterWeight = _w; };

	bool GetShowOpen() { return m_showOpen; };
	bool GetShowClosed() { return m_showClosed; };

	void SetShowOpen(bool _t) { m_showOpen = _t; };
	void SetShowClosed(bool _t) { m_showClosed = _t; };

	void SetFollowMouse(bool _t) { m_followMouse = _t; m_followTile = NULL; };
	bool GetFollowMouse() { return m_followMouse; };
	Tile* GetFollowTile();

private:

	int m_width, m_height, m_x, m_y;
	bool m_showOpen, m_showClosed, m_followMouse;
	bool m_followFound;
	std::vector<TileRow*> m_tileRows;

	SDL_Renderer* m_render;
	Tile* m_selectedTile;
	Tile* m_followTile;

	Tile* m_startTile;
	Tile* m_endTile;

	int m_mouseChoice;

	int m_wallWeight, m_grassWeight, m_stoneWeight, m_waterWeight, m_forestWeight;

	void CheckMouseState(int _mouseX, int _mouseY, int _mouseState, Tile* _tile);
	bool CheckSurroundings(Tile* _tile);
	void ClearMap();
	
};

#endif