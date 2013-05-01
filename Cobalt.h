/*
-----------------------------------------------------------------------------
Filename:    Cobalt.h
-----------------------------------------------------------------------------
*/
#ifndef __Cobalt_h_
#define __Cobalt_h_

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#  include <OIS/OISEvents.h>
#  include <OIS/OISInputManager.h>
#  include <OIS/OISKeyboard.h>
#  include <OIS/OISMouse.h>

#  include <OGRE/SdkTrays.h>
#  include <OGRE/SdkCameraMan.h>
#else
#  include <OISEvents.h>
#  include <OISInputManager.h>
#  include <OISKeyboard.h>
#  include <OISMouse.h>

#  include <SdkTrays.h>
#  include <SdkCameraMan.h>
#endif
 
#include "BaseApplication.h"
#include "Environment.h"
#include "Physics.h"
#include "ControlManager.h"

#define LCONTROLX 0
#define LCONTROLY 1
#define RCONTROLX 3
#define RCONTROLY 4

#define LTRIG 2
#define RTRIG 2

#define LBUMP 4
#define RBUMP 5

#define LJOYCLICK 8
#define RJOYCLICK 9

#define RBUTTON1 0
#define RBUTTON2 1
#define RBUTTON3 2
#define RBUTTON4 3

#define LMIDBUTTON 6
#define RMIDBUTTON 7

 
class Cobalt : public BaseApplication
{
public:
	Cobalt(void);
	virtual ~Cobalt(void);
 
protected:
	virtual void createCamera(void);
	virtual void createScene(void);
	virtual void createFrameListener(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );


    virtual bool povMoved( const OIS::JoyStickEvent &e, int pov );
    virtual bool axisMoved( const OIS::JoyStickEvent &e, int axis );
    virtual bool buttonPressed( const OIS::JoyStickEvent &e, int button );
    virtual bool buttonReleased( const OIS::JoyStickEvent &e, int button );
	
private:
	

};
 
#endif // #ifndef __Cobalt_h_
