#include "DllMain.h"
#include "Loot.hpp"
#include "MainRenderer.hpp"
#include "Camera.hpp"

extern const void* DetourFunc64(BYTE* const src, const BYTE* dest, const unsigned int jumplength);

void Main() {
	StartDebuggingIfApplicable(); // define GAY_DEBUGGING for debugging mode in [ C/C++ > Preprocessor -> Preprocessor Definitions ]
	dprintf(xMe("Main Started\n"));

	UE::GWorld = (UWorld**)((DWORD64)GetModuleHandleW(NULL) + OFFSET_UWORLD);

	UWorld* World = *UE::GWorld;

	dprintf(xMe("Uworld: 0x%X\n"), World);
	auto GameInstance = World->OwningGameInstance;
	dprintf(xMe("GameInstance: 0x%X\n"), GameInstance);
	auto LocalPlayer = GameInstance->LocalPlayers[0];
	dprintf(xMe("LocalPlayer: 0x%X\n"), LocalPlayer);
	auto ViewportClient = LocalPlayer->ViewportClient;

	PBYTE addr = (PBYTE)((DWORD64)GetModuleHandleW(NULL) + OFFSET_LINEOFSIGHTTO); //dissassemble result then it's the 'call'
	LOS = reinterpret_cast<decltype(LOS)>(addr);
	dprintf(xMe("LineOfSightTo: 0x%X\n"), LOS);

	if (!World || !LocalPlayer || !ViewportClient)
	{
		dprintf(xMe("World/LocalPlayer/ViewportClient are invalid\n"));
		return;
	}

	dprintf(xMe("ViewportClient: %s\n"), (UObject*)ViewportClient->GetFullName().c_str());
	auto vTable = *(void***)(ViewportClient);
	origin_renderer = (PostRender_t)vTable[100];
	SwapVtable((void*)ViewportClient, 100, MainRenderer);

	// hook cam
	auto cameraHook = (PBYTE)((DWORD64)GetModuleHandleW(NULL) + OFFSET_CAMERAHOOK);
	camMainOrigin = reinterpret_cast<decltype(camMainOrigin)>(DetourFunc64((BYTE*)cameraHook, (BYTE*)CameraMain, 19));
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
	if (dwReason == 1) Main();

	return true;
}