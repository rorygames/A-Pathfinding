#ifndef _TILE_H
#define _TILE_H

// Rory Clark
// http://rorywebdev.com
// 2016

struct Tile
{
	// Tile object
	int x, y, w, h, mapX, mapY;
	// Which type of tile is it
	int state;
	// Cost of movement to the tile
	float parameter;
	// What's the mouse doing to the tile?
	int mouse;
	// All of the surrouding tiles
	// u = up, d = down, l = left, r = right, ul = upper left, ur = upper right, dl = down left, dr = down right
	Tile* u;
	Tile* ul;
	Tile* ur;
	Tile* l;
	Tile* r;
	Tile* dl;
	Tile* dr;
	Tile* d;
	// Parent tile (for path creation)
	Tile* parent;
	// Algorithm values
	float gVal, hVal, fVal;
	// Whether the tile is open or closed
	bool open, closed;
};

#endif