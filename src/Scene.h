#ifndef _SCENE_H
#define _SCENE_H

// Rory Clark
// http://rorywebdev.com
// 2016

#include "Player.h"
#include "Button.h"

class Scene
{
public:
	Scene(SDL_Renderer* _render, int _w, int _h, TTF_Font* _font);
	~Scene();

	void Update(float _delta, int _mouseX, int _mouseY, int _mouseState);
	void Draw();


private:

	SDL_Renderer* m_render;

	Player* m_player;
	Map* m_map;

	PathAlgorithm* m_algorithm;

	int m_windowW, m_windowH;

	bool m_moving, m_calcProgress;

	int m_startState;
	Tile* m_startTile;
	Tile* m_endTile;
	Tile* m_previousFollow;

	SDL_Color m_fontColor;
	TTF_Font* m_font;

	Text* m_pathInfo;

	void UpdateMouseSelection(float _delta, int _mouseX, int _mouseY, int _mouseState);

	Button* m_startPosButton;
	Button* m_endPosButton;

	Button* m_resetPathButton;
	Button* m_generateNewMap;

	int m_startBAllow, m_endBAllow;

	int m_wallWeight, m_grassWeight, m_stoneWeight, m_waterWeight, m_forestWeight;

	Text* m_wallWeightInfo;
	Button* m_incWallButton;
	Button* m_decWallButton;

	Text* m_grassWeightInfo;
	Button* m_incGrassButton;
	Button* m_decGrassButton;

	Text* m_stoneWeightInfo;
	Button* m_incStoneButton;
	Button* m_decStoneButton;

	Text* m_forestWeightInfo;
	Button* m_incForestButton;
	Button* m_decForestButton;

	Text* m_waterWeightInfo;
	Button* m_incWaterButton;
	Button* m_decWaterButton;

	Button* m_showOpenButton;

	Button* m_showClosedButton;

	Button* m_showPathButton;

	Button* m_startMoveButton;

	Button* m_recalcPathButton;

	Button* m_followMouseButton;

	Button* m_allowDiagonalButton;

	void UpdateWeighting(float _delta, int _mouseX, int _mouseY, int _mouseState);

	void ResetPathDetails();

};

#endif