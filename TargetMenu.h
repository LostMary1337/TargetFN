#pragma once
using namespace std;
using namespace SDK;

// Defines
#define D2R(d) (d / 180.f) * M_PI

// Variables
FVector2D Target_MenuInitialPos = { 400, 400 };
FVector2D borderStartPos;
FVector2D borderSize;
FLinearColor Target_ThemeColor = { 0, 89, 158, 1.f };
bool Target_bIsDragging = false;
int Target_MenuDragStartX = 0;
int Target_MenuDragStartY = 0;
int Target_MenuW = 539;
int Target_MenuH = 618;
int Target_TabIndex = 1;
FVector2D Target_ItemsCalculation;
FKey LeftMouseButton = { FName(FNAME_LEFTMOUSEBUTTON) };
APlayerController* PlayerController;
UKismetMathLibrary* MathLib;
string Target_MenuTitle = xMe("TARGET INSERT");

// Function headers
void MenuDrawTabs(UCanvas* pCanvas);
void MenuDrawItemsFor(UCanvas* pCanvas, int index);
bool IsInMenu(int x, int y);
UFont* GetFont();
void MenuHandleMouseClicks();

void DrawMenu(UCanvas* pCanvas, APlayerController* pc, UKismetMathLibrary* ml) {
	PlayerController = pc;
	MathLib = ml;

	// Menu background
	DrawFilledRect(pCanvas, Target_MenuInitialPos, Target_MenuW - 2, Target_MenuH - 2, FLinearColor(255, 255, 255, 1.f));
	// Menu tabs
	MenuDrawTabs(pCanvas);
	// Items in the menu
	MenuDrawItemsFor(pCanvas, Target_TabIndex);
	// Handle mouse clicks
	MenuHandleMouseClicks();

	float x, y;
	PlayerController->GetMousePosition(&x, &y);
	if (x && y) pCanvas->K2_DrawBox(FVector2D{ x - 2, y - 1 }, { 4, 4 }, 1.f, FLinearColor(0, 0, 0, 1.f));

	// Dev related
	/*if (Settings::devOutput) {
		if (GetAsyncKeyState(VK_F1)) {
			Settings::devFloat -= 1.f;
		}
		if (GetAsyncKeyState(VK_F2)) {
			Settings::devFloat += 1.f;
		}

		char tier_data[256];
		wchar_t tier_data_wide[256];
		sprintf(tier_data, xMe("TARGET DEV OUTPUT:\nFloat (F1/F2): %d"), Settings::devFloat);
		AnsiToWide(tier_data, tier_data_wide);
		pCanvas->K2_DrawText(Globals::Font, tier_data_wide, { 30, 280 }, FVector2D(1.0f, 1.0f), FLinearColor(0.f, 255.f, 0.f, 1.f), 1.0f, FLinearColor(0, 0, 0, 0), FVector2D(), false, false, true, FLinearColor(0, 0, 0, 1.0f));
	}*/
}

bool IsInMenu(int x, int y)
{
	return (x >= Target_MenuInitialPos.X) && (x <= Target_MenuInitialPos.X + Target_MenuW) && (y >= Target_MenuInitialPos.Y) && (y <= Target_MenuInitialPos.Y + Target_MenuH);
}

UFont* GetFont()
{
	static UFont* font = 0;
	if (!font) font = UObject::FindObject<UFont>(xMe("Font Roboto.Roboto"));

	return font;
}

bool IsInRectangle(int x1, int y1, int x2, int y2, int x, int y) {
	if (x > x1 and x < x2 and y > y1 and y < y2)
		return true;

	return false;
}

void MenuDrawTabs(UCanvas* pCanvas) {
	vector<const wchar_t*> pTabs;

	char charHolder[7];
	wchar_t strAimbot[7];
	sprintf(charHolder, xMe("AIMBOT"));
	AnsiToWide(charHolder, strAimbot);

	wchar_t strVisuals[7];
	sprintf(charHolder, xMe("VISUALS"));
	AnsiToWide(charHolder, strVisuals);

	wchar_t strMisc[7];
	sprintf(charHolder, xMe("MISC"));
	AnsiToWide(charHolder, strMisc);

	pTabs.push_back(strAimbot);
	pTabs.push_back(strVisuals);
	pTabs.push_back(strMisc);

	auto inp = Target_MenuInitialPos + FVector2D({ 20, 35 });

	static char logo_buff[256];
	static wchar_t logo_buff_wide[256];
	sprintf(logo_buff, xMe("%s (Build date: %s %s)"), Target_MenuTitle, xMe(__DATE__), xMe(__TIME__));
	AnsiToWide(logo_buff, logo_buff_wide);
	pCanvas->K2_DrawText(GetFont(), logo_buff_wide, Target_MenuInitialPos + FVector2D({ 25, 11 }), FVector2D(1.0f, 1.0f), FLinearColor(0.f, 0.f, 0.f, 1.f), 1.0f, FLinearColor(0, 0, 0, 1.0f), FVector2D(), false, false, false, FLinearColor(0, 0, 0, 1.0f));

	auto tabsz = (Target_MenuW - 40) / pTabs.size(); tabsz -= 2;
	FVector2D ip = inp + FVector2D(2, 2);

	// for border around tabs
	FVector2D startPos;
	FVector2D widthHeight; widthHeight.Y = 22;

	auto i = 0;
	for (int p = 0; p < pTabs.size(); p++) {
		auto tab = pTabs.at(p);

		auto clr2 = FLinearColor(255.f, 255.f, 255.f, 1.f);
		auto clr = FLinearColor(0.f, 0.f, 0.f, 1.f);
		if (Target_TabIndex == i) {
			clr = FLinearColor(255.f, 255.f, 255.f, 1.f);
			clr2 = Target_ThemeColor;
		}

		DrawFilledRect(pCanvas, ip, tabsz, 22, clr2);

		int x1 = ip.X, y1 = ip.Y, x2 = ip.X + tabsz, y2 = ip.Y + 22;

		// given point
		float x, y;
		PlayerController->GetMousePosition(&x, &y);
		if (IsInRectangle(x1, y1, x2, y2, x, y)) {
			Target_bIsDragging = false;
			if (PlayerController->WasInputKeyJustPressed(LeftMouseButton))
				Target_TabIndex = i;
		}

		if (!startPos.X || startPos.X == 0)
			startPos = ip;

		widthHeight.X += tabsz + 2;

		FVector2D TextSize0 = pCanvas->K2_TextSize(GetFont(), tab, FVector2D(1.0f, 1.0f));
		float bestFitByDanielle = (ip.X + (tabsz / 2)) - (TextSize0.X / 2);
		pCanvas->K2_DrawText(GetFont(), tab, FVector2D(bestFitByDanielle, ip.Y + 3), FVector2D(1.0f, 1.0f), clr, 1.0f, FLinearColor(0, 0, 0, 1.0f), FVector2D(), false, false, false, FLinearColor(0, 0, 0, 1.0f));

		ip.X += tabsz + 2;
		i++;
	}
	widthHeight.X -= 2;

	// draw border
	pCanvas->K2_DrawBox(startPos, widthHeight, 1.f, FLinearColor(150, 150, 150, 1.f));
	borderStartPos = startPos;
	borderSize = widthHeight;

	Target_ItemsCalculation = inp + FVector2D(0, 35);
}

void Render_Circle(UCanvas* pCanvas, FVector2D pos, int r, FLinearColor color)
{
	float circum = M_PI * 2.f * r;

	int seg;
	int inum = (int)circum;
	if (circum == (float)inum) {
		seg = inum;
	}
	seg = inum + 1;

	if (seg > 180) seg = 180;

	float theta = 0.f;
	float step = 180.f / seg;

	for (size_t i = 0; i < seg; ++i)
	{
		theta = i * step;
		auto delta = FVector2D(MathLib->STATIC_Round(r * MathLib->STATIC_Sin(D2R(theta))), MathLib->STATIC_Round(r * MathLib->STATIC_Cos(D2R(theta))));
		pCanvas->K2_DrawLine(pos + delta, pos - delta, 1.f, color);
	}
}

void Render_Clear(UCanvas* pCanvas, FVector2D one, FVector2D two, FLinearColor color) {
	for (auto x = one.X; x < two.X; x += 1.f)
	{
		pCanvas->K2_DrawLine(FVector2D(x, one.Y), FVector2D(x, two.Y), 1.f, color);
	}
}

void Render_PointArray(UCanvas* pCanvas, size_t count, FVector2D* ary, FLinearColor color) {
	for (size_t i = 1; i < count; ++i)
		pCanvas->K2_DrawLine(ary[i - 1], ary[i], 1.0f, color);
}

void Render_CircleOutline(UCanvas* pCanvas, FVector2D pos, int r, FLinearColor outline) {
	float circum = M_PI * 2.f * r;

	int seg;
	int inum = (int)circum;
	if (circum == (float)inum) {
		seg = inum;
	}
	seg = inum + 1;

	if (seg > 180) seg = 180;

	float theta = 0.f;
	float step = 360.f / seg;

	FVector2D points[180] = {};

	for (size_t i = 0; i < seg; ++i)
	{
		theta = i * step;
		points[i] = FVector2D(pos.X + MathLib->STATIC_Round(r * MathLib->STATIC_Sin(D2R(theta))), pos.Y + MathLib->STATIC_Round(r * MathLib->STATIC_Cos(D2R(theta))));
	}

	Render_PointArray(pCanvas, seg, points, outline);
}

void Render_CircleOutlined(UCanvas* pCanvas, FVector2D pos, int r, FLinearColor fill, FLinearColor outline) {
	Render_Circle(pCanvas, pos, r, fill);
	Render_CircleOutline(pCanvas, pos, r, outline);
}

void Render_Toggle(UCanvas* pCanvas, FVector2D& loc_ref, const wchar_t* name, bool* on) {
	auto loc = &loc_ref;

	float mouseX, mouseY;
	PlayerController->GetMousePosition(&mouseX, &mouseY);

	bool hover = mouseX > loc->X && mouseX < (loc->X + 64) && mouseY > loc->Y && mouseY < (loc->Y + 20);
	if (hover)
		Target_bIsDragging = false;

	if (PlayerController->WasInputKeyJustPressed(LeftMouseButton) && hover)
		*on = !*on;

	FLinearColor col = *on ? Target_ThemeColor : FLinearColor(170.f, 170.f, 170.f, 1.f);

	Render_Circle(pCanvas, FVector2D(loc->X + 10, loc->Y + 10), 6, col);
	Render_Circle(pCanvas, FVector2D(loc->X + 18, loc->Y + 10), 6, col);
	Render_Clear(pCanvas, FVector2D(loc->X + 10, loc->Y + 4), FVector2D(loc->X + 18, loc->Y + 16), col);

	if (*on) {
		Render_CircleOutlined(pCanvas, FVector2D(loc->X + 18, loc->Y + 10), 5, hover ? FLinearColor(0.8f * 255, 0.8f * 255, 0.8f * 255, 1.f) : FLinearColor(255, 255, 255, 1), FLinearColor(0, 0, 0, 1.f));
		pCanvas->K2_DrawLine(FVector2D(loc->X + 9, loc->Y + 8), FVector2D(loc->X + 9, loc->Y + 12), 1.f, FLinearColor(0, 0, 0, 1.f));
	}
	else {
		Render_CircleOutlined(pCanvas, FVector2D(loc->X + 10, loc->Y + 10), 5, hover ? FLinearColor(0.8f * 255, 0.8f * 255, 0.8f * 255, 1.f) : FLinearColor(255, 255, 255, 1), FLinearColor(0, 0, 0, 1.f));
		Render_CircleOutline(pCanvas, FVector2D(loc->X + 19, loc->Y + 10), 2, FLinearColor(0, 0, 0, 1.f));
	}

	pCanvas->K2_DrawText(GetFont(), name, FVector2D(loc->X + 32, loc->Y + 2), FVector2D(1.0f, 1.0f), FLinearColor(0.f, 0.f, 0.f, 1.f), 1.0f, FLinearColor(), FVector2D(), false, false, false, FLinearColor(0, 0, 0, 1.f));

	loc->Y += 25.0f;
}

void Render_Slider(UCanvas* pCanvas, const wchar_t* name, float minimum, float maximum, float* val, FVector2D* loc) {
	auto kinda_white = FLinearColor(204.f, 204.f, 204.f, 1.f);

	pCanvas->K2_DrawText(GetFont(), name, FVector2D(loc->X + 6, loc->Y), FVector2D(1.0f, 1.0f), FLinearColor(0.f, 0.f, 0.f, 1.f), 1.0f, FLinearColor(), FVector2D(), false, false, false, FLinearColor(0, 0, 0, 1.f));

	loc->X += 6.f;

	float mouseX, mouseY;
	PlayerController->GetMousePosition(&mouseX, &mouseY);

	bool hover = mouseX > loc->X && mouseX < (loc->X + 199) && mouseY > loc->Y && mouseY < (loc->Y + 30);
	if (hover)
		Target_bIsDragging = false;

	if (PlayerController->IsInputKeyDown(LeftMouseButton) && hover) {
		float ratio = (float)(mouseX - loc->X) / 199;
		if (ratio < 0.f) ratio = 0.f;
		if (ratio > 1.f) ratio = 1.f;
		*val = minimum + ((maximum - minimum) * ratio);
	}

	int xpos = ((*val - minimum) / (maximum - minimum)) * 199;
	loc->Y += 24.f;

	Render_Circle(pCanvas, *loc, 3, Target_ThemeColor);
	Render_Clear(pCanvas, FVector2D(loc->X, loc->Y - 3), FVector2D(loc->X + xpos, loc->Y + 3), Target_ThemeColor);

	Render_Clear(pCanvas, FVector2D(loc->X + xpos, loc->Y - 3), FVector2D(loc->X + 199, loc->Y + 3), kinda_white);
	Render_Circle(pCanvas, FVector2D(loc->X + 199, loc->Y), 3, kinda_white);

	Render_CircleOutlined(pCanvas, FVector2D(loc->X + xpos, loc->Y), 5, FLinearColor(255.f, 255.f, 255.f, 1.f), FLinearColor(0.f, 0.f, 0.f, 1.f));

	loc->Y -= 24.f;

	wchar_t wstr[16] = {};
	char __str[16] = {};
	sprintf(__str, xMe("%0.1f"), *val);
	AnsiToWide(__str, wstr);
	pCanvas->K2_DrawText(GetFont(), wstr, FVector2D(loc->X + 199 - 30.0f, loc->Y + 33), FVector2D(1.0f, 1.0f), FLinearColor(0.f, 0.f, 0.f, 1.f), 1.0f, FLinearColor(), FVector2D(), false, false, false, FLinearColor(0, 0, 0, 1.f));

	loc->X -= 6.f;
	loc->Y += 35.0f;
	loc->Y += 13.0f;
}

void MenuCheckbox(UCanvas* pCanvas, FVector2D sp, const wchar_t* text, bool* shittobind)
{
	FLinearColor Green = { 0.000000000f, 0.501960814f * 255, 0.000000000f, 1.000000000f };
	FLinearColor SlateGray = { 0.439215720f * 255, 0.501960814f * 255, 0.564705908f * 255, 1.000000000f };
	auto color = *shittobind ? Green : SlateGray;
	sp.X += 3;
	FLinearColor gayshit = { 0.06f * 255, 0.06f * 255, 0.06f * 255, 1.000000000f };
	//RegisterButtonControl(sp + g_Clientarea, 15, 15, gayshit, -1, shittobind);
	DrawFilledRect(pCanvas, sp + Target_ItemsCalculation + 3, 9, 9, color);
	pCanvas->K2_DrawText(GetFont(), text, sp + Target_ItemsCalculation + FVector2D({ 20, -2 }), FVector2D(1.0f, 1.0f), FLinearColor(255, 255, 255, 1), 1.0f, FLinearColor(0, 0, 0, 1), FVector2D(), false, false, true, FLinearColor(0, 0, 0, 0));
}

void MenuDrawItemsFor(UCanvas* pCanvas, int index) {
	// draw border
	pCanvas->K2_DrawBox(FVector2D(borderStartPos.X, Target_ItemsCalculation.Y + 4), FVector2D(borderSize.X, Target_MenuH - 95), 1.f, FLinearColor(150, 150, 150, 1.f));

	if (index == 0)
	{
		auto loc = Target_ItemsCalculation + FVector2D(11, 12);
		//Render_Toggle(pCanvas, loc, xMe(L"Enable aimbot (F1)"), &Settings::aimbot);
		//Render_Toggle(pCanvas, loc, xMe(L"Silent (option)"), &Settings::silent);
		Render_Toggle(pCanvas, loc, xMe(L"Enable Silent (F1)"), &Settings::silent);
		//Render_Toggle(pCanvas, loc, xMe(L"Building weaks spot aimbot"), &Settings::aimWeakspot); // aim weakspot not added yet
		Render_Toggle(pCanvas, loc, xMe(L"Aim at Body"), &Settings::aimBody);
		Render_Toggle(pCanvas, loc, xMe(L"Aim through walls"), &Settings::aimVisCheck);
		Render_Slider(pCanvas, xMe(L"Aimbot FOV"), 10, 100, &Settings::FOV, &loc);
		//Render_Slider(pCanvas, xMe(L"[x] Smooth"), 0, 1, &Settings::smooth, &loc); // not added yet
	}
	if (index == 1)
	{
		auto loc = Target_ItemsCalculation + FVector2D(11, 12);
		Render_Toggle(pCanvas, loc, xMe(L"Enable visuals (F2)"), &Settings::visuals);
		Render_Toggle(pCanvas, loc, xMe(L"Enable all game items"), &Settings::allLoot);
		Render_Toggle(pCanvas, loc, xMe(L"Supply drops / Llamas"), &Settings::supplyLlama);
		Render_Toggle(pCanvas, loc, xMe(L"Loot / dropped items"), &Settings::loot);
		//Render_Toggle(pCanvas, loc, xMe(L"[x] Vehicles"), &Settings::vehicles); // not added yet
		Render_Toggle(pCanvas, loc, xMe(L"Chests"), &Settings::chests);
		Render_Toggle(pCanvas, loc, xMe(L"Ammo"), &Settings::ammo);
		Render_Toggle(pCanvas, loc, xMe(L"Player weapons"), &Settings::playerWeapon);
		Render_Toggle(pCanvas, loc, xMe(L"Player name"), &Settings::playerName);
		Render_Toggle(pCanvas, loc, xMe(L"Player distance"), &Settings::playerDistance);
		Render_Toggle(pCanvas, loc, xMe(L"Player box"), &Settings::playerBox);
		Render_Toggle(pCanvas, loc, xMe(L"Visiblity check"), &Settings::visCheck);
		Render_Toggle(pCanvas, loc, xMe(L"Player Corner box (option)"), &Settings::playerCornerBox);
		Render_Toggle(pCanvas, loc, xMe(L"Draw snaplines"), &Settings::snaplines);
		//Render_Toggle(pCanvas, loc, xMe(L"[x] Dashing snaplines (option)"), &Settings::dashingSnaplines); // not added yet
		Render_Toggle(pCanvas, loc, xMe(L"Draw local player / self"), &Settings::drawSelf);

		loc.Y = Target_ItemsCalculation.Y + 12;
		loc.X += Target_MenuW / 2 - 1;

		Render_Slider(pCanvas, xMe(L"Loot distance"), 50, 500, &Settings::lootDistance, &loc);
		Render_Slider(pCanvas, xMe(L"Chests / ammo distance"), 50, 500, &Settings::chestsAmmoDistance, &loc);
		Render_Slider(pCanvas, xMe(L"Overall ESP render distance"), 1, 650, &Settings::overallDistance, &loc);
		Render_Slider(pCanvas, xMe(L"Loot minimal tier"), 1, 7, &Settings::lootMinimalTier, &loc);
		Settings::lootMinimalTier = (float)(int)(Settings::lootMinimalTier);
		Render_Slider(pCanvas, xMe(L"Snapline power"), 1, 15, &Settings::snaplinePower, &loc);
		Render_Slider(pCanvas, xMe(L"Player box corner scale"), 0.1f, 5.0f, &Settings::cornerboxScale, &loc);
		Render_Slider(pCanvas, xMe(L"Player box thickness"), 1, 6, &Settings::playerBoxThickness, &loc);
	}
	if (index == 2)
	{
		auto loc = Target_ItemsCalculation + FVector2D(11, 12);
		Render_Toggle(pCanvas, loc, xMe(L"Enable side info text"), &Settings::infoText);
		Render_Toggle(pCanvas, loc, xMe(L"Show custom crosshair"), &Settings::customCrosshair);
		Render_Toggle(pCanvas, loc, xMe(L"Show aim FOV"), &Settings::drawFOV);
		Render_Toggle(pCanvas, loc, xMe(L"Show closest enemy distance"), &Settings::showClosestDistance);
		//Render_Toggle(pCanvas, loc, xMe(L"Enable dev output"), &Settings::devOutput);
	}
}

void MenuHandleMouseClicks() {
	if (PlayerController) {
		if (PlayerController->WasInputKeyJustPressed(LeftMouseButton)) {
			float locationX, locationY;
			PlayerController->GetMousePosition(&locationX, &locationY);
			if (IsInMenu(locationX, locationY)) {
				Target_bIsDragging = true;
				Target_MenuDragStartX = locationX;
				Target_MenuDragStartY = locationY;
			}
		}
		if (PlayerController->WasInputKeyJustReleased(LeftMouseButton)) {
			Target_bIsDragging = false;
		}
		if (Target_bIsDragging) {
			float locationX, locationY;
			PlayerController->GetMousePosition(&locationX, &locationY);

			int width, height;
			PlayerController->GetViewportSize(&width, &height);
			auto newX = Target_MenuInitialPos.X + (locationX - Target_MenuDragStartX);
			auto newY = Target_MenuInitialPos.Y + (locationY - Target_MenuDragStartY);
			if (newX >= width - Target_MenuW)
				newX = width - Target_MenuW;
			if (newY >= height - Target_MenuH)
				newY = height - Target_MenuH;
			if (newX <= 0)
				newX = 0;
			if (newY <= 0)
				newY = 0;

			Target_MenuInitialPos.X = newX;
			Target_MenuInitialPos.Y = newY;
			Target_MenuDragStartX = locationX;
			Target_MenuDragStartY = locationY;
		}
	}
}