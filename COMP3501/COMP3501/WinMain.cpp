////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////
#include "system.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	SystemClass* system;
	bool result;
	
	
	// Create the system object.
	system = new SystemClass;
	if(!system) {
		return 0;
	}

	// Initialize and run the system object.
	result = system->Initialize();
	if(result) {
		system->Run();
	}

	// Shutdown and release the system object.
	system->Shutdown();
	delete system;
	system = 0;

	return 0;
}