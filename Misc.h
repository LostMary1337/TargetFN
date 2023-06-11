#pragma once
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include "SDK.hpp"
#include "StringC.h"

#ifdef GAY_DEBUGGING
#define dprintf printf
#else
#define dprintf(x)
#endif

void StartDebuggingIfApplicable() {
#ifdef GAY_DEBUGGING
	AllocConsole();
	FILE* doodoo;
	freopen_s(&doodoo, xMe("CONOUT$"), xMe("w"), stdout);
#endif
}

uint32_t GetVtableSize(void* object) {
	auto vtable = *(void***)(object);
	int i = 0;

	for (; vtable[i]; i++)
		__noop();

	return i;
}

void SwapVtable(void* obj, uint32_t index, void* hook) {
	auto currVt = *(void**)(obj);
	auto size = GetVtableSize(obj);
	dprintf(xMe("VTable has %d functions\n"), size);
	auto newVt = new uintptr_t[size];
	memcpy(newVt, currVt, size * 0x8);
	newVt[index] = (uintptr_t)hook;
	*(uintptr_t**)(obj) = newVt;
}

int GetSocketIDFromBoneID(AFortPlayerPawn* pawn, int boneId) {
	auto allSockets = pawn->Mesh->GetAllSocketNames(); //get all socket names

	FName boneName = pawn->Mesh->GetBoneName(boneId);

	for (int i = 0; i < allSockets.Num(); i++)
	{
		FName socketName = allSockets[i];
		FName socketBoneName = pawn->Mesh->GetSocketBoneName(socketName);
		auto allBones = pawn->Mesh->GetNumBones();
		if (socketBoneName.GetName() == boneName.GetName() || socketBoneName == boneName) {
			return i;
		}
	}

	return 0;
}