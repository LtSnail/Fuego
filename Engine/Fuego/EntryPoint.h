#pragma once

extern Fuego::Application* Fuego::CreateApplication();

#if defined(FUEGO_PLATFORM_MACOS)
/* MacOS-specific part of Entrypoint.h
 * Compiled in a client application
 */

#include "chpch.h"

#include "Log.h"

@interface FuegoApplication : NSApplication

@property Fuego::Application* application;

- (void)run;

@end

@implementation FuegoApplication;


- (void)run
{    
	[[NSNotificationCenter defaultCenter]
		postNotificationName:NSApplicationWillFinishLaunchingNotification
		object:NSApp];
	[[NSNotificationCenter defaultCenter]
		postNotificationName:NSApplicationDidFinishLaunchingNotification
		object:NSApp];
	
    _application->Run();
}

@end

int main(int argc, const char * argv[]) 
{
	@autoreleasepool 
	{
        Fuego::Log::Init();
         
		FuegoApplication* applicationObject = [FuegoApplication alloc];

		Fuego::Application* app = Fuego::CreateApplication();
		applicationObject.application = app;

		if ([applicationObject respondsToSelector:@selector(run)])
		{
			[applicationObject
				performSelectorOnMainThread:@selector(run)
				withObject:nil
				waitUntilDone:YES];
		}

		delete app;
	}
	
	return 0;
}
#endif
