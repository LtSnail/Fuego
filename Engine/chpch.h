#pragma once

#include <iostream>
#include <memory>
#include <functional>

#include <string>
#include <ostream>
#include <sstream>
#include <vector>
#include <queue>

#ifdef FUEGO_PLATFORM_MACOS
    #ifdef __OBJC__
        #import <Cocoa/Cocoa.h>
        #import <simd/simd.h>
        #import <Metal/Metal.h>
        #import <MetalKit/MetalKit.h>
        #import <ModelIO/ModelIO.h>
    #endif
#elif FUEGO_PLATFORM_WIN
	#include <windows.h>
#endif

