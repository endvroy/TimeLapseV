/*
	NUM0				activate/resume
	NUM1				advance time test
	NUM2 				teleport camera test
	NUM3			 	screenshot test
	NUM9			 	pause
*/

#include "ScreenCapturer.h"
#include "PathManager.h"
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
			coords.z += GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(3.0, 25.0);
			break;
		}
	}
	// if ground not found then set Z in air
	if (!groundFound) {
		coords.z = 300.0;
	}
}

void tpCamera() {
	CAM::DESTROY_ALL_CAMS(true);
	Cam cam = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", true);
	Vector3 coords;
	coords.x = GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(-1500.0, 2000.0);
	coords.y = GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(-2500.0, 5500.0);
	setZCoord(coords);
	Vector3 rot;
	rot.x = GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(-30.0, 30.0);
	rot.y = 0.0;
	rot.z = GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(-178.0, 178.0);
	CAM::SET_CAM_COORD(cam, coords.x, coords.y, coords.z);
	CAM::SET_CAM_ROT(cam, rot.x, rot.y, rot.z, 2);
	CAM::RENDER_SCRIPT_CAMS(true, // render camera
		false,	// smooth transition - false
		0,	// transition time
		false, // always 0 
		false // always 0
	);
}

Vector3 getCamCoord() {
	Cam current_cam = CAM::GET_RENDERING_CAM();
	return CAM::GET_CAM_COORD(current_cam);
}

Vector3 getCamRot() {
	Cam current_cam = CAM::GET_RENDERING_CAM();
	return CAM::GET_CAM_ROT(current_cam, 2);
}

void drawCamCoord() {
	// for debug drawing coords
	bool widescreen = GRAPHICS::GET_IS_WIDESCREEN();
	Vector3 position = getCamCoord();
	Vector3 rot = getCamRot();
	char coords_text[200];
	sprintf_s(coords_text, "X: %.3f Y: %.3f Z: %.3f, rotX: %.3f rotY: %.3f rotZ: %.3f",
		position.x, position.y, position.z,
		rot.x, rot.y, rot.z);
	widescreen ? draw_text(coords_text, 0.978, 0.205 - 0.03, 0.3) : draw_text(coords_text, 0.978, 0.205, 0.3);
}

void drawGameplayCamCoord() {
	// for figuring out reasonable coords
	bool widescreen = GRAPHICS::GET_IS_WIDESCREEN();
	Vector3 position = CAM::GET_GAMEPLAY_CAM_COORD();
	Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(0);
	char coords_text[200];
	sprintf_s(coords_text, "X: %.3f Y: %.3f Z: %.3f, rotX: %.3f rotY: %.3f rotZ: %.3f",
		position.x, position.y, position.z,
		rot.x, rot.y, rot.z);
	widescreen ? draw_text(coords_text, 0.978, 0.205 - 0.03, 0.3) : draw_text(coords_text, 0.978, 0.205, 0.3);
}

#define LOC_NUM 5

void main()
{
	// reset time
	TIME::SET_CLOCK_TIME(0, 0, 0);
	// pause time
	TIME::PAUSE_CLOCK(true);

	GAMEPLAY::SET_FADE_IN_AFTER_LOAD(true);

	ScreenCapturer screenCapturer;
	PathManager pathManager;
	pathManager.setBasePath("E:\\TimeLapseVData");
	pathManager.makeBaseDir();

	int loc = 0;
	bool pause = true;

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
			tpCamera();

		}
		if (IsKeyJustUp(VK_NUMPAD3))
		{
			// screenshot test
			screenCapturer.screenshot("E:\\screenshot.png");
		}
		if (IsKeyJustUp(VK_NUMPAD0))
		{
			// start capturing
			pause = false;
		}
		if (IsKeyJustUp(VK_NUMPAD9)) {
			pause = true;
		}
		if (pause == false) {
			// capturing logic
			if (loc < LOC_NUM) {
				tpCamera();
				pathManager.locIndex = loc;
				pathManager.makeLocDir();
				WAIT(3000); // wait for rendering to complete
				for (int hour = 0; hour <= 23; hour++) {
					TIME::SET_CLOCK_TIME(hour, 0, 0);
					pathManager.hour = hour;
					pathManager.getFilePath();
					WAIT(850);
					screenCapturer.screenshot(pathManager.pathBuf);
					if (IsKeyJustUp(VK_NUMPAD9)) {
						pause = true;
					}
				}
				loc++;
			}
		}

		//drawGameplayCamCoord();
		drawCamCoord();

		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}
