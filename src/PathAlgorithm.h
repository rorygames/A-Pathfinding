#ifndef _PATHALGO_H
#define _PATHALGO_H

// Rory Clark
// http://rorywebdev.com
// 2016

#include "SDL.h"
#include <vector>
#include <string>
#include <algorithm>
#include "Tile.h"

struct Path
{
	// The path object
	// These objects should be created and stored within your usage classes
	// The overall algorithm class is designed to simply process the objects

	// Start tile
	Tile* start;

	// End tile
	Tile* end;

	// Open nodes
	std::vector<Tile*> open;

	// Closed nodes
	std::vector<Tile*> closed;
	
	// Path positions
	std::vector<Tile*> positions;

	// Points used for drawing the path (can be removed if not using SDL)
	std::vector<SDL_Point> points;

	// Calculated the path? Routed the overall path? Did we reach the end tile?
	bool calculated = false;
	bool routed = false;
	bool reached = false;

	bool diagonal = true;

	// At what point are we currently moving in (used in turn with positions)
	int index = 0;
	
	// How long it took to calculate the path
	float time = 0.0f;
};

class PathAlgorithm
{
public:
	PathAlgorithm();
	~PathAlgorithm();

	// Main function to calculate the path object,
	void CalculatePath(Path* &_path);

	float CalculateDistance(Tile* _a, Tile* _b);
	bool CheckTileCorners(Tile* _tile, int _dir);

	void ResetPath(Path* &_path);

private:

	float m_timeLimit, m_timeCurrent;
	Uint32 m_oldTicks, m_newTicks;

	const float m_normalTileDist = 1.0f;
	const float m_diagTileDist = 1.4f;


	bool CalculateTileValue(Tile* _tile, std::vector<Tile*> _sortedTiles, Path* &_path);

	void AddClosedTile(Tile* _tile, std::vector<Tile*> &_closed);
	void AddOpenTile(Tile* _tile, std::vector<Tile*> &_open);

};

#endif