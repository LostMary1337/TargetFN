#pragma once

void LootESP(UCanvas* pCanvas) {
	float distance;
	TArray<AActor*> chests, loot, drops, quests;
	FVector CamLoc;
	APlayerCameraManager* CamManager = UE::PlayerController->PlayerCameraManager;

	UE::GPStatics->STATIC_GetAllActorsOfClass(UE::World, (AActor*)ATiered_Chest_Athena_C::StaticClass(), &chests);
	UE::GPStatics->STATIC_GetAllActorsOfClass(UE::World, (AActor*)AFortPickupAthena::StaticClass(), &loot);
	UE::GPStatics->STATIC_GetAllActorsOfClass(UE::World, (AActor*)AFortAthenaSupplyDrop::StaticClass(), &drops);
	UE::GPStatics->STATIC_GetAllActorsOfClass(UE::World, (AActor*)AQuestVisual::StaticClass(), &quests);

	//ESP chest/loot
	if (CamManager)
	{
		CamLoc = CamManager->GetCameraLocation();
		if (Settings::supplyLlama) {
			//quests
			for (int i = 0; i < quests.Num(); i++)
			{
				AQuestVisual* Loot = (AQuestVisual*)quests[i];
				//if (Loot->GetName().find("lama") == std::string::npos) continue;

				FVector LootLoc = Loot->K2_GetActorLocation();
				FVector2D LootLoc2D;

				distance = CamLoc.Distance(LootLoc);
				distance = UE::MathLib->STATIC_Sqrt(distance) / 100;

				char charText[256];
				wchar_t wideText[256];
				sprintf(charText, xMe("[ XP Coin %d m ]"), (int)distance);
				AnsiToWide(charText, wideText);

				if (UE::PlayerController->ProjectWorldLocationToScreen(LootLoc, false, &LootLoc2D))
				{
					pCanvas->K2_DrawText(UE::Font, wideText, LootLoc2D,
						FVector2D(1.2f, 1.2f), FLinearColor(245.0f, 66.0f, 206.0f, 0.95f),
						0.0f, FLinearColor(), LootLoc2D, true, false, true, FLinearColor(0.0f, 0.0f, 0.0f, 0.95f));
				}
			}

			//llama
			for (int i = 0; i < drops.Num(); i++)
			{
				bool isLlama = true;
				AFortAthenaSupplyDrop* Loot = (AFortAthenaSupplyDrop*)drops[i];

				if (Loot->GetName().find(xMe("lama")) == std::string::npos) {
					isLlama = false;
				}

				FVector LootLoc = Loot->K2_GetActorLocation();
				FVector2D LootLoc2D;

				distance = CamLoc.Distance(LootLoc);
				distance = UE::MathLib->STATIC_Sqrt(distance) / 100;

				char charText[256];
				wchar_t wideText[256];
				if (!isLlama)
					sprintf(charText, xMe("[ Supply Drop %d m ]"), (int)distance);
				else
					sprintf(charText, xMe("[ Llama %d m ]"), (int)distance);

				AnsiToWide(charText, wideText);

				if (UE::PlayerController->ProjectWorldLocationToScreen(LootLoc, false, &LootLoc2D))
				{
					if (!isLlama)
						pCanvas->K2_DrawText(UE::Font, wideText, LootLoc2D,
							FVector2D(1.2f, 1.2f), FLinearColor(47.0f, 129.0f, 222.0f, 0.95f),
							0.0f, FLinearColor(), LootLoc2D, true, false, true, FLinearColor(0.0f, 0.0f, 0.0f, 0.95f));
					else
						pCanvas->K2_DrawText(UE::Font, wideText, LootLoc2D,
							FVector2D(1.2f, 1.2f), FLinearColor(26.0f, 237.0f, 163.0f, 0.95f),
							0.0f, FLinearColor(), LootLoc2D, true, false, true, FLinearColor(0.0f, 0.0f, 0.0f, 0.95f));
				}
			}
		}

		if (Settings::chests) {
			//chest
			for (int i = 0; i < chests.Num(); i++)
			{
				ATiered_Chest_Athena_C* Loot = (ATiered_Chest_Athena_C*)chests[i];

				if (!Loot || Loot->bAlreadySearched) continue;

				FVector LootLoc = Loot->K2_GetActorLocation();
				FVector2D LootLoc2D;
				wchar_t NameStr[30], distStr[10];

				distance = CamLoc.Distance(LootLoc);
				distance = UE::MathLib->STATIC_Sqrt(distance) / 100;
				if (distance > Settings::chestsAmmoDistance) continue;

				char charText[256];
				wchar_t wideText[256];
				sprintf(charText, xMe("[ Chest %d m ]"), (int)distance);
				AnsiToWide(charText, wideText);

				if (UE::PlayerController->ProjectWorldLocationToScreen(LootLoc, false, &LootLoc2D))
				{
					pCanvas->K2_DrawText(UE::Font, wideText, LootLoc2D,
						FVector2D(1.2f, 1.2f), FLinearColor(236.0f, 252.0f, 3.0f, 0.95f),
						0.0f, FLinearColor(), LootLoc2D, true, false, true, FLinearColor(0.0f, 0.0f, 0.0f, 0.95f));
				}
			}
		}

		if (Settings::loot) {
			//loot
			for (int i = 0; i < loot.Num(); i++)
			{
				AFortPickupAthena* Loot = (AFortPickupAthena*)loot[i];

				if (!Loot) continue;

				FVector LootLoc = Loot->K2_GetActorLocation();
				FVector2D LootLoc2D;
				wchar_t NameStr[100], distStr[10], * itemName = NULL;
				FString LootName;

				distance = CamLoc.Distance(LootLoc);
				distance = UE::MathLib->STATIC_Sqrt(distance) / 100;

				//get loot name
				if (Loot->PrimaryPickupItemEntry.ItemDefinition) itemName = Loot->PrimaryPickupItemEntry.ItemDefinition->DisplayName.GetText();

				auto isAmmo = itemName != NULL && wcsstr(itemName, xMe(L"Ammo: "));
				auto isBrick = itemName != NULL && wcsstr(itemName, xMe(L"Brick"));
				auto isWood = itemName != NULL && wcsstr(itemName, xMe(L"Wood"));
				auto isMetal = itemName != NULL && wcsstr(itemName, xMe(L"Metal"));
				auto isStone = itemName != NULL && wcsstr(itemName, xMe(L"Stone"));
				auto isGold = itemName != NULL && wcsstr(itemName, xMe(L"Gold"));
				auto isBounty = itemName != NULL && wcsstr(itemName, xMe(L"Bounty"));
				if (isBrick || isWood || isMetal || isStone || isGold || isBounty) continue;

				if (!Settings::ammo) {
					if (isAmmo) continue;
				}
				else {
					if (isAmmo) if (distance > Settings::chestsAmmoDistance) continue;
					else if (distance > Settings::lootDistance) continue;
				}

				if (distance > Settings::overallDistance) continue;

				if (Loot->PrimaryPickupItemEntry.ItemDefinition != NULL) {
					auto LootTier = Loot->PrimaryPickupItemEntry.ItemDefinition->Tier;
					if (LootTier < GetItemEFortTier(Settings::lootMinimalTier)) continue;

					auto LootColor = GetItemColor(LootTier);

					CHAR text[0xFF] = { 0 };
					if (itemName + (isAmmo ? 6 : 0) != 0) wcstombs(text, itemName + (isAmmo ? 6 : 0), sizeof(text));

					char charText[256];
					wchar_t wideText[256];
					sprintf(charText, xMe("[ %s %d m ]"), text, (int)distance);
					AnsiToWide(charText, wideText);

					if (UE::PlayerController->ProjectWorldLocationToScreen(LootLoc, false, &LootLoc2D))
					{
						pCanvas->K2_DrawText(UE::Font, wideText, LootLoc2D,
							FVector2D(1.2f, 1.2f), LootColor,
							0.0f, FLinearColor(), LootLoc2D, true, false, true, FLinearColor(0.0f, 0.0f, 0.0f, 0.95f));
					}
				}
			}
		}
	}
}


void WeakSpot(UCanvas* pCanvas) {
	TArray<AActor*> weakSpots;
	APlayerCameraManager* CamManager = UE::PlayerController->PlayerCameraManager;

	UE::GPStatics->STATIC_GetAllActorsOfClass(UE::World, (AActor*)ABuildingWeakSpot::StaticClass(), &weakSpots);

	if (CamManager)
	{
		//weakSpots
		for (int i = 0; i < weakSpots.Num(); i++)
		{
			ABuildingWeakSpot* Loot = (ABuildingWeakSpot*)weakSpots[i];
			if (!Loot->bActive) continue;

			FVector LootLoc = Loot->K2_GetActorLocation();

			FVector2D LootLoc2D;

			char charText[256];
			wchar_t wideText[256];
			sprintf(charText, xMe("[ x ]"));
			AnsiToWide(charText, wideText);

			if (LootLoc.X != 0 && LootLoc.Y != 0 && LootLoc.Z != 0) {
				FKey RightMouseButton = { FName(FNAME_RIGHTMOUSEBUTTON) };
				if (!UE::PlayerController) return;

				if (UE::PlayerController->IsInputKeyDown(RightMouseButton)) {
					auto camloc = CamManager->GetCameraLocation();
					if (camloc.X == 0 || camloc.Y == 0 || camloc.Z == 0) return;
					auto camrot = CamManager->GetCameraRotation();
					if (camrot.Pitch == 0 || camrot.Yaw == 0 || camrot.Roll == 0) return;

					auto AimRotation = UE::MathLib->STATIC_FindLookAtRotation(camloc, LootLoc);
					if (AimRotation.Pitch == 0 || AimRotation.Yaw == 0) return;

					if (!UE::PlayerController->InputPitchScale) return;
					if (!UE::PlayerController->InputYawScale) return;

					AimRotation.Pitch = (AimRotation.Pitch - camrot.Pitch) / UE::PlayerController->InputPitchScale;
					AimRotation.Yaw = (AimRotation.Yaw - camrot.Yaw) / UE::PlayerController->InputYawScale;
					if (AimRotation.Pitch == 0 || AimRotation.Yaw == 0) return;
					if (!UE::PlayerController) return;

					UE::PlayerController->AddPitchInput(AimRotation.Pitch);
					UE::PlayerController->AddYawInput(AimRotation.Yaw);
				}
			}

			if (UE::PlayerController->ProjectWorldLocationToScreen(LootLoc, false, &LootLoc2D))
			{
				pCanvas->K2_DrawText(UE::Font, wideText, LootLoc2D,
					FVector2D(1.2f, 1.2f), FLinearColor(0.0f, 255.0f, 0.0f, 0.95f),
					0.0f, FLinearColor(), LootLoc2D, true, false, true, FLinearColor(0.0f, 0.0f, 0.0f, 0.95f));
			}
		}
	}
}