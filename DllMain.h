#pragma once

#define M_PI 3.14159265358979323846264338327950288419716939937510582f

//#define TargetProject 2 //1: assist / 2: vision
#ifdef GAY_DEBUGGING
#define dprintf printf
#else
#define dprintf(x)
#endif

#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include "SDK.hpp"
using namespace std;
using namespace SDK;

#include "StringC.h"
#include "Settings.h"
#include "UE.h"
#include "Drawing.h"
#include <chrono>
#include "Misc.h"
#include "TargetMenu.h"

/*****function type definitions*****/

typedef void (*PostRender_t)(SDK::UObject* pObject, SDK::UCanvas* pCanvas);