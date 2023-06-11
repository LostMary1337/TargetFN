#pragma once

BOOL(*LOS)(APlayerController* controller, AFortPlayerPawn* pawn, FVector* vp, bool alt) = nullptr;
//void AMB(APlayerCameraManager* MyCamera, AFortPlayerPawn* MyPawn, AFortPlayerPawn* Target);
PostRender_t origin_renderer;
AFortPlayerPawn* Target = NULL;
FString MyName;
bool doneName;

BOOLEAN LineOfSightTo(APlayerController* controller, AFortPlayerPawn* pawn, FVector* vp) {
	return LOS(controller, pawn, vp, false);
}

void MainLoop(UCanvas* pCanvas)
{
	if (Target) Target = NULL;

	if (!UE::SetObjects()) return;

	int width, height;
	UE::PlayerController->GetViewportSize(&width, &height);

	auto AllPlayers = UE::FortKismetLib->STATIC_GetFortPlayerPawns(UE::World);
	auto MyPlayer = (AFortPlayerPawn*)UE::PlayerController->K2_GetPawn();
	auto CamManager = UE::PlayerController->PlayerCameraManager;
	auto MyCameraPosition = CamManager->GetCameraLocation();
	auto MyCameraRotation = CamManager->GetCameraRotation();
	AFortPlayerPawn* PotTarget = NULL;

	if (Settings::visuals && Settings::allLoot) LootESP(pCanvas);
	//if (Settings::aimWeakspot) WeakSpot(pCanvas);

	float closestDistance = FLT_MAX;
	float closestEnemyDist = FLT_MAX;
	if (!doneName) {
		MyName = UE::FortKismetLib->STATIC_GetPlayerNameSafe(MyPlayer);
		doneName = true;
	}

	for (int i = 0; i < AllPlayers.Num(); i++)
	{
		AFortPlayerPawn* Enemy = (AFortPlayerPawn*)AllPlayers[i];
		FString EnemyName;
		if (Settings::drawSelf) {
			if (MyPlayer != Enemy && UE::FortKismetLib->STATIC_OnSameTeam(MyPlayer, Enemy)) continue;
		}
		else {
			if (UE::FortKismetLib->STATIC_OnSameTeam(MyPlayer, Enemy)) continue;
		}

		if (!Enemy) continue;
		if (Enemy->IsDead()) continue;

		FVector viewPoint = { 0, 0, 0 };
		bool visible = LineOfSightTo(UE::PlayerController, Enemy, &viewPoint);
		bool aimVisible = visible;
		if (!Settings::visCheck) visible = true;

		auto allSockets = Enemy->Mesh->GetAllSocketNames();
		auto HeadSocketID = GetSocketIDFromBoneID(Enemy, 66);
		auto RootSocketID = GetSocketIDFromBoneID(Enemy, 0);
		FVector HeadSocketLocation = Enemy->Mesh->GetSocketLocation(allSockets[HeadSocketID]);
		FVector RootSocketLocation = Enemy->Mesh->GetSocketLocation(allSockets[RootSocketID]);
		HeadSocketLocation.Z += 15;

		auto distance = UE::MathLib->STATIC_Sqrt(MyCameraPosition.Distance(HeadSocketLocation)) / 100;
		if ((int)distance > Settings::overallDistance) continue;

		FVector2D Head2D, Root2D;
		bool OnScreen = UE::PlayerController->ProjectWorldLocationToScreen(HeadSocketLocation, false, &Head2D) &&
			UE::PlayerController->ProjectWorldLocationToScreen(RootSocketLocation, false, &Root2D);

		if (distance < closestEnemyDist) closestEnemyDist = distance;

		if (OnScreen)
		{
			if (Settings::visuals) {
				float heightA = UE::MathLib->STATIC_Abs(Head2D.Y - Root2D.Y);
				float widthA = heightA * 0.65f;
				auto Size = FVector2D(widthA, heightA);
				auto Pos = FVector2D(Head2D.X - widthA / 2, Head2D.Y);
				auto BoxLineColor = visible ? FLinearColor(34.0f, 217.0f, 28.0f, 1.0f) : FLinearColor(247.0f, 38.0f, 10.0f, 1.0f);
				if (Enemy->bIsDBNO) BoxLineColor = FLinearColor(243.0f, 228.0f, 173.0f, 1.0f);

				int TeamIndex = 1;
				if (Enemy->PlayerState) {
					TeamIndex = ((AFortPlayerStateAthena*)Enemy->PlayerState)->TeamIndex;
					if (Settings::playerName) {
						EnemyName = UE::FortKismetLib->STATIC_GetPlayerNameSafe(Enemy);
						string str2 = EnemyName.ToString().substr(0, 13);
						char charText[256];
						wchar_t wideText[256];
						sprintf(charText, xMe("%s"), str2);
						AnsiToWide(charText, wideText);
						EnemyName = wideText;
					}
					else {
						char charText[256];
						wchar_t wideText[256];
						sprintf(charText, xMe("NONAME"));
						AnsiToWide(charText, wideText);
						EnemyName = wideText;
					}
				}
				else EnemyName = MyName;

				auto centerTop = FVector2D(Pos.X + (Size.X / 2), Pos.Y);

				if (Settings::snaplines) {
					pCanvas->K2_DrawLine(Root2D, FVector2D(width / 2, height - 20), Settings::snaplinePower, FLinearColor(255.f, 255.f, 255.f, 1.f));
				}

				// Draw Target ESP according to the text above
				DrawTargetESP(pCanvas, TeamIndex, centerTop);

				char charText[256];
				wchar_t wideText[256];
				if (Settings::playerName && Settings::playerDistance) { // Player Name + Distance
					// Draw Distance ESP
					sprintf(charText, xMe("%dm"), (int)distance);
					AnsiToWide(charText, wideText);
					auto textLocation = FVector2D(centerTop.X - 47 - 36 + 15 + 1, centerTop.Y - 40 + 4 + 1);
					pCanvas->K2_DrawText(UE::Font, wideText, FVector2D(textLocation.X, textLocation.Y), FVector2D(1.0f, 1.0f), FLinearColor(255, 255, 255, 1), 1.0f, FLinearColor(), FVector2D(), false, false, false, FLinearColor());

					// Draw Name ESP
					auto textLocationX = FVector2D(centerTop.X - 47 - 36 + 15 + 1 + 37, textLocation.Y);
					pCanvas->K2_DrawText(UE::Font, EnemyName, FVector2D(textLocationX.X, textLocationX.Y), FVector2D(1.0f, 1.0f), FLinearColor(255, 255, 255, 1), 1.0f, FLinearColor(), FVector2D(), false, false, false, FLinearColor());
				}
				else if (Settings::playerName && !Settings::playerDistance) { // Player Name only
					// Draw Name ESP
					auto textLocationX = FVector2D(centerTop.X - 47 - 36 + 15 + 1, centerTop.Y - 40 + 4 + 1);
					pCanvas->K2_DrawText(UE::Font, EnemyName, FVector2D(textLocationX.X, textLocationX.Y), FVector2D(1.0f, 1.0f), FLinearColor(255, 255, 255, 1), 1.0f, FLinearColor(), FVector2D(), false, false, false, FLinearColor());
				}
				else if (!Settings::playerName && Settings::playerDistance) { // Distance only
					// Draw Distance ESP
					sprintf(charText, xMe("%dm"), (int)distance);
					AnsiToWide(charText, wideText);
					auto textLocation = FVector2D(centerTop.X - 10 - 21.3 + 12 + 1, centerTop.Y - 40 + 4 + 1);
					pCanvas->K2_DrawText(UE::Font, wideText, FVector2D(textLocation.X, textLocation.Y), FVector2D(1.0f, 1.0f), FLinearColor(255, 255, 255, 1), 1.0f, FLinearColor(), FVector2D(), false, false, false, FLinearColor());
				}

				// Draw Box ESP
				if (Settings::playerBox && Settings::playerCornerBox) {
					//ImGui::GetOverlayDrawList()->AddRect(topLeft, bottomRight, boxColor, 0.5, 15, 1.5f);
					//pCanvas->K2_DrawBox(Pos, Size, 4.0f, BoxLineColor);

					auto topLeft = Pos;
					auto bottomRight = Pos + Size;

					auto theAdditionY = (bottomRight.Y - topLeft.Y) / (Settings::cornerboxScale+1);
					auto theAdditionX = (bottomRight.X - topLeft.X) / Settings::cornerboxScale;

					auto topRight = FVector2D(bottomRight.X, topLeft.Y);

					auto bottomLeft = FVector2D(topLeft.X, bottomRight.Y);

					pCanvas->K2_DrawLine(topLeft, FVector2D(topLeft.X, topLeft.Y + theAdditionY), Settings::playerBoxThickness, BoxLineColor); // Top Left Y
					pCanvas->K2_DrawLine(topLeft, FVector2D(topLeft.X + theAdditionX, topLeft.Y), Settings::playerBoxThickness, BoxLineColor); // Top Left X

					pCanvas->K2_DrawLine(topRight, FVector2D(topRight.X, topRight.Y + theAdditionY), Settings::playerBoxThickness, BoxLineColor); // Top Right Y
					pCanvas->K2_DrawLine(topRight, FVector2D(topRight.X - theAdditionX, topRight.Y), Settings::playerBoxThickness, BoxLineColor); // Top Right X

					pCanvas->K2_DrawLine(bottomLeft, FVector2D(bottomLeft.X, bottomLeft.Y - theAdditionY), Settings::playerBoxThickness, BoxLineColor); // Bottom Left Y
					pCanvas->K2_DrawLine(bottomLeft, FVector2D(bottomLeft.X + theAdditionX, bottomLeft.Y), Settings::playerBoxThickness, BoxLineColor); // Bottom Left X

					pCanvas->K2_DrawLine(bottomRight, FVector2D(bottomRight.X, bottomRight.Y - theAdditionY), Settings::playerBoxThickness, BoxLineColor); // Bottom Right Y
					pCanvas->K2_DrawLine(bottomRight, FVector2D(bottomRight.X - theAdditionX, bottomRight.Y), Settings::playerBoxThickness, BoxLineColor); // Bottom Right X
				}
				else if (Settings::playerBox) {
					// The doodoo ugly box esp
					pCanvas->K2_DrawBox(Pos, Size, Settings::playerBoxThickness, BoxLineColor);
				}

				// Draw Weapon ESP
				if (Settings::playerWeapon) {
					auto curWeapon = Enemy->CurrentWeapon;
					if (curWeapon != nullptr) {
						auto itemDef = curWeapon->WeaponData;
						if (itemDef != nullptr) {
							auto weaponName = itemDef->DisplayName;
							if (!weaponName.GetText()) continue;

							bool isReloading = curWeapon->bIsReloadingWeapon;

							auto topLeft = Pos;
							auto bottomRight = Pos + Size;

							if (isReloading) {
								char tier_data[12];
								wchar_t tier_data_wide[12];
								sprintf(tier_data, xMe("Reloading.."));
								AnsiToWide(tier_data, tier_data_wide);
								auto size = pCanvas->K2_TextSize(UE::Font, tier_data_wide, { 1.f, 1.f });
								pCanvas->K2_DrawText(UE::Font, tier_data_wide, FVector2D(((topLeft.X + bottomRight.X) / 2.0f) - size.X / 2.0f, bottomRight.Y + 6), FVector2D(1.0f, 1.0f), FLinearColor(0, 200.f, 0, 1.f), 1.0f, FLinearColor(0, 0, 0, 255), FVector2D(), false, false, true, FLinearColor(0, 0, 0, 1.0f));
							}
							else {
								auto size = pCanvas->K2_TextSize(UE::Font, weaponName.GetText(), { 1.f, 1.f });
								pCanvas->K2_DrawText(UE::Font, weaponName.GetText(), FVector2D(((topLeft.X + bottomRight.X) / 2.0f) - size.X / 2.0f, bottomRight.Y + 6), FVector2D(1.0f, 1.0f), GetItemColor(itemDef->Tier), 1.0f, FLinearColor(0, 0, 0, 255), FVector2D(), false, false, true, FLinearColor(0, 0, 0, 1.0f));
							}
						}
					}
				}
			}
		}

		if (MyPlayer && MyPlayer != Enemy) { // (MyPlayer != Enemy) is for incase drawSelf is enabled, we don't wanna aimbot at self.
			auto dx = Head2D.X - (width / 2);
			auto dy = Head2D.Y - (height / 2);
			auto dist = UE::MathLib->STATIC_Sqrt(dx * dx + dy * dy);

			if (!Settings::aimVisCheck) aimVisible = true;

			if (!MyPlayer->IsSkydiving() && !MyPlayer->IsParachuteOpen() && aimVisible)
			{
				if (dist < (Settings::FOV * 10) && dist < closestDistance) {
					closestDistance = dist;
					//check if player is knocked out
					if (Enemy->bIsDBNO)
						PotTarget = Enemy;
					else
						Target = Enemy;
				}
			}
		}
	}

	if (!Target && PotTarget) Target = PotTarget;
	//if (Target && MyPlayer && CamManager)
		//AMB(CamManager, MyPlayer, Target);

	if (Settings::visuals && Settings::infoText) {
		// Item rarity
		char tier_data[256];
		wchar_t tier_data_wide[256];
		auto ItemTierName = GetItemTierName(GetItemEFortTier(Settings::lootMinimalTier));
		sprintf(tier_data, xMe("Minimum Item Rarity: %s [Page Up/Down]"), ItemTierName);
		AnsiToWide(tier_data, tier_data_wide);
		pCanvas->K2_DrawText(UE::Font, tier_data_wide, { 30, 220 }, FVector2D(1.0f, 1.0f), GetItemColor(GetItemEFortTier(Settings::lootMinimalTier)), 1.0f, FLinearColor(0, 0, 0, 1), FVector2D(), false, false, true, FLinearColor(0, 0, 0, 1.0f));

		// Overall Render distance
		sprintf(tier_data, xMe("Overall render distance: %d m"), (int)Settings::overallDistance);
		AnsiToWide(tier_data, tier_data_wide);
		pCanvas->K2_DrawText(UE::Font, tier_data_wide, { 30, 240 }, FVector2D(1.0f, 1.0f), FLinearColor(255.f, 255.f, 255.f, 1.f), 1.0f, FLinearColor(0, 0, 0, 1), FVector2D(), false, false, true, FLinearColor(0, 0, 0, 1.0f));

		// Loot Render distance
		sprintf(tier_data, xMe("Loot render distance: %d m"), (int)Settings::lootDistance);
		AnsiToWide(tier_data, tier_data_wide);
		pCanvas->K2_DrawText(UE::Font, tier_data_wide, { 30, 260 }, FVector2D(1.0f, 1.0f), FLinearColor(255.f, 255.f, 255.f, 1.f), 1.0f, FLinearColor(0, 0, 0, 1), FVector2D(), false, false, true, FLinearColor(0, 0, 0, 1.0f));

		if (closestEnemyDist < 500 && Settings::showClosestDistance) {
			sprintf(tier_data, xMe("CLOSEST ENEMY: %d m"), (int)closestEnemyDist);
			AnsiToWide(tier_data, tier_data_wide);
			pCanvas->K2_DrawText(UE::Font, tier_data_wide, { 30, (float)(height / 2) }, FVector2D(1.0f, 1.0f), closestEnemyDist < 50 ? FLinearColor(200, 0, 0, 1) : FLinearColor(255, 255, 255, 1), 1.0f, FLinearColor(0, 0, 0, 1), FVector2D(), false, false, true, FLinearColor(0, 0, 0, 1.0f));
		}
	}

	if (Settings::customCrosshair) {
		pCanvas->K2_DrawLine(FVector2D(width / 2 - 12, height / 2), FVector2D(width / 2 - 2, height / 2), 2.0f, FLinearColor(0.0f, 0.0f, 0.0f, 255.0f));
		pCanvas->K2_DrawLine(FVector2D(width / 2 + 13, height / 2), FVector2D(width / 2 + 3, height / 2), 2.0f, FLinearColor(0.0f, 0.0f, 0.0f, 255.0f));
		pCanvas->K2_DrawLine(FVector2D(width / 2, height / 2 - 12), FVector2D(width / 2, height / 2 - 2), 2.0f, FLinearColor(0.0f, 0.0f, 0.0f, 255.0f));
		pCanvas->K2_DrawLine(FVector2D(width / 2, height / 2 + 13), FVector2D(width / 2, height / 2 + 3), 2.0f, FLinearColor(0.0f, 0.0f, 0.0f, 255.0f));
	}

	if (Settings::drawFOV) DrawCircle(pCanvas, width / 2, height / 2, Settings::FOV * 10, 65, FLinearColor(0.0f, 0.0f, 0.0f, 255.0f));

	if (Settings::ShowSexyMenu) DrawMenu(pCanvas, UE::PlayerController, UE::MathLib);
}

void MainRenderer(UObject* pObject, UCanvas* pCanvas)
{
	if (pCanvas) MainLoop(pCanvas);

	return origin_renderer(pObject, pCanvas);
}
/*
void AMB(APlayerCameraManager* MyCamera, AFortPlayerPawn* MyPawn, AFortPlayerPawn* Target)
{
	FKey RightMouseButton = { FName(FNAME_RIGHTMOUSEBUTTON) };
	if (!UE::PlayerController->IsInputKeyDown(RightMouseButton)) return;
	if (!Settings::aimbot) return;
	if (Settings::silent) return;

	AFortWeapon* MyWeapon = MyPawn->CurrentWeapon;
	if (!MyWeapon) return;
	if (MyWeapon->GetWeaponDataItemType() != EFortItemType::WeaponRanged) return;

	//set locations
	FVector CamLocation = MyCamera->GetCameraLocation();
	if (CamLocation.X == 0 || CamLocation.Y == 0 || CamLocation.Z == 0) return;
	FRotator CamRotation = MyCamera->GetCameraRotation();
	if (CamRotation.Pitch == 0 || CamRotation.Yaw == 0 || CamRotation.Roll == 0) return;

	int HeadSocketID = 0;
	if (Settings::aimBody)
		HeadSocketID = GetSocketIDFromBoneID(Target, 5);
	else
		HeadSocketID = GetSocketIDFromBoneID(Target, 66);

	FVector AimLocation = Target->Mesh->GetSocketLocation(Target->Mesh->GetAllSocketNames()[HeadSocketID]);
	if (AimLocation.X == 0 || AimLocation.Y == 0 || AimLocation.Z == 0) return;

	FRotator AimRotation = UE::MathLib->STATIC_FindLookAtRotation(CamLocation, AimLocation);
	if (AimRotation.Pitch == 0 || AimRotation.Yaw == 0 || AimRotation.Roll == 0) return;

	//AimRotation = SmoothMe(CamRotation, AimRotation, 2.0f);
	//set aim
	AimRotation.Pitch = (AimRotation.Pitch - CamRotation.Pitch) / UE::PlayerController->InputPitchScale;
	AimRotation.Yaw = (AimRotation.Yaw - CamRotation.Yaw) / UE::PlayerController->InputYawScale;
	if (AimRotation.Pitch == 0 || AimRotation.Yaw == 0) return;
	if (!UE::PlayerController) return;

	UE::PlayerController->AddPitchInput(AimRotation.Pitch);
	UE::PlayerController->AddYawInput(AimRotation.Yaw);
}
*/