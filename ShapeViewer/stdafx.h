#pragma once

#include "framework.h"

#define ASSERT(x) if(!(x)) __debugbreak();

#ifdef _DEBUG
#define CAD_DEBUGBREAK() __debugbreak()
#define CAD_ENABLE_ASSERTS
#else
#define CAD_DEBUGBREAK()
#endif // _DEBUG

#ifdef CAD_ENABLE_ASSERTS
#define CAD_ASSERT(x, ...) { if(!(x)) {CAD_ERROR("Assertion failed {0}", __VA_ARGS__); CAD_DEBUGBREAK();}}
#else 
#define CAD_ASSERT(x, ...)
#endif //CAD_ENABLE_ASSERTS


#include <memory>
#include <future>
#include <mutex>
#include <algorithm>
#include <vector>
#include <unordered_map>

#include <string>

#include <ogrsf_frmts.h>
#include <core/Log.h>
#include <sstream>
