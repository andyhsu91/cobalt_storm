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
#include "GUI.h"
#include "ControlManager.h"
#include "Player.h"
#include "Network.h"
#include "Sound.h"
 
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
    virtual bool mouseMoved( const OIS::MouseEvent &evt );
    virtual bool mousePressed( const OIS::MouseEvent &evt, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &evt, OIS::MouseButtonID id );


    virtual bool povMoved( const OIS::JoyStickEvent &e, int pov );
    virtual bool axisMoved( const OIS::JoyStickEvent &e, int axis );
    virtual bool buttonPressed( const OIS::JoyStickEvent &e, int button );
    virtual bool buttonReleased( const OIS::JoyStickEvent &e, int button );
	PlayerVars* createPacket(void);

private:
    
    Player* serverPlayer;
    Player* clientPlayer;
    Player* myself;	
    Player* enemy;
    Ogre::Real timeElapsed;
    PlayerVars* gameUpdate;


    Ogre::Vector3 cameraPos;
    Ogre::Vector3 serverPos;
    Ogre::Vector3 clientPos;
    Ogre::Vector3* myPos;
    Ogre::Vector3* enemyPos;
    float gameOverTimeOut;
    float timeElapsedSinceGameOver;
    
};
 
#endif // #ifndef __Cobalt_h_
