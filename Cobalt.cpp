/*
-----------------------------------------------------------------------------
Filename:    Cobalt.cpp
-----------------------------------------------------------------------------
*/
 
#include "Cobalt.h"

using namespace std;

static Physics mBullet;
static Environment mEnv;
Player mPlayer;
//static ControlManager CtrlManager;
//static GUI mGUICtrl;
 bool isMultiplayer;
 bool isConnected;
 bool isServer;
 Network* nManager;
 Sound* sManager;

//-------------------------------------------------------------------------------------
Cobalt::Cobalt(void)
{
//0 is the Server
	serverPlayer = new Player();
	//clientPlayer = new Player();
}
//-------------------------------------------------------------------------------------
Cobalt::~Cobalt(void)
{
	if(nManager){delete nManager;}
	if(sManager){delete sManager;}
}
//-------------------------------------------------------------------------------------
void Cobalt::createCamera(void)
{
	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");
	
	mCamera->setPosition(Ogre::Vector3(700, 500, -300));
	mCamera->lookAt(Ogre::Vector3(700, 250, -550));
	mCamera->setNearClipDistance(0.1);
	mCamera->setFarClipDistance(50000);

	cameraTarget = Ogre::Vector3(700, 250, -700);

	if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
	{
		mCamera->setFarClipDistance(0);   // enable infinite far clip distance if we can
	}	
 	
	mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}
//-------------------------------------------------------------------------------------
void Cobalt::createScene(void)
{
	// create your scene here :)
	cerr << "Initing Bullet" << endl;	
	mBullet.initPhysics();
	
	cerr << "Initing Env" << endl;
	mEnv.initEnvironment(mSceneMgr, mWindow, &mBullet);
	
	cerr << "Initing GUI" << endl;
	//mGUICtrl.initGUI(mSceneMgr);

	//Initialize Network Manager
	nManager = new Network();
	//nManager->init();
	isConnected = nManager->isConnectionOpen();
	isServer = nManager->isThisServer();
	
	isMultiplayer = false; //change to true when ready

	if(isMultiplayer && isServer && !isConnected){
		nManager->waitForClientConnection();
		isConnected = nManager->isConnectionOpen();
	}
	if(isConnected){
		isServer = nManager->isThisServer();
	} else{
		isMultiplayer = false;
	}
	cout<<"Current State: isConnected="<<boolalpha<<isConnected<<", isMutliplayer="<<isMultiplayer<<", isServer="<<isServer<<endl;
	sManager = new Sound();
	sManager->playBackground(-1);
	cerr << "Initing Player" << endl;
    serverPlayer->initPlayer(mSceneMgr, &mBullet, "pnode");
	cerr << "Finished scene" << endl;	

}

//-------------------------------------------------------------------------------------
void Cobalt::createFrameListener(void)
{
	BaseApplication::createFrameListener();

	mEnv.createFL(mTrayMgr);
	
}
//-------------------------------------------------------------------------------------
bool Cobalt::frameRenderingQueued(const Ogre::FrameEvent& evt)
{	//return True to continue rendering, false to drop out of the rendering loop
	bool ret = BaseApplication::frameRenderingQueued(evt);

	if(isMultiplayer){
		bool packetReceived = nManager->checkForPackets(); //check for game updates and connection closed packets
		isConnected = nManager->isConnectionOpen();
		PlayerVars* gameUpdate = NULL;
		if(!isConnected){ mShutDown = true; return false; } //opponent closed connection
		if(packetReceived){ gameUpdate = nManager->getGameUpdate(); }

		if(isServer){
			//I am server


		}else{
			//I am client
			

		}
		

	}else{
		//single player mode
		
		Ogre::Vector3 playerVector = serverPlayer->getPlayerPosition();
		//printf("playerVector.x %f cameraTarget.x %f playerVector.z %f cameraTarget.z %f\n",playerVector.x, cameraTarget.x,playerVector.z, cameraTarget.z);
		 //playerVector = Ogre::Vector3((playerVector.x - cameraTarget.x)+50,100,(playerVector.z - cameraTarget.z)+50);
		Ogre::Real ctDistance = Ogre::Math::Sqrt(Ogre::Math::Sqr(playerVector.x - cameraTarget.x) + Ogre::Math::Sqr(playerVector.z - cameraTarget.z));

		if (ctDistance == 0.0) {
			ctDistance = .01;
		}
		Ogre::Vector3 CameraVector = Ogre::Vector3(
		 		playerVector.x + (((playerVector.x - cameraTarget.x)/ctDistance)*80),
		 		320,
				playerVector.z + (((playerVector.z - cameraTarget.z)/ctDistance)*80) );

		 //printf("X:%f Dist:%f  X/Dist:%f\n", (playerVector.x - cameraTarget.x),ctDistance,(playerVector.x - cameraTarget.x)/ctDistance);
		
		mCamera->setPosition(CameraVector);
		//mCamera->setPosition(Ogre::Vector3(playerVector.x, playerVector.y, playerVector.z));

		//mCamera->lookAt(cameraTarget);


		mBullet.updateWorld(evt);
		mEnv.frmqUpdate(evt, mTrayMgr);

		serverPlayer->updatePosition(evt);

	}
	

	return ret;
}

//-------------------------------------------------------------------------------------
bool Cobalt::keyPressed( const OIS::KeyEvent &arg )
{	
	if(arg.key == OIS::KC_ESCAPE)
		{ 
        mShutDown = true;
    	}
    else if (arg.key == OIS::KC_W)
		{
			serverPlayer->updateControlAxis(LCONTROLY, -1);
		}
	else if (arg.key == OIS::KC_A)
        {
			serverPlayer->updateControlAxis(LCONTROLX, -1);
        }
	else if (arg.key == OIS::KC_S)
        {
			serverPlayer->updateControlAxis(LCONTROLY, 1);
        }
	else if (arg.key == OIS::KC_D)
        {
			serverPlayer->updateControlAxis(LCONTROLX, 1);
        }
    else if (arg.key == OIS::KC_O)
        {
			serverPlayer->updateControlAxis(RBUMP, 1);
        }
    else if (arg.key == OIS::KC_P)
    	{
    		serverPlayer->updateControlAxis(LBUMP, 1);
    	}
       //this command will move the camera
	//mCameraMan->injectKeyDown(arg);
	return true;
}
//-------------------------------------------------------------------------------------
bool Cobalt::keyReleased( const OIS::KeyEvent &arg )
{
     if (arg.key == OIS::KC_W)
		{
			serverPlayer->updateControlAxis(LCONTROLY, 0);
		}
	else if (arg.key == OIS::KC_A)
        {
			serverPlayer->updateControlAxis(LCONTROLX, 0);
        }
	else if (arg.key == OIS::KC_S)
        {
			serverPlayer->updateControlAxis(LCONTROLY, 0);
        }
	else if (arg.key == OIS::KC_D)
        {
			serverPlayer->updateControlAxis(LCONTROLX, 0);
        }
    else if (arg.key == OIS::KC_O)
        {
			serverPlayer->updateControlAxis(RBUMP, 0);
        }
    else if (arg.key == OIS::KC_P)
        {
			serverPlayer->updateControlAxis(LBUMP, 0);
        }
//mCameraMan->injectKeyUp(arg);
	return true;
}
//-------------------------------------------------------------------------------------
bool Cobalt::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectMouseDown(arg, id)) return true;
	mCameraMan->injectMouseDown(arg, id);
	return true;
}
//-------------------------------------------------------------------------------------
bool Cobalt::mouseMoved( const OIS::MouseEvent &arg )
{
    if (mTrayMgr->injectMouseMove(arg)) return true;
	mCameraMan->injectMouseMove(arg);
}
//-------------------------------------------------------------------------------------
bool Cobalt::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectMouseUp(arg, id)) return true;
	mCameraMan->injectMouseUp(arg, id);
	return true;
}


//for buttons 1,2,3,4 on the left hand controller (probably broken)
 bool Cobalt::povMoved( const OIS::JoyStickEvent &e, int pov )
 {
 	/*printf("povMoved:\n");
 	cout<<pov<<endl;*/
    return true;
 }

/*The axis are LCONTROLX,LCONTROLY,RCONTROLX,RCONTROLY for the
 different joystick axis, they vary from MIN_AXIS TO MAX_AXIS from Left to Right
 and Up to Down,   LTRIG and RTRIG are the same input and vary from
 0 to MIN_AXIS, and 0 to MAX_AXIS but are added together(if you push both down
 then the sum will be 0)*/
 bool Cobalt::axisMoved( const OIS::JoyStickEvent &e, int axis )
 {
 	float axisValue = ((float)e.state.mAxes[axis].abs)/OIS::JoyStick::MAX_AXIS;
 	//printf("axisMoved %f\n",((float)e.state.mAxes[axis].abs)/OIS::JoyStick::MAX_AXIS);
	 	serverPlayer->updateControlAxis(axis, axisValue);

    return true;
 }

/*the different buttons are LBUMP,RBUMP, LJOYCLICK, RJOYCLICK
 RBUTTON1, RBUTTON2, RBUTTON3, RBUTTON4, LMIDBUTTON, RMIDBUTTON */
 bool Cobalt::buttonPressed( const OIS::JoyStickEvent &e, int button )
 {
 	printf("buttonPressed %d\n",button);
 	switch(button)
 	{
		case LBUMP:
		serverPlayer->updatePlayerState(SHOOTING1, true);
		break;
		case RBUMP:
		serverPlayer->updatePlayerState(SHOOTING2, true);
		break;
		case LJOYCLICK:
		serverPlayer->updatePlayerState(JUMPING, true);
		break;
		case RJOYCLICK:
		serverPlayer->updatePlayerState(JUMPING, true);
		break;

		//these are not currently used
		case RBUTTON1:
		break;
		case RBUTTON2:
		break;
		case RBUTTON3:
		break;
		case RBUTTON4:
		break;
		case LMIDBUTTON:
		break;
		case RMIDBUTTON:
		break;
		default:
		break;
 	}

	
 	return true;
 }
 bool Cobalt::buttonReleased( const OIS::JoyStickEvent &e, int button )
 {
 	//printf("buttonReleased\n");
 	switch(button)
 	{
		case LBUMP:
		serverPlayer->updatePlayerState(SHOOTING1, false);
		break;
		case RBUMP:
		serverPlayer->updatePlayerState(SHOOTING2, false);
		break;
		case LJOYCLICK:
		serverPlayer->updatePlayerState(JUMPING, false);
		break;
		case RJOYCLICK:
		serverPlayer->updatePlayerState(JUMPING, false);
		break;

		//these are not currently used
		case RBUTTON1:
		break;
		case RBUTTON2:
		break;
		case RBUTTON3:
		break;
		case RBUTTON4:
		break;
		case LMIDBUTTON:
		break;
		case RMIDBUTTON:
		break;
		default:
		break;
 	}
    return true;

 }

 
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include "macUtils.h"
#endif
 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif
 
#ifdef __cplusplus
	extern "C" {
#endif
 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
		int main(int argc, char *argv[])
#endif
		{
			// Create application object
			Cobalt app;
 
			try {
				app.go();
			} catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
				MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
				std::cerr << "An exception has occurred: " <<
					e.getFullDescription().c_str() << std::endl;
#endif
			}
 
			return 0;
		}
 
#ifdef __cplusplus
	}
#endif
