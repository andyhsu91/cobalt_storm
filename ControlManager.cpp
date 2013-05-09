#include "ControlManager.h"


using namespace std;

ControlManager::ControlManager(void)
{


	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) < 0)
    {
        fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_Surface *screen = SDL_SetVideoMode(640, 480, 0, SDL_OPENGL);
    printf("%i joysticks were found.\n\n", SDL_NumJoysticks() );
    printf("The names of the joysticks are:\n");
		int i=0;
    for(; i < SDL_NumJoysticks(); i++ ) 
    {
        printf("    %s\n", SDL_JoystickName(i));
    }

    //SDL_Joystick *joystick;

    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);

    if(joystick)
  {
    printf("Opened Joystick 0\n");
    printf("Name: %s\n", SDL_JoystickName(0));
    printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joystick));
    printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joystick));
    printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joystick));
  }


}

ControlManager::~ControlManager(void)
{
	
}

//will return the state of the control when it is called
//this should be called in the main loop to update controls
controlState ControlManager::GetControlState(void)
{
SDL_JoystickUpdate();
printf("%d \n", SDL_JoystickGetButton(joystick, 0) );
 //this should be called in the main loop
    while(SDL_PollEvent(&event))
    {  
    	printf("WHILE EVENT\n\n");
        switch(event.type)
        {  
            case SDL_KEYDOWN:
            printf("KEY PRESSED\n\n");
            /* handle keyboard stuff here */				
            break;

            case SDL_QUIT:
            /* Set whatever flags are necessary to */
            /* end the main game loop here */
            break;

            case SDL_JOYAXISMOTION:  /* Handle Joystick Motion */
			    //if ( ( event.jaxis.value < -3200 ) || (event.jaxis.value > 3200 ) ) 
			    //{
			    	//cerr<< "the joystick was moved"<< endl;
			    	//The first control joystick
			    	if(event.jaxis.which == 0)
			    	{
				        if( event.jaxis.axis == 0) 
						    {
						         /* Left-right movement code goes here */
						    	playerControls.axis1LR = event.jaxis.value;
						    }

						    if( event.jaxis.axis == 1) 
						    {
						        /* Up-Down movement code goes here */
						    	playerControls.axis1UD = event.jaxis.value;
						    }
					//the second controller joystick
					}else if(event.jaxis.which == 1)
					{
						 if( event.jaxis.axis == 0) 
						    {
						         /* Left-right movement code goes here */
						    	playerControls.axis2LR = event.jaxis.value;
						    }

						    if( event.jaxis.axis == 1) 
						    {
						            /* Up-Down movement code goes here */
						    	playerControls.axis2UD = event.jaxis.value;
						    }
					}
			    //}
			    break;


        }
    }
return playerControls;
    /* End loop here */
}