/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2015
*/

/*
	F4					activate
	NUM2/8/4/6			navigate thru the menus and lists (numlock must be on)
	NUM5 				select
	NUM0/BACKSPACE/F4 	back
	NUM9/3 				use vehicle boost when active
	NUM+ 				use vehicle rockets when active
*/

#include "script.h"
#include "keyboard.h"

#include <string>
#include <ctime>

#pragma warning(disable : 4244 4305) // double <-> float conversions

void draw_text(char* text, float x, float y, float scale) {
	UI::SET_TEXT_FONT(0);
	UI::SET_TEXT_SCALE(scale, scale);
	UI::SET_TEXT_COLOUR(255, 255, 255, 245);
	UI::SET_TEXT_WRAP(0.0, 1.0);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(2, 2, 0, 0, 0);
	UI::SET_TEXT_EDGE(1, 0, 0, 0, 205);
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(text);
	UI::_DRAW_TEXT(y, x);
}

static struct {
	LPCSTR  text;
	float x;
	float y;
	float z;
} predefined_coords[] = {
		{ "MARKER" },
		{ "MICHAEL'S HOUSE", -852.4f, 160.0f, 65.6f },
		{ "FRANKLIN'S HOUSE", 7.9f, 548.1f, 175.5f },
		{ "TREVOR'S TRAILER", 1985.7f, 3812.2f, 32.2f },
		{ "AIRPORT ENTRANCE", -1034.6f, -2733.6f, 13.8f },
		{ "AIRPORT FIELD", -1336.0f, -3044.0f, 13.9f },
		{ "ELYSIAN ISLAND", 338.2f, -2715.9f, 38.5f },
		{ "JETSAM", 760.4f, -2943.2f, 5.8f },
		{ "STRIPCLUB", 127.4f, -1307.7f, 29.2f },
		{ "ELBURRO HEIGHTS", 1384.0f, -2057.1f, 52.0f },
		{ "FERRIS WHEEL", -1670.7f, -1125.0f, 13.0f },
		{ "CHUMASH", -3192.6f, 1100.0f, 20.2f },
		{ "WINDFARM", 2354.0f, 1830.3f, 101.1f },
		{ "MILITARY BASE", -2047.4f, 3132.1f, 32.8f },
		{ "MCKENZIE AIRFIELD", 2121.7f, 4796.3f, 41.1f },
		{ "DESERT AIRFIELD", 1747.0f, 3273.7f, 41.1f },
		{ "CHILLIAD", 425.4f, 5614.3f, 766.5f }
};

void setZCoord(Vector3& coords) {
	bool groundFound = false;
	static float groundCheckHeight[] = {
		0.0, 50.0, 100.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0,
		450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0
	};
	Entity e = PLAYER::PLAYER_PED_ID();
	for (int i = 0; sizeof(groundCheckHeight) / sizeof(float); i++) {
		//ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, groundCheckHeight[i], 0, 0, 1);
		STREAMING::CLEAR_FOCUS();
		STREAMING::CLEAR_HD_AREA();
		STREAMING::_SET_FOCUS_AREA(coords.x, coords.y, groundCheckHeight[i], 0, 0, 0);
		WAIT(200);
		if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, groundCheckHeight[i], &coords.z, false)) {
			groundFound = true;
			// force to above sea level
			if (coords.z < 0) {
				coords.z = 0;
			}
			coords.z += GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(3.0, 13.0);
			break;
		}
	}
	// if ground not found then set Z in air
	if (!groundFound) {
		coords.z = 300.0;
	}
}

void setGroundZ(Vector3& coords) {
	static float firstCheck[] = { 0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000 };

	static float secondCheck[] = { 1000, 900, 800, 700, 600, 500,
		400, 300, 200, 100, 0, -100, -200, -300, -400, -500 };

	static float thirdCheck[] = { -500, -400, -300, -200, -100, 0,
		100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};

	if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, 1000.0, &coords.z, false)) {
		return;
	}

	for (int i = 0; sizeof(firstCheck) / sizeof(float); i++) {
		if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, firstCheck[i], &coords.z, false)) {
			return;
		}
	}

	for (int i = 0; sizeof(secondCheck) / sizeof(float); i++) {
		if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, secondCheck[i], &coords.z, false)) {
			return;
		}
	}
	for (int i = 0; sizeof(thirdCheck) / sizeof(float); i++) {
		if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, thirdCheck[i], &coords.z, false)) {
			return;
		}
	}
}

void update_features()
{
	// wait until player is ready, basicly to prevent using the trainer while player is dead or arrested
	while (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) || PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), TRUE))
		WAIT(0);

	// common variables
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);

	// player invincible
	if (bPlayerExists)
		PLAYER::SET_PLAYER_INVINCIBLE(player, TRUE);
}

void main()
{
	// reset time
	TIME::SET_CLOCK_TIME(0, 0, 0);
	// pause time
	TIME::PAUSE_CLOCK(true);

	GAMEPLAY::SET_FADE_IN_AFTER_LOAD(false);
	Player player = PLAYER::PLAYER_ID();
	PLAYER::SET_PLAYER_INVINCIBLE(player, TRUE);
	// hide hud and radar
	// DOES NOT WORK
	//UI::DISPLAY_HUD(false);
	//UI::DISPLAY_RADAR(true);

	// for debug drawing coords
	bool widescreen = GRAPHICS::GET_IS_WIDESCREEN();

	while (true)
	{
		if (IsKeyJustUp(VK_NUMPAD1))
		{
			// set time test
			// advance time by 1 hour
			int h = TIME::GET_CLOCK_HOURS();
			h = (h + 1) % 24;
			int m = TIME::GET_CLOCK_MINUTES();
			TIME::SET_CLOCK_TIME(h, m, 0);
		}
		if (IsKeyJustUp(VK_NUMPAD2))
		{
			// teleport test
			Cam current_cam = CAM::GET_RENDERING_CAM();
			Vector3 rot = CAM::GET_CAM_ROT(current_cam, 2);

			CAM::DESTROY_ALL_CAMS(true);
			Cam cam = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", true);
			Vector3 coords;
			coords.x = GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(-1500.0, 2000.0);
			coords.y = GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(-2500.0, 5500.0);
			setZCoord(coords);
			CAM::SET_CAM_COORD(cam, coords.x, coords.y, coords.z);
			CAM::SET_CAM_ROT(cam, rot.x, rot.y, rot.z, 2);
			CAM::RENDER_SCRIPT_CAMS(true, // render camera
				false,	// smooth transition - false
				0,	// transition time
				false, // always 0 
				false // always 0
			);
		}
		if (IsKeyJustUp(VK_NUMPAD3))
		{
			// screenshot test
		}

		//update_features();

		// for debug drawing coords
		Cam current_cam = CAM::GET_RENDERING_CAM();
		Vector3 position = CAM::GET_CAM_COORD(current_cam);
		Vector3 rot = CAM::GET_CAM_ROT(current_cam, 2);
		char coords_text[200];
		sprintf_s(coords_text, "X: %.3f Y: %.3f Z: %.3f, rotX: %.3f rotY: %.3f rotZ: %.3f",
			position.x, position.y, position.z,
			rot.x, rot.y, rot.z);
		widescreen ? draw_text(coords_text, 0.978, 0.205 - 0.03, 0.3) : draw_text(coords_text, 0.978, 0.205, 0.3);

		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}
