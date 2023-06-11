#include "Settings.h"
#include "StringC.h"

// Initialize Settings

namespace Settings {
	// Main
	bool ShowSexyMenu = true;

	// AIMBOT TAB
	bool aimbot = true;
	bool silent = true;
	bool aimWeakspot = false;
	bool aimBody = false;
	bool aimVisCheck = false;
	float FOV = 50.0f;
	float smooth = 0.5f;

	// VISUALS TAB
	bool visuals = true;
	bool allLoot = true;
	bool supplyLlama = true;
	bool loot = true;
	bool vehicles = true;
	bool chests = true;
	bool ammo = false;
	bool playerWeapon = true;
	bool playerName = true;
	bool playerDistance = true;
	bool playerBox = true;
	bool playerCornerBox = true;
	bool visCheck = true;
	bool snaplines = false;
	bool dashingSnaplines = false;
	bool drawSelf = false;

	// VISUALS TAB - SIDE
	float lootDistance = 300.f;
	float chestsAmmoDistance = 300.f;
	float overallDistance = 500.f;
	float lootMinimalTier = 2;
	float snaplinePower = 2.f;
	float cornerboxScale = 3.f;
	float playerBoxThickness = 4.f;

	// MISC TAB
	bool devOutput = false;
	bool infoText = true;
	bool customCrosshair = true;
	bool drawFOV = true;
	bool showClosestDistance = true;

	// DEV params
	int devFloat = 19;
}