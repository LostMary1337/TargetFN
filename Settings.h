#pragma once
#include "SDK.hpp"

namespace Settings {
	// Main
	extern bool ShowSexyMenu;

	// AIMBOT TAB
	//extern bool aimbot;
	extern bool silent;
	extern bool aimWeakspot;
	extern bool aimBody;
	extern bool aimVisCheck;
	extern float FOV;
	extern float smooth;

	// VISUALS TAB
	extern bool visuals;
	extern bool allLoot;
	extern bool supplyLlama;
	extern bool loot;
	extern bool vehicles;
	extern bool chests;
	extern bool ammo;
	extern bool playerWeapon;
	extern bool playerName;
	extern bool playerDistance;
	extern bool playerBox;
	extern bool playerCornerBox;
	extern bool visCheck;
	extern bool snaplines;
	extern bool dashingSnaplines;
	extern bool drawSelf;

	// VISUALS TAB - SIDE
	extern float lootDistance;
	extern float chestsAmmoDistance;
	extern float overallDistance;
	extern float lootMinimalTier;
	extern float snaplinePower;
	extern float cornerboxScale;
	extern float playerBoxThickness;

	// MISC TAB
	extern bool devOutput;
	extern bool infoText;
	extern bool customCrosshair;
	extern bool drawFOV;
	extern bool showClosestDistance;

	// DEV params
	extern int devFloat;
}