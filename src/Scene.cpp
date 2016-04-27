#include "Scene.h"

Scene::Scene(SDL_Renderer* _render, int _w, int _h, TTF_Font* _font)
{
	// Create the map object
	m_map = new Map(_render);

	// Create the algorithm
	m_algorithm = new PathAlgorithm();

	// Create the player
	m_player = new Player(_render,m_map,m_algorithm);

	SDL_Color white = { 255, 255, 255 };
	m_fontColor = white;
	
	m_windowW = _w;
	m_windowH = _h;

	// Create our tile map
	m_map->SetBounds(m_windowH - 40, m_windowH - 40, 20, 20, 40, 40);

	m_render = _render;
	m_map->SetMouseChoice(0);
	m_startState = 0;
	m_font = _font;
	
	// Set the text for the start and end points
	Text* pText = new Text(m_render);
	pText->LoadText("P", m_fontColor, m_font);
	m_player->SetText(pText);

	pText = new Text(m_render);
	pText->LoadText("E", m_fontColor, m_font);
	m_player->SetEndText(pText);

	m_startBAllow = 0;
	m_endBAllow = 0;

	// Get the set weighting values for the randoms
	m_grassWeight = m_map->GetGrassWeight();
	m_wallWeight = m_map->GetWallWeight();
	m_stoneWeight = m_map->GetStoneWeight();
	m_forestWeight = m_map->GetForestWeight();
	m_waterWeight = m_map->GetWaterWeight();

	// Create all the text and buttons to be used for the UI

	m_startPosButton = new Button(m_render, "Set Start Position", m_font, white, m_windowH, 20, 120, 30, 1, false);
	m_startPosButton->SetColor(50, 50, 200);

	m_endPosButton = new Button(m_render, "Set End Position", m_font, white, (m_windowH + 130), 20, 120, 30, 1, false);
	m_endPosButton->SetColor(200, 50, 50);

	m_pathInfo = new Text(m_render);
	m_pathInfo->LoadText("Waiting for path. Please choose a start and end position.", white, m_font);
	m_pathInfo->ChangePos(m_windowH, 55);

	m_resetPathButton = new Button(m_render, "Reset Path", m_font, white, m_windowH, 80, 250, 30, 1, false);
	m_resetPathButton->SetColor(20, 163, 36);

	m_generateNewMap = new Button(m_render, "Generate New Grid", m_font, white, m_windowH, 120, 250, 30, 1, false);
	m_generateNewMap->SetColor(125, 20, 163);

	m_grassWeightInfo = new Text(m_render);
	m_grassWeightInfo->LoadText("Grass Weight: " + std::to_string(m_grassWeight), white, m_font);
	m_grassWeightInfo->ChangePos(m_windowH + 40, 165);
	m_decGrassButton = new Button(m_render, "-", m_font, white, m_windowH, 160, 30, 30, 1, false);
	m_decGrassButton->SetColor(23, 165, 30);
	m_incGrassButton = new Button(m_render, "+", m_font, white, m_windowH + 220, 160, 30, 30, 1, false);
	m_incGrassButton->SetColor(23, 165, 30);

	m_wallWeightInfo = new Text(m_render);
	m_wallWeightInfo->LoadText("Wall Weight: " + std::to_string(m_wallWeight), white, m_font);
	m_wallWeightInfo->ChangePos(m_windowH + 40, 205);
	m_decWallButton = new Button(m_render, "-", m_font, white, m_windowH, 200, 30, 30, 1, false);
	m_decWallButton->SetColor(89, 38, 7);
	m_incWallButton = new Button(m_render, "+", m_font, white, m_windowH + 220, 200, 30, 30, 1, false);
	m_incWallButton->SetColor(89, 38, 7);

	m_stoneWeightInfo = new Text(m_render);
	m_stoneWeightInfo->LoadText("Stone Weight: " + std::to_string(m_stoneWeight), white, m_font);
	m_stoneWeightInfo->ChangePos(m_windowH + 40, 245);
	m_decStoneButton = new Button(m_render, "-", m_font, white, m_windowH, 240, 30, 30, 1, false);
	m_decStoneButton->SetColor(120, 120, 120);
	m_incStoneButton = new Button(m_render, "+", m_font, white, m_windowH + 220, 240, 30, 30, 1, false);
	m_incStoneButton->SetColor(120, 120, 120);

	m_forestWeightInfo = new Text(m_render);
	m_forestWeightInfo->LoadText("Forest Weight: " + std::to_string(m_forestWeight), white, m_font);
	m_forestWeightInfo->ChangePos(m_windowH + 40, 285);
	m_decForestButton = new Button(m_render, "-", m_font, white, m_windowH, 280, 30, 30, 1, false);
	m_decForestButton->SetColor(10, 76, 13);
	m_incForestButton = new Button(m_render, "+", m_font, white, m_windowH + 220, 280, 30, 30, 1, false);
	m_incForestButton->SetColor(10, 76, 13);

	m_waterWeightInfo = new Text(m_render);
	m_waterWeightInfo->LoadText("Water Weight: " + std::to_string(m_waterWeight), white, m_font);
	m_waterWeightInfo->ChangePos(m_windowH + 40, 325);
	m_decWaterButton = new Button(m_render, "-", m_font, white, m_windowH, 320, 30, 30, 1, false);
	m_decWaterButton->SetColor(21, 201, 228);
	m_incWaterButton = new Button(m_render, "+", m_font, white, m_windowH + 220, 320, 30, 30, 1, false);
	m_incWaterButton->SetColor(21, 201, 228);

	m_showOpenButton = new Button(m_render, "Show Open Nodes", m_font, white, m_windowH, 360, 250, 30, 1, false);
	
	m_showClosedButton = new Button(m_render, "Show Closed Nodes", m_font, white, m_windowH, 400, 250, 30, 1, false);

	m_showPathButton = new Button(m_render, "Hide Completed Path", m_font, white, m_windowH, 440, 250, 30, 1, false);

	m_startMoveButton = new Button(m_render, "Start Movement", m_font, white, m_windowH, 480, 250, 30, 1, false);

	m_recalcPathButton = new Button(m_render, "Recalculate Path", m_font, white, m_windowH, 520, 250, 30, 1, false);

	m_followMouseButton = new Button(m_render, "Follow Mouse", m_font, white, m_windowH, 560, 250, 30, 1, false);

	m_allowDiagonalButton = new Button(m_render, "Prevent Diagonal Movement", m_font, white, m_windowH, 600, 250, 30, 1, false);

	m_moving = false;

}

Scene::~Scene()
{
	// Delete all our buttons and memory objects

	delete m_pathInfo;

	delete m_startPosButton;
	delete m_endPosButton;

	delete m_resetPathButton;
	delete m_generateNewMap;

	delete m_wallWeightInfo;
	delete m_incWallButton;
	delete m_decWallButton;

	delete m_grassWeightInfo;
	delete m_incGrassButton;
	delete m_decGrassButton;

	delete m_stoneWeightInfo;
	delete m_incStoneButton;
	delete m_decStoneButton;

	delete m_forestWeightInfo;
	delete m_incForestButton;
	delete m_decForestButton;

	delete m_waterWeightInfo;
	delete m_incWaterButton;
	delete m_decWaterButton;

	delete m_showOpenButton;

	delete m_showClosedButton;

	delete m_showPathButton;

	delete m_startMoveButton;

	delete m_recalcPathButton;

	delete m_followMouseButton;

	delete m_allowDiagonalButton;

	delete m_algorithm;
	delete m_map;
	delete m_player;
}

void Scene::Update(float _delta, int _mouseX, int _mouseY, int _mouseState)
{
	// Update the map, buttons and player
	m_map->Update(_delta, _mouseX, _mouseY, _mouseState);
	this->UpdateMouseSelection(_delta, _mouseX, _mouseY, _mouseState);
	m_player->Update(_delta);	
}

void Scene::UpdateMouseSelection(float _delta, int _mouseX, int _mouseY, int _mouseState)
{
	// Set the start tile
	if (m_startTile == NULL)
	{
		int startInt = (int)m_startPosButton->Update(_delta, _mouseX, _mouseY, _mouseState);
		if (startInt != 0 && m_endBAllow == 0)
		{
			m_startBAllow = 1;
			m_startPosButton->ChangeText("Start: PICKING");
			m_map->SetMouseChoice(1);
		}
	}
	// ..and the end tile
	if (m_endTile == NULL)
	{
		int endInt = (int)m_endPosButton->Update(_delta, _mouseX, _mouseY, _mouseState);
		if (endInt != 0 && m_startBAllow == 0)
		{
			m_endBAllow = 1;
			m_endPosButton->ChangeText("End: PICKING");
			m_map->SetMouseChoice(2);
		}
	}
	// If we're not pressing down the mouse
	if (_mouseState == 0)
	{
		// We're currently selecting a start point
		if (m_startTile == NULL && m_startBAllow != 0)
		{
			Tile* selected = m_map->GetSelectedTile();
			if (selected != NULL)
			{
				m_startTile = selected;
				m_startPosButton->ChangeText("Start: X" + std::to_string(m_startTile->mapX) + " Y" + std::to_string(m_startTile->mapY));
				m_player->SetStartPosition(m_startTile);
				m_startBAllow = 0;
				if (m_endTile != NULL)
				{
					m_pathInfo->ChangeText("Calculating Path");
					m_calcProgress = false;
				}
			}
		}
		// Currently selecting an end point
		if (m_endTile == NULL && m_endBAllow != 0)
		{
			Tile* selected = m_map->GetSelectedTile();
			if (selected != NULL)
			{
				m_endTile = selected;
				m_endPosButton->ChangeText("End: X" + std::to_string(m_endTile->mapX) + " Y" + std::to_string(m_endTile->mapY));
				m_player->SetDestination(m_endTile);
				m_endBAllow = 0;
				if (m_startTile != NULL)
				{
					m_pathInfo->ChangeText("Calculating Path");
					m_calcProgress = false;
				}
			}
		}
	}
	// Reset the path and update the info text if no path was successfully found
	if (m_startTile != NULL && m_endTile != NULL && m_player->CheckFailurePath())
	{
		if (m_map->GetFollowMouse())
		{
			m_map->SetFollowMouse(false);
			m_followMouseButton->ChangeText("Follow Mouse");
			m_player->SetDestination(m_endTile);
			m_player->QueueMidReset();
			m_pathInfo->ChangeText("No path found. Original destination has been set.");
		}
		else
		{
			ResetPathDetails();
			m_pathInfo->ChangeText("No path found. Please choose different start and end positions.");
		}
	}
	// Calculated path, display the time to calculate
	if (m_startTile != NULL && m_endTile != NULL && m_player->CheckCompletePath() && !m_calcProgress)
	{
		m_calcProgress = true;
		m_pathInfo->ChangeText("Path found in " + std::to_string(m_player->GetCalculateTime()) + " seconds.");
	}

	if (m_map->GetFollowMouse() && m_map->GetFollowTile() != m_previousFollow && m_player->CheckCompletePath())
	{
		m_previousFollow = m_map->GetFollowTile();
		m_pathInfo->ChangeText("Mouse path found in " + std::to_string(m_player->GetCalculateTime()) + " seconds.");
	}

	// Update all our buttons
	if (m_resetPathButton->Update(_delta, _mouseX, _mouseY, _mouseState))
	{
		// Reset the path
		ResetPathDetails();
		m_pathInfo->ChangeText("Path has been reset. Please select new start and end positions.");

	}
	if (m_generateNewMap->Update(_delta, _mouseX, _mouseY, _mouseState))
	{
		// Recreate the tile map
		ResetPathDetails();
		m_map->SetBounds(m_windowH - 40, m_windowH - 40, 20, 20, 40, 40);
		m_pathInfo->ChangeText("New map has been created. Please select new start and end positions.");
	}

	UpdateWeighting(_delta, _mouseX, _mouseY, _mouseState);

	if (m_showOpenButton->Update(_delta, _mouseX, _mouseY, _mouseState))
	{
		// Show open nodes
		if (m_map->GetShowOpen())
		{
			m_map->SetShowOpen(false);
			m_showOpenButton->ChangeText("Show Open Nodes");
		}
		else
		{
			m_map->SetShowOpen(true);
			m_showOpenButton->ChangeText("Hide Open Nodes");
		}
	}

	if (m_showClosedButton->Update(_delta, _mouseX, _mouseY, _mouseState))
	{
		// Show closed nodes
		if (m_map->GetShowClosed())
		{
			m_map->SetShowClosed(false);
			m_showClosedButton->ChangeText("Show Closed Nodes");
		}
		else
		{
			m_map->SetShowClosed(true);
			m_showClosedButton->ChangeText("Hide Closed Nodes");
		}
	}

	if (m_showPathButton->Update(_delta, _mouseX, _mouseY, _mouseState))
	{
		// Show the completed path
		if (m_player->GetShowPath())
		{
			m_player->SetShowPath(false);
			m_showPathButton->ChangeText("Show Completed Path");
		}
		else
		{
			m_player->SetShowPath(true);
			m_showPathButton->ChangeText("Hide Completed Path");
		}
	}

	if (m_player->GetMoving() != m_moving || m_startMoveButton->Update(_delta, _mouseX, _mouseY, _mouseState))
	{
		// Allow movement
		if (m_player->GetMoving())
		{
			m_player->SetMoving(false);
			m_startMoveButton->ChangeText("Start Movement");
			m_moving = false;
		}
		else
		{
			m_player->SetMoving(true);
			m_startMoveButton->ChangeText("Stop Movement");
			m_moving = true;
		}
	}

	if (m_recalcPathButton->Update(_delta, _mouseX, _mouseY, _mouseState) && m_startTile != NULL && m_endTile != NULL)
	{
		// Force recalculate the path
		m_player->QueueMidReset();
	}

	if (m_followMouseButton->Update(_delta, _mouseX, _mouseY, _mouseState) && m_startTile != NULL && m_endTile != NULL)
	{
		// Follow the mouse
		if (m_map->GetFollowMouse())
		{
			m_map->SetFollowMouse(false);
			m_followMouseButton->ChangeText("Follow Mouse");
			m_player->SetDestination(m_endTile);
			m_player->QueueMidReset();
		}
		else
		{
			m_map->SetFollowMouse(true);
			m_followMouseButton->ChangeText("Stop Following Mouse");
		}
	}

	if (m_allowDiagonalButton->Update(_delta, _mouseX, _mouseY, _mouseState))
	{
		if (m_player->GetDiagonal())
		{
			m_player->SetDiagonal(false);
			m_allowDiagonalButton->ChangeText("Allow Diagonal Movement");
		}
		else
		{
			m_player->SetDiagonal(true);
			m_allowDiagonalButton->ChangeText("Prevent Diagonal Movement");
		}
	}

	if (m_map->GetFollowMouse())
	{
		// If we're allowing the mouse to be followed then set the new tile to be followed if it's different to the old
		Tile* follow = m_map->GetFollowTile();
		if (follow != NULL && follow != m_player->GetDestination())
		{
			m_player->SetDestination(follow);
			m_player->QueueMidReset();
		}
	}
}

void Scene::UpdateWeighting(float _delta, int _mouseX, int _mouseY, int _mouseState)
{
	// Update all of the text and values for the random weighting of the tile map

	if (m_incWallButton->Update(_delta, _mouseX, _mouseY, _mouseState))
	{
		if (m_wallWeight < 5)
		{
			m_wallWeight++;
			m_map->SetWallWeight(m_wallWeight);
			m_wallWeightInfo->ChangeText("Wall Weight: " + std::to_string(m_wallWeight));
		}
	}
	if (m_decWallButton->Update(_delta, _mouseX, _mouseY, _mouseState))
	{
		if (m_wallWeight > 0)
		{
			m_wallWeight--;
			m_map->SetWallWeight(m_wallWeight);
			m_wallWeightInfo->ChangeText("Wall Weight: " + std::to_string(m_wallWeight));
		}
	}

	if (m_incGrassButton->Update(_delta, _mouseX, _mouseY, _mouseState))
	{
		if (m_grassWeight < 10)
		{
			m_grassWeight++;
			m_map->SetGrassWeight(m_grassWeight);
			m_grassWeightInfo->ChangeText("Grass Weight: " + std::to_string(m_grassWeight));
		}
	}
	if (m_decGrassButton->Update(_delta, _mouseX, _mouseY, _mouseState))
	{
		if (m_grassWeight > 1)
		{
			m_grassWeight--;
			m_map->SetGrassWeight(m_grassWeight);
			m_grassWeightInfo->ChangeText("Grass Weight: " + std::to_string(m_grassWeight));
		}
	}

	if (m_incStoneButton->Update(_delta, _mouseX, _mouseY, _mouseState))
	{
		if (m_stoneWeight < 10)
		{
			m_stoneWeight++;
			m_map->SetStoneWeight(m_stoneWeight);
			m_stoneWeightInfo->ChangeText("Stone Weight: " + std::to_string(m_stoneWeight));
		}
	}
	if (m_decStoneButton->Update(_delta, _mouseX, _mouseY, _mouseState))
	{
		if (m_stoneWeight > 0)
		{
			m_stoneWeight--;
			m_map->SetStoneWeight(m_stoneWeight);
			m_stoneWeightInfo->ChangeText("Stone Weight: " + std::to_string(m_stoneWeight));
		}
	}

	if (m_incForestButton->Update(_delta, _mouseX, _mouseY, _mouseState))
	{
		if (m_forestWeight < 10)
		{
			m_forestWeight++;
			m_map->SetForestWeight(m_forestWeight);
			m_forestWeightInfo->ChangeText("Forest Weight: " + std::to_string(m_forestWeight));
		}
	}
	if (m_decForestButton->Update(_delta, _mouseX, _mouseY, _mouseState))
	{
		if (m_forestWeight > 0)
		{
			m_forestWeight--;
			m_map->SetForestWeight(m_forestWeight);
			m_forestWeightInfo->ChangeText("Forest Weight: " + std::to_string(m_forestWeight));
		}
	}

	if (m_incWaterButton->Update(_delta, _mouseX, _mouseY, _mouseState))
	{
		if (m_waterWeight < 10)
		{
			m_waterWeight++;
			m_map->SetWaterWeight(m_waterWeight);
			m_waterWeightInfo->ChangeText("Water Weight: " + std::to_string(m_waterWeight));
		}
	}
	if (m_decWaterButton->Update(_delta, _mouseX, _mouseY, _mouseState))
	{
		if (m_waterWeight > 0)
		{
			m_waterWeight--;
			m_map->SetWaterWeight(m_waterWeight);
			m_waterWeightInfo->ChangeText("Water Weight: " + std::to_string(m_waterWeight));
		}
	}
}

void Scene::Draw()
{
	// Draw the map, player and all the buttons of the UI

	m_map->Draw();
	m_player->Draw();

	m_startPosButton->Draw();
	m_endPosButton->Draw();

	m_resetPathButton->Draw();
	m_generateNewMap->Draw();

	m_wallWeightInfo->Draw();
	m_incWallButton->Draw();
	m_decWallButton->Draw();

	m_grassWeightInfo->Draw();
	m_incGrassButton->Draw();
	m_decGrassButton->Draw();

	m_stoneWeightInfo->Draw();
	m_incStoneButton->Draw();
	m_decStoneButton->Draw();

	m_forestWeightInfo->Draw();
	m_incForestButton->Draw();
	m_decForestButton->Draw();

	m_waterWeightInfo->Draw();
	m_incWaterButton->Draw();
	m_decWaterButton->Draw();

	m_showClosedButton->Draw();
	m_showOpenButton->Draw();
	m_showPathButton->Draw();

	m_startMoveButton->Draw();

	m_recalcPathButton->Draw();

	m_followMouseButton->Draw();

	m_allowDiagonalButton->Draw();

	m_pathInfo->Draw();
}

void Scene::ResetPathDetails()
{
	// Reset our path and related systems
	m_startTile = NULL;
	m_endTile = NULL;
	m_player->RemoveStartPosition();
	m_player->RemoveDestination();
	m_player->ResetPath();
	m_startPosButton->ChangeText("Set Start Position");
	m_endPosButton->ChangeText("Set End Position");
	m_startBAllow = 0;
	m_endBAllow = 0;
	m_map->ResetStartEnd();
	m_calcProgress = false;
	m_map->SetFollowMouse(false);
}