#include "PathAlgorithm.h"

// Rory Clark
// https://rorywebdev.com
// 2016

// A* Pathfinding algorithm
// Used as a singular processing class, not for information storage
// Path objects are to be created and stored in parent classes but processed within here
// It sorts and modifies the relevant data and will always return some form of result

// Simple sort function to organise the tiles based on their movement cost
bool SortTiles(const Tile* _i, const Tile* _j)
{
	return _i->fVal < _j->fVal;
}

PathAlgorithm::PathAlgorithm()
{
	// Set how long we want the algorithm to be calculating the path for each frame
	// Reduce the number for it take a fewer number of single frames but for it to take longer per frame
	m_timeLimit = 1.0f / 600.0f;
}

PathAlgorithm::~PathAlgorithm()
{

}

void PathAlgorithm::CalculatePath(Path* &_path)
{
	// Limit the algorithm CPU time to a variable limit, if we want lots of AI then decrease the amount of time spent
	m_timeCurrent = 0;
	m_newTicks = SDL_GetTicks();
	while (m_timeCurrent < m_timeLimit)
	{
		// Sort the time for the check so we don't go over the limit
		m_oldTicks = m_newTicks;
		m_newTicks = SDL_GetTicks();
		m_timeCurrent += (m_newTicks - m_oldTicks) / 1000.0f;
		_path->time += (m_newTicks - m_oldTicks) / 1000.0f;
		// Make sure we haven't already finished calculating the path
		if (!_path->calculated)
		{
			// Make sure we have open nodes to check
			if (_path->open.size() > 0)
			{
				// Sort our open nodes based on their F val (h + g)
				std::sort(_path->open.begin(), _path->open.end(), SortTiles);
				// Set a new vector to our open nodes (as we will be adding new tiles to the open nodes vector)
				std::vector<Tile*> tileCheck = _path->open;
				// Set our lowest value to the element at index 0
				float checkVal = tileCheck.at(0)->fVal;
				for each (Tile* tile in tileCheck)
				{
					// If we've got a tile with the lowest value then check it otherwise continue to the next cycle
					if (tile->fVal == checkVal)
					{
						// Calculate the value of the surrounding tiles (and add them to the open list)
						if (CalculateTileValue(tile, tileCheck, _path))
						{
							// Found the objective and created the path
							break;
						}
					}
					else
					{
						// Stop checking the lowest value, use it for the next frame
						break;
					}
				}
			}
			else
			{
				// Unable to reach end tile
				_path->calculated = true;
				_path->routed = true;
				_path->reached = false;
				break;
			}
		}
		if (_path->calculated)
		{
			// If we've calculated then stop and 
			break;
		}
	}
}

bool PathAlgorithm::CalculateTileValue(Tile* _tile, std::vector<Tile*> _sortedTiles, Path* &_path)
{
	// Allow the option of having diagonal movement or not
	
	int loop = 8;
	if (!_path->diagonal)
	{
		loop = 4;
	}

	// Loop through all our surrounding tiles
	for (int i = 0; i < loop; i++)
	{
		Tile* affector;
		int diag = 0;
		switch (i)
		{
		default:
		case 0:
			affector = _tile->u;
			break;
		case 1:
			affector = _tile->l;
			break;
		case 2:
			affector = _tile->r;
			break;
		case 3:
			affector = _tile->d;
			break;
		case 4:
			affector = _tile->ul;
			diag = 1;
			break;
		case 5:
			affector = _tile->ur;
			diag = 2;
			break;
		case 6:
			affector = _tile->dl;
			diag = 3;
			break;
		case 7:
			affector = _tile->dr;
			diag = 4;
			break;
		}
		// If the tile exists then check it
		if (affector != NULL)
		{
			// If the tile is the end node and can be accessed diagonally (if it is a diagonal tile)
			if (affector == _path->end && CheckTileCorners(_tile, diag))
			{
				_path->calculated = true;
				_path->reached = true;
				// Set the parent of the working tile to the originally passed tile
				affector->parent = _tile;
				Tile* traceBack = affector;
				SDL_Point point;
				_path->points.clear();
				_path->positions.clear();
				// Trace back each parent from the the final tile so we can create the overall path to be travelled
				while (traceBack != _path->start)
				{
					_path->positions.push_back(traceBack);
					point.x = traceBack->x + (traceBack->w / 2);
					point.y = traceBack->y + (traceBack->h / 2);
					_path->points.push_back(point);
					traceBack = traceBack->parent;
					// Make sure we limit it
					// Without this parent tiles can get confused and can create infinite while loops
					if (_path->positions.size() == _path->closed.size())
					{
						break;
					}
				}
				// Add the final point so that we draw an equal line
				point.x = _path->start->x + (_path->start->w / 2);
				point.y = _path->start->y + (_path->start->h / 2);
				_path->points.push_back(point);
				_path->positions.push_back(_path->start);
				_path->index = _path->positions.size() - 1;
				_path->routed = true;
				return true;
			}
			else
			{
				// If the tile in question hasn't been tested and can be accessed if diagonal
				if (!affector->closed && affector->state >= 0 && CheckTileCorners(_tile, diag))
				{
					// Set the heuristic value to the distance between the tile and the end point
					affector->hVal = CalculateDistance(affector, _path->end);
					// G is the cost of the original tile to the one being tested
					// If we're diagonal then use the diagonal value (1.4) otherwise use the normal (1.0)
					// Multiply it by the parameter value of the tile (e.g. water is highly expensive so would report an incredibly high value)
					if (diag != 0)
					{
						affector->gVal = m_diagTileDist * affector->parameter;
					}
					else
					{
						affector->gVal = m_normalTileDist * affector->parameter;
					}
					// Set the overall value of the tile to the G + H value
					affector->fVal = affector->gVal + affector->hVal;
					// If it's not open then add it to the open list and set it's parent to the original testing tile
					if (!affector->open)
					{
						AddOpenTile(affector,_path->open);
						affector->parent = _tile;
					}
				}
			}
		}
	}
	// Remove our original tile from the open nodes and add it to the closed
	_path->open.erase(_path->open.begin());
	AddClosedTile(_tile,_path->closed);
	return false;
}

bool PathAlgorithm::CheckTileCorners(Tile* _tile, int _dir)
{
	// Simple function used to test whether a tile can be accessed diagonally or not
	switch (_dir)
	{
	default:
		return true;
		break;
	case 1:
		if (_tile->u == NULL || _tile->l == NULL)
		{
			return false;
		}
		if (_tile->u->state >= 0 && _tile->l->state >= 0)
		{
			return true;
		}
		break;
	case 2:
		if (_tile->u == NULL || _tile->r == NULL)
		{
			return false;
		}
		if (_tile->u->state >= 0 && _tile->r->state >= 0)
		{
			return true;
		}
		break;
	case 3:
		if (_tile->d == NULL || _tile->l == NULL)
		{
			return false;
		}
		if (_tile->d->state >= 0 && _tile->l->state >= 0)
		{
			return true;
		}
		break;
	case 4:
		if (_tile->d == NULL || _tile->r == NULL)
		{
			return false;
		}
		if (_tile->d->state >= 0 && _tile->r->state >= 0)
		{
			return true;
		}
		break;
	}
	return false;
}

float PathAlgorithm::CalculateDistance(Tile* _start, Tile* _end)
{
	// Simple function to get the distance between tile A and B
	float xVal = _start->mapX - _end->mapX;
	float yVal = _start->mapY - _end->mapY;
	return sqrt((xVal * xVal) + (yVal * yVal));
}

void PathAlgorithm::AddOpenTile(Tile* _tile, std::vector<Tile*> &_open)
{
	// Add to the open tiles set
	_open.push_back(_tile);
	_open.back()->open = true;
	_open.back()->closed = false;
}

void PathAlgorithm::AddClosedTile(Tile* _tile, std::vector<Tile*> &_closed)
{
	// Add to the closed tiles set
	_closed.push_back(_tile);
	_closed.back()->open = false;
	_closed.back()->closed = true;
}

void PathAlgorithm::ResetPath(Path* &_path)
{
	// Clear out our path object
	// Also clears all of the values of the tiles used
	for each (Tile* tile in _path->open)
	{
		tile->open = false;
		tile->closed = false;
		tile->parent = NULL;
		tile->gVal = 0;
		tile->hVal = 0;
		tile->fVal = 0;
	}
	_path->open.clear();
	for each (Tile* tile in _path->closed)
	{
		tile->open = false;
		tile->closed = false;
		tile->parent = NULL;
		tile->gVal = 0;
		tile->hVal = 0;
		tile->fVal = 0;
	}
	_path->closed.clear();
	for each (Tile* tile in _path->positions)
	{
		tile->open = false;
		tile->closed = false;
		tile->parent = NULL;
		tile->gVal = 0;
		tile->hVal = 0;
		tile->fVal = 0;
	}
	_path->positions.clear();
	_path->points.clear();
	_path->calculated = false;
	_path->routed = false;
	_path->reached = false;
	_path->index = 0;
	_path->time = 0.0f;
	_path->start = NULL;
	_path->end = NULL;
}