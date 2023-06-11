#pragma once
#include "SDK.hpp"

namespace UE
{
	static UWorld** GWorld = NULL;
	static UWorld* World = NULL;
	static APlayerController* PlayerController = NULL;
	static ULocalPlayer* LocalPlayer = NULL;
	static UKismetMathLibrary* MathLib = NULL;
	static UKismetSystemLibrary* KismetSystemLib = NULL;
	static UFortKismetLibrary* FortKismetLib = NULL;
	static UGameplayStatics* GPStatics = NULL;
	static UFont* Font = NULL;
	static FKey Insert = { FName(FNAME_INSERT) };
	static FKey PageUp = { FName(FNAME_PAGEUP) };
	static FKey PageDown = { FName(FNAME_PAGEDOWN) };
	static FKey F1 = { FName(FNAME_F1) };
	static FKey F2 = { FName(FNAME_F2) };
	//static UFortPlayerInputSettings* inputSettings = NULL;

	static bool TickyTicky() {
		if (PlayerController->WasInputKeyJustReleased(Insert)) {
			Settings::ShowSexyMenu = !Settings::ShowSexyMenu;
			if (Settings::ShowSexyMenu) {
				PlayerController->SetIgnoreLookInput(true);
				PlayerController->SetIgnoreMoveInput(true);
			}
			else {
				PlayerController->ResetIgnoreLookInput();
				PlayerController->ResetIgnoreMoveInput();
			}
		}
		else if (PlayerController->WasInputKeyJustReleased(PageUp)) {
			if (Settings::lootMinimalTier != 7)
				Settings::lootMinimalTier++;
		}
		else if (PlayerController->WasInputKeyJustReleased(PageDown)) {
			if (Settings::lootMinimalTier != 1)
				Settings::lootMinimalTier--;
		}
		else if (PlayerController->WasInputKeyJustReleased(F1)) {
			//Settings::aimbot = !Settings::aimbot;
			Settings::silent = !Settings::silent;
		}
		else if (PlayerController->WasInputKeyJustReleased(F2)) {
			Settings::visuals = !Settings::visuals;
		}

		return true;
	}


	static bool SetObjects()
	{
		//set Font
		if (!Font) Font = UObject::FindObject<UFont>(xMe("Font Roboto.Roboto"));

		//set MathLib for math functions
		if (!MathLib) MathLib = (UKismetMathLibrary*)UKismetMathLibrary::StaticClass();

		//set SystemLib for math functions
		if (!KismetSystemLib) KismetSystemLib = (UKismetSystemLibrary*)UKismetSystemLibrary::StaticClass();

		//Set FortKismetLib for game functions
		if (!FortKismetLib) FortKismetLib = (UFortKismetLibrary*)UFortKismetLibrary::StaticClass();
		if (!FortKismetLib) return false;

		//Set GPStatics for game functions
		if (!GPStatics) GPStatics = (UGameplayStatics*)UGameplayStatics::StaticClass();
		if (!GPStatics) return false;

		////Set InputSettings for game functions
		//if (!inputSettings) inputSettings = (UFortPlayerInputSettings*)UFortPlayerInputSettings::StaticClass();
		//if (!inputSettings) return false;

		//set world
		World = *GWorld;

		//check world
		if (!World) return false;

		//Set local player 
		if (World->OwningGameInstance && World->OwningGameInstance->LocalPlayers.IsValidIndex(0)) LocalPlayer = World->OwningGameInstance->LocalPlayers[0];
		if (!LocalPlayer) return false;

		//Set player contoller
		if (LocalPlayer) PlayerController = LocalPlayer->PlayerController;
		if (!PlayerController) return false;

		if (!TickyTicky()) return false;

		//Everything went well
		return true;
	}
};

