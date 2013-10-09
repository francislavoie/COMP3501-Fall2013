////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////
#include "system.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	System* System;
	bool result;
	
	
	// Create the system object.
	System = new System;
	if(!System) {
		return 0;
	}

	// Initialize and run the system object.
	result = System->Initialize();
	if(result) {
		System->Run();
	}

	// Shutdown and release the system object.
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}