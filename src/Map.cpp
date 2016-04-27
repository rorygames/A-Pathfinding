#include "Map.h"

// Rory Clark
// https://rorywebdev.com
// 2016

// Map class creates all of the tiles and assigns their data
// The map can have variable sizing and tile numbers
// Tile values are limited to pure int values meaning tiles with decimal widths and height will cause strange overall grid sizes

Map::Map(SDL_Renderer* _render)
{
	m_render = _render;
	m_mouseChoice = 0;
	// Set our default weighting values for our randoms
	m_grassWeight = 3;
	m_wallWeight = 1;
	m_stoneWeight = 1;
	m_waterWeight = 1;
	m_forestWeight = 1;
	m_showOpen = false;
	m_showClosed = false;
}

Map::~Map()
{
	// Remove all tiles on destruct
	ClearMap();
}

void Map::ClearMap(){
	// Delete each tile then clear our vectors
	// Clear does not delete memory in vector objects when using pointers
	for each (TileRow* row in m_tileRows)
	{
		for each(Tile* tile in row->tiles)
		{
			delete tile;
		}
		row->tiles.clear();
		delete row;
	}
	m_tileRows.clear();
}

void Map::Update(float _delta, int _mouseX, int _mouseY, int _mouseState)
{
	// Update the tiles when we want to choose the start and end points
	// Also run it when we want to get the tile to follow based on the mouse
	if (m_mouseChoice > 0 || m_followMouse)
	{
		m_followFound = false;
		for each (TileRow* row in m_tileRows)
		{
			for each (Tile* tile in row->tiles)
			{
				this->CheckMouseState(_mouseX, _mouseY, _mouseState, tile);
				if (m_followFound)
				{
					break;
				}
			}
			if (m_followFound)
			{
				break;
			}
		}
	}
}

void Map::Draw()
{
	// Draw each tile
	for each (TileRow* row in m_tileRows)
	{
		for each (Tile* tile in row->tiles)
		{
			SDL_Rect rect;
			rect.x = tile->x;
			rect.y = tile->y;
			rect.w = tile->w;
			rect.h = tile->h;
			switch (tile->state)
			{
			case -1:
				// Wall
				SDL_SetRenderDrawColor(m_render, 89, 38, 7, 255);
				break;
			case 0:
				// Grass
				SDL_SetRenderDrawColor(m_render, 23, 165, 30, 255);
				break;
			case 1:
				// Stone
				SDL_SetRenderDrawColor(m_render, 120, 120, 120, 255);
				break;
			case 2:
				// Forest
				SDL_SetRenderDrawColor(m_render, 10, 76, 13, 255);
				break;
			case 3:
				// Water
				SDL_SetRenderDrawColor(m_render, 21, 201, 228, 255);
				break;
			}
			// Draw the rectangle
			// Keeps things simple and easy to change
			SDL_RenderFillRect(m_render, &rect);
			if (m_mouseChoice > 0)
			{
				// Draw where we're selecting if we're choosing a tile start/end point
				switch (tile->mouse)
				{
				case 1:
					SDL_SetRenderDrawColor(m_render, 100, 100, 200, 255);
					SDL_RenderFillRect(m_render, &rect);
					break;
				case 2:
					SDL_SetRenderDrawColor(m_render, 200, 100, 100, 255);
					SDL_RenderFillRect(m_render, &rect);
					break;
				}
			}
			// Draw all our open tiles (if the user has the option enabled)
			if (tile->open && m_showOpen)
			{
				SDL_SetRenderDrawColor(m_render, 255, 255, 255, 150);
				SDL_RenderFillRect(m_render, &rect);
			}
			// Draw all the closed ones (if the user has the option enabled)
			if (tile->closed && m_showClosed)
			{
				SDL_SetRenderDrawColor(m_render, 255, 255, 0, 150);
				SDL_RenderFillRect(m_render, &rect);
			}
		}
	}
}

void Map::SetBounds(int _width, int _height, int _x, int _y, int _countX, int _countY)
{
	ClearMap();
	m_width = _width;
	m_height = _height;
	m_x = _x;
	m_y = _y;
	int xT = _countX, yT = _countY;
	int tW = m_width / xT, tH = m_height / yT;
	int remW = m_width % xT, remH = m_height % yT;

	// The weighted random system for generating the map
	// More numbers are added to the vector before generating based on the user's choice
	// E.g. 5 water and 5 grass would have the same effect as 1 water and 1 grass
	// While a weight of 5 to 1 would heavily out weigh the other
	std::vector<int> tileTypeWeight;

	for (int i = 0; i < m_grassWeight; i++)
	{
		tileTypeWeight.push_back(0);
	}

	for (int i = 0; i < m_wallWeight; i++)
	{
		tileTypeWeight.push_back(1);
	}

	for (int i = 0; i < m_stoneWeight; i++)
	{
		tileTypeWeight.push_back(2);
	}

	for (int i = 0; i < m_forestWeight; i++)
	{
		tileTypeWeight.push_back(3);
	}

	for (int i = 0; i < m_waterWeight; i++)
	{
		tileTypeWeight.push_back(4);
	}

	for (int y = 0; y < yT; y++)
	{
		m_tileRows.push_back(new TileRow());
		for (int x = 0; x < xT; x++)
		{
			// Create a new tile with "default" values other than the sizing and positions
			Tile* tempTile = new Tile();
			tempTile->w = tW;
			tempTile->h = tH;
			tempTile->x = m_x + (tW * x);
			tempTile->y = m_y + (tH * y);
			tempTile->parameter = 1;
			tempTile->closed = false;
			tempTile->open = false;
			tempTile->mapX = false;
			tempTile->parent = NULL;
			// Choose our random type of terrain
			// Set our parameter value, this will be used for the cost to move to that terrain
			int randVal = rand() % tileTypeWeight.size();
			switch (tileTypeWeight.at(randVal))
			{
			default:
			case 0:
				tempTile->parameter = GRASS_VALUE;
				tempTile->state = 0;
				break;
			case 1:
				tempTile->state = -1;
				break;
			case 2:
				tempTile->parameter = STONE_VALUE;
				tempTile->state = 1;
				break;
			case 3:
				tempTile->parameter = FOREST_VALUE;
				tempTile->state = 2;
				break;
			case 4:
				tempTile->parameter = WATER_VALUE;
				tempTile->state = 3;
				break;
			}
			
			// Set the raw X and Y values
			tempTile->mapX = x;
			tempTile->mapY = y;
			
			// Assign our surrounding values so each tile can connect to each other
			// Doing so allows our algorithm to work efficiently and at high speed
			if (x > 0)
			{
				tempTile->l = m_tileRows.back()->tiles.back();
				m_tileRows.back()->tiles.back()->r = tempTile;
				if (y > 0)
				{
					tempTile->u = m_tileRows.at(m_tileRows.size() - 2)->tiles.at(x);
					tempTile->ul = m_tileRows.at(m_tileRows.size() - 2)->tiles.at(x-1);
					m_tileRows.at(m_tileRows.size() - 2)->tiles.at(x)->d = tempTile;
					m_tileRows.at(m_tileRows.size() - 2)->tiles.at(x-1)->dr = tempTile;
					if (x < xT - 1)
					{
						tempTile->ur = m_tileRows.at(m_tileRows.size() - 2)->tiles.at(x+1);
						m_tileRows.at(m_tileRows.size() - 2)->tiles.at(x + 1)->dl = tempTile;
					}
				}
			}
			if (x == 0)
			{
				if (y>0)
				{
					tempTile->u = m_tileRows.at(m_tileRows.size() - 2)->tiles.at(x);
					m_tileRows.at(m_tileRows.size() - 2)->tiles.at(x)->d = tempTile;
					tempTile->ur = m_tileRows.at(m_tileRows.size() - 2)->tiles.at(x + 1);
				}
			}
			m_tileRows.back()->tiles.push_back(tempTile);
		}
	}
}

Tile* Map::GetTileInfo(int _x, int _y)
{
	// Get a tile at a specific X and Y value relative to the grid sizing
	if (m_tileRows.at(_y)->tiles.at(_x) != NULL)
	{
		return m_tileRows.at(_y)->tiles.at(_x);
	}
	return NULL;
}

void Map::CheckMouseState(int _mouseX, int _mouseY, int _mouseState, Tile* _tile)
{
	// If we're following the mouse then we set the currently hovered tile to the current destination tile
	if (m_followMouse)
	{
		if ((_mouseX >= _tile->x && _mouseX < (_tile->x + _tile->w)) && (_mouseY >= _tile->y && _mouseY < (_tile->y + _tile->h)) && _tile->state != -1)
		{
			m_followTile = _tile;
			m_followFound = true;
		}
	}
	else
	{
		m_followTile = NULL;
	}

	// Similar code to how the buttons work but only for selecting our tiles or for following the mouse
	if (m_startTile == NULL || m_endTile == NULL)
	{
		if ((_mouseX >= _tile->x && _mouseX < (_tile->x + _tile->w)) && (_mouseY >= _tile->y && _mouseY < (_tile->y + _tile->h)))
		{
			if (_tile->state >= 0 && CheckSurroundings(_tile) && _tile != m_startTile && _tile != m_endTile)
				switch (_mouseState)
			{
				case 0:
					// If the user moves the mouse off of the X & Y then we don't want to be sending the return value when they let go of the mouse
					if (_tile->mouse == 2)
					{
						m_selectedTile = _tile;
						if (m_mouseChoice == 1)
						{
							m_startTile = _tile;
						}
						if (m_mouseChoice == 2)
						{
							m_endTile = _tile;
						}
						m_mouseChoice = 0;

					}
					else
					{
						_tile->mouse = 1;
					}

					break;
				case 1:
					if (_tile->mouse != -1)
					{
						if (m_selectedTile == NULL)
						{
							_tile->mouse = 2;
						}
					}
					break;
			}

		}
		else
		{
			if (_mouseState > 0)
			{
				_tile->mouse = -1;
			}
			else
			{
				if (m_selectedTile == NULL)
				{
					_tile->mouse = 0;
				}
			}
		}
	}
}

Tile* Map::GetSelectedTile()
{
	// Get the selected tile (used by the scene to understand the start and end points)
	if (m_selectedTile != NULL)
	{
		Tile* _tile = m_selectedTile;
		m_selectedTile = NULL;
		return _tile;
	}
	return NULL;
}

Tile* Map::GetFollowTile()
{
	// Get the current following tile
	return m_followTile;
}

bool Map::CheckSurroundings(Tile* _tile)
{
	// Make sure the tile in question is not a singular tile (prevents the user from instantly creating impossible path options)
	if ((_tile->u == NULL || _tile->u->state == -1) && (_tile->l == NULL || _tile->l->state == -1) && (_tile->d == NULL || _tile->d->state == -1) && (_tile->r == NULL || _tile->r->state == -1))
	{
		return false;
	}
	return true;
}

void Map::ResetStartEnd()
{
	// Reset the start and end points of the tiles
	m_startTile = NULL;
	m_endTile = NULL;
}