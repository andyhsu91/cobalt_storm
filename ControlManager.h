#ifndef __ControlManager_h_
#define __ControlManager_h_


#include <SDL/SDL.h>
#include <iostream>
struct controlState {
	//the two Joystick values, left/right values and up/down values
	// (range: -32768 to 32767)
   long axis1LR, axis1UD, axis2LR, axis2UD;
   bool jump, shoot;
};

class ControlManager {
public:
ControlManager();
~ControlManager(void);
controlState GetControlState();

private:
SDL_Event event;
controlState playerControls;
SDL_Joystick *joystick;
};


#endif
