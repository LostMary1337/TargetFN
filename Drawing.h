#pragma once
#include <vector>
#include "SDK.hpp"

FLinearColor GetItemColor(EFortItemTier tier)
{
	switch (tier)
	{
	case EFortItemTier::I:
		return FLinearColor{ 123, 123, 123, 0.95f };
	case EFortItemTier::II:
		return FLinearColor{ 58, 121, 19, 0.95f };
	case EFortItemTier::III:
		return FLinearColor{ 18, 88, 162, 0.95f };
	case EFortItemTier::IV:
		return FLinearColor{ 189, 63, 250, 0.95f };
	case EFortItemTier::V:
		return FLinearColor{ 255, 118, 5, 0.95f };
	case EFortItemTier::VI:
		return FLinearColor{ 220, 160, 30, 0.95f };
	case EFortItemTier::VII:
		return FLinearColor{ 0, 225, 252, 0.95f };
	default:
		return FLinearColor{ 123, 123, 123, 0.95f };
	}
}

EFortItemTier GetItemEFortTier(int tier)
{
	switch (tier)
	{
	case 1:
		return EFortItemTier::I;
	case 2:
		return EFortItemTier::II;
	case 3:
		return EFortItemTier::III;
	case 4:
		return EFortItemTier::IV;
	case 5:
		return EFortItemTier::V;
	case 6:
		return EFortItemTier::VI;
	case 7:
		return EFortItemTier::VII;
	default:
		return EFortItemTier::I;
	}
}

std::string GetItemTierName(EFortItemTier tier)
{
	switch (tier)
	{
	case EFortItemTier::I:
		return xMe("Common");
	case EFortItemTier::II:
		return xMe("Uncommon");
	case EFortItemTier::III:
		return xMe("Rare");
	case EFortItemTier::IV:
		return xMe("Epic");
	case EFortItemTier::V:
		return xMe("Legendary");
	case EFortItemTier::VI:
		return xMe("Mythic");
	case EFortItemTier::VII:
		return xMe("Exotic");
	default:
		return xMe("Common");
	}
}

void AnsiToWide(char* inAnsi, wchar_t* outWide)
{
	int i = 0;
	for (; inAnsi[i] != '\0'; i++)
		outWide[i] = (wchar_t)(inAnsi)[i];
	outWide[i] = L'\0';
}

void DrawFilledRect(UCanvas* pCanvas, FVector2D initial_pos, float width, float height, FLinearColor color)
{
	for (float i = 0.0f; i < height; i += 1.0f)
		pCanvas->K2_DrawLine(FVector2D(initial_pos.X, initial_pos.Y + i), FVector2D(initial_pos.X + width, initial_pos.Y + i), 1.0f, color);

}

void DrawTriangle(UCanvas* pCanvas, const FVector2D& point1, const FVector2D& point2, const FVector2D& point3, const FLinearColor& color, bool isFilled)
{
	/*static float devParam = 0;
	if (GetAsyncKeyState(VK_F1) & 1) {
		devParam += 1;
	}
	if (GetAsyncKeyState(VK_F2) & 1) {
		devParam -= 1;
	}*/
	//pCanvas->K2_DrawLine(FVector2D(224, 74), FVector2D(299, 74), 1.0f, color);
	pCanvas->K2_DrawLine(point1, point2, 1.0f, color);
	//float halfPositionX = point1.X + (DistanceBetween2Points(point1.X, point1.Y, point2.X, point2.Y) / 2);
	//pCanvas->K2_DrawLine(point1, FVector2D(halfPositionX, 150), 1.0f, color);
	pCanvas->K2_DrawLine(point1, point3, 1.0f, color);
	pCanvas->K2_DrawLine(point2, point3, 1.0f, color);
	// Fill it
	if (isFilled) {
		float addX = 0.5f;
		float addY = 1;
		for (int i = point1.Y; i < point3.Y; i++) {
			// draw line
			pCanvas->K2_DrawLine(FVector2D(point1.X + addX, point1.Y + addY), FVector2D(point2.X - addX, point2.Y + addY), 1.0f, color);
			addY++;
			addX += 0.5f;
		}
	}
}

void DrawCircle(UCanvas* pCanvas, int x, int y, int radius, int numsides, FLinearColor color)
{
	float Step = M_PI * 2.0 / numsides;
	int Count = 0;
	FVector2D V[128];
	for (float a = 0; a < M_PI * 2.0; a += Step)
	{
		float X1 = radius * UE::MathLib->STATIC_Cos(a) + x;
		float Y1 = radius * UE::MathLib->STATIC_Sin(a) + y;
		float X2 = radius * UE::MathLib->STATIC_Cos(a + Step) + x;
		float Y2 = radius * UE::MathLib->STATIC_Sin(a + Step) + y;
		V[Count].X = X1;
		V[Count].Y = Y1;
		V[Count + 1].X = X2;
		V[Count + 1].Y = Y2;
		pCanvas->K2_DrawLine(FVector2D({ V[Count].X, V[Count].Y }), FVector2D({ X2, Y2 }), 1.f, color);
	}
}

int colorOrder = 1;
float playersSavedColors[100][6]{};
void DrawTargetESP(UCanvas* pCanvas, int enemyTeamID, FVector2D& centerTop) {
	float metersR = 0.8471; float metersG = 0.3098; float metersB = 0.5451; float boxR = 0.8706; float boxG = 0.5137; float boxB = 0.6667;
	if (colorOrder == 1) { metersR = 0.3333; metersG = 0.3647; metersB = 0.6353; boxR = 0.5216, boxG = 0.5412; boxB = 0.7333; colorOrder = 2; }
	else if (colorOrder == 2) { metersR = 0.8471; metersG = 0.3098; metersB = 0.5451; boxR = 0.8706; boxG = 0.5137; boxB = 0.6667; colorOrder = 3; }
	else if (colorOrder == 3) { metersR = 0.3843; metersG = 0.5333; metersB = 0.5020; boxR = 0.5765; boxG = 0.6784; boxB = 0.6510; colorOrder = 4; }
	else if (colorOrder == 4) { boxR = 0.4784; boxG = 0.1922; boxB = 0.4157; metersR = 0.5922; metersG = 0.2588; metersB = 0.6392; colorOrder = 5; }
	else if (colorOrder == 5) { boxR = 0.0510; boxG = 0.4784; boxB = 0.4392; metersR = 0.3216; metersG = 0.6314; metersB = 0.6275; colorOrder = 6; }
	else if (colorOrder == 6) { boxR = 0.4745; boxG = 0.5843; boxB = 0.3373; metersR = 0.6314; metersG = 0.7333; metersB = 0.4431; colorOrder = 7; }
	else if (colorOrder == 7) { boxR = 0.2549; boxG = 0.3961; boxB = 0.2000; metersR = 0.4784; metersG = 0.6745; metersB = 0.4235; colorOrder = 8; }
	else if (colorOrder == 8) { boxR = 0.0000; boxG = 0.1333; boxB = 0.9216; metersR = 0.2784; metersG = 0.3804; metersB = 0.9843; colorOrder = 9; }
	else if (colorOrder == 9) { boxR = 0.5686; boxG = 0.2314; boxB = 0.1608; metersR = 0.8745; metersG = 0.3216; metersB = 0.2588; colorOrder = 10; }
	else if (colorOrder == 10) { boxR = 0.3020; boxG = 0.4902; boxB = 0.5255; metersR = 0.3216; metersG = 0.5608; metersB = 0.6118; colorOrder = 11; }
	else if (colorOrder == 11) { boxR = 0.1098; boxG = 0.4627; boxB = 0.6902; metersR = 0.3059; metersG = 0.5765; metersB = 0.8118; colorOrder = 12; }
	else if (colorOrder == 12) { boxR = 0.8471; boxG = 0.6471; boxB = 0.4471; metersR = 0.7843; metersG = 0.6549; metersB = 0.3843; colorOrder = 13; }
	else if (colorOrder == 13) { boxR = 0.5961; boxG = 0.3216; boxB = 0.7412; metersR = 0.6784; metersG = 0.1333; metersB = 0.7725; colorOrder = 14; }
	else if (colorOrder == 14) { boxR = 0.6980; boxG = 0.1647; boxB = 0.3608; metersR = 0.7804; metersG = 0.2902; metersB = 0.4706; colorOrder = 1; }

	if (playersSavedColors[enemyTeamID][0]) {
		metersR = playersSavedColors[enemyTeamID][0]; metersG = playersSavedColors[enemyTeamID][1]; metersB = playersSavedColors[enemyTeamID][2];
		boxR = playersSavedColors[enemyTeamID][3]; boxG = playersSavedColors[enemyTeamID][4]; boxB = playersSavedColors[enemyTeamID][5];
	}

	if (Settings::playerName && Settings::playerDistance) { // Player Name + Distance
		// box name esp
		DrawFilledRect(pCanvas, FVector2D(centerTop.X - 47 + 15, centerTop.Y - 40 + 4), 50 - 8 + 55 + 5, 20, FLinearColor(boxR * 255.f, boxG * 255.f, boxB * 255.f, 0.85f));

		// box distance esp
		DrawFilledRect(pCanvas, FVector2D(centerTop.X - 47 - 36 + 15, centerTop.Y - 40 + 4), 50 - 8 + -6, 20, FLinearColor(metersR * 255.f, metersG * 255.f, metersB * 255.f, 0.85f));

		// triangle above box esp
		DrawTriangle(pCanvas, FVector2D(centerTop.X - 10, centerTop.Y - 40 + 19 + 4), FVector2D(centerTop.X + 10, centerTop.Y - 40 + 19 + 4), FVector2D(centerTop.X, centerTop.Y - 40 + 19 + 15 + 4), FLinearColor(boxR * 255.f, boxG * 255.f, boxB * 255.f, 0.85f), true);

		// line between triangle and box esp
		pCanvas->K2_DrawLine(FVector2D(centerTop.X - 47 - 36 + 15 + 1, centerTop.Y - 40 + 4 + 20 - 1), FVector2D(centerTop.X - 47 + 15 + 50 - 8 + 55 + 5 - 0.5, centerTop.Y - 40 + 4 + 20 - 1), 2.f, FLinearColor(255.f, 255.f, 255.f, 1.f));

	}
	else if (Settings::playerName && !Settings::playerDistance) { // Player Name only
		// box name esp
		DrawFilledRect(pCanvas, FVector2D(centerTop.X - 47 - 36 + 15, centerTop.Y - 40 + 4), 50 - 8 + -6 + 50 - 8 + 55 + 5, 20, FLinearColor(boxR * 255.f, boxG * 255.f, boxB * 255.f, 0.85f));

		// triangle above box esp
		DrawTriangle(pCanvas, FVector2D(centerTop.X - 10, centerTop.Y - 40 + 19 + 4), FVector2D(centerTop.X + 10, centerTop.Y - 40 + 19 + 4), FVector2D(centerTop.X, centerTop.Y - 40 + 19 + 15 + 4), FLinearColor(boxR * 255.f, boxG * 255.f, boxB * 255.f, 0.85f), true);

		// line between triangle and box esp
		pCanvas->K2_DrawLine(FVector2D(centerTop.X - 47 - 36 + 15 + 1, centerTop.Y - 40 + 4 + 20 - 1), FVector2D(centerTop.X - 47 + 15 + 50 - 8 + 55 + 5 - 0.5, centerTop.Y - 40 + 4 + 20 - 1), 2.f, FLinearColor(255.f, 255.f, 255.f, 1.f));
	}
	else if (!Settings::playerName && Settings::playerDistance) { // Distance only +12
		// box distance esp
		DrawFilledRect(pCanvas, FVector2D(centerTop.X - 10 - 21.3 + 12, centerTop.Y - 40 + 4), 50 - 8 + -6, 20, FLinearColor(metersR * 255.f, metersG * 255.f, metersB * 255.f, 0.85f));

		// triangle above box esp
		DrawTriangle(pCanvas, FVector2D(centerTop.X - 10, centerTop.Y - 40 + 19 + 4), FVector2D(centerTop.X + 10, centerTop.Y - 40 + 19 + 4), FVector2D(centerTop.X, centerTop.Y - 40 + 19 + 15 + 4), FLinearColor(boxR * 255.f, boxG * 255.f, boxB * 255.f, 0.85f), true);

		// line between triangle and box esp
		pCanvas->K2_DrawLine(FVector2D(centerTop.X - 10 - 21.3 + 12 + 1, centerTop.Y - 40 + 4 + 20 - 1), FVector2D(centerTop.X - 10 - 21.3 + 12 + 50 - 8 + -6, centerTop.Y - 40 + 4 + 20 - 1), 2.f, FLinearColor(255.f, 255.f, 255.f, 1.f));
	}
	
	// Save color for the team index
	playersSavedColors[enemyTeamID][0] = metersR; playersSavedColors[enemyTeamID][1] = metersG; playersSavedColors[enemyTeamID][2] = metersB;
	playersSavedColors[enemyTeamID][3] = boxR; playersSavedColors[enemyTeamID][4] = boxG; playersSavedColors[enemyTeamID][5] = boxB;
}