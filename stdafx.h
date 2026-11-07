#pragma once

#define _HAS_STD_BYTE 0 // avoid std::byte vs. rpcndr byte ambiguity (GDI+)
#include <windows.h>
#include <algorithm>
#include <atlbase.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <io.h>
#include <fcntl.h>

#include "Debug.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
