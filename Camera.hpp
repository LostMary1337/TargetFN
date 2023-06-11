#pragma once

bool(*camMainOrigin)(APlayerController* Controller, FVector* Location, FRotator* Rotation) = nullptr;

int CameraMain(APlayerController* Controller, FVector* Location, FRotator* Rotation) {
	auto ret = camMainOrigin(Controller, Location, Rotation);

	if (!UE::PlayerController) return ret;
	//if (!Settings::aimbot) return ret;
	if (!Settings::silent) return ret;
	if (!Target) return ret;
	if (!UE::PlayerController->PlayerCameraManager) return ret;
	auto MyPlayer = (AFortPlayerPawn*)UE::PlayerController->K2_GetPawn();
	if (!MyPlayer) return ret;

	int HeadSocketID = 0;
	if (Settings::aimBody)
		HeadSocketID = GetSocketIDFromBoneID(Target, 5);
	else
		HeadSocketID = GetSocketIDFromBoneID(Target, 66);

	FVector AimLocation = Target->Mesh->GetSocketLocation(Target->Mesh->GetAllSocketNames()[HeadSocketID]);
	FRotator SilentAimAngle = UE::MathLib->STATIC_FindLookAtRotation(*Location, AimLocation);
	static float last_fire_ability_time = 0.0f;

	auto LocalWeapon = MyPlayer->CurrentWeapon;

	if (!LocalWeapon) return ret;

	if (LocalWeapon->LastFireAbilityTime != last_fire_ability_time)
	{
		if (SilentAimAngle.Pitch != 0 || SilentAimAngle.Yaw != 0)
			*Rotation = SilentAimAngle;

		last_fire_ability_time = LocalWeapon->LastFireAbilityTime;
	}

	return ret;
}