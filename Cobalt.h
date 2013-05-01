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

	/*virtual bool keyPressed( const OIS::KeyEvent &arg );
        virtual bool keyReleased( const OIS::KeyEvent &arg );
        virtual bool mouseMoved( const OIS::MouseEvent &arg );
        virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
        virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	*/
private:
	

};
 
#endif // #ifndef __Cobalt_h_
