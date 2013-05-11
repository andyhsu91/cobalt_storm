/*
-----------------------------------------------------------------------------
Filename:    Cobalt.cpp
-----------------------------------------------------------------------------
*/
 
#include "Cobalt.h"

using namespace std;

static Physics mBullet;
static Environment mEnv;
static GUI mGUI;
Player mPlayer;
//static ControlManager CtrlManager;
 bool isMultiplayer;
 bool isConnected;
 bool isServer;
 bool isPaused = true;
 bool menuCam = true;
 Network* nManager;
 Sound* sManager;
 const float timeLimit = 60.0;
 long numPacketsReceived = 0;
float UnlockedCameraMovementSpeed= 80;


//-------------------------------------------------------------------------------------
Cobalt::Cobalt(void)
{
//0 is the Server
	serverPlayer = new Player();
	clientPlayer = new Player();
}
//-------------------------------------------------------------------------------------
Cobalt::~Cobalt(void)
{
	if(nManager)	{delete nManager;}
	if(sManager)	{delete sManager;}
	if(gameUpdate)	{delete gameUpdate;}
}
//-------------------------------------------------------------------------------------
void Cobalt::createCamera(void)
{
	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");
	
	mCamera->setPosition(Ogre::Vector3(700, 500, -300));
	mCamera->lookAt(Ogre::Vector3(0.0, 250, -1000));
	mCamera->setNearClipDistance(0.1);
	mCamera->setFarClipDistance(50000);

	

	if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
	{
		mCamera->setFarClipDistance(0);   // enable infinite far clip distance if we can
	}	
 	
	mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}
//-------------------------------------------------------------------------------------
void Cobalt::createScene(void)
{
	timeElapsed = 0;
	// create your scene here :)
	cerr << "Initing Bullet" << endl;	
	mBullet.initPhysics(mSceneMgr);
	
	cerr << "Initing Env" << endl;
	mEnv.initEnvironment(mSceneMgr, mWindow, &mBullet);
	
	cerr << "Initing GUI" << endl;
	mGUI.initGUI(mSceneMgr, &isPaused, &mShutDown);
	mGUI.drawMenu();

	//Initialize Network Manager
	nManager = new Network();
	nManager->init();
	isConnected = nManager->isConnectionOpen();
	isServer = nManager->isThisServer();
	
	isMultiplayer = true; //change to true when ready

	if(isMultiplayer && isServer && !isConnected){
		nManager->waitForClientConnection();
		isConnected = nManager->isConnectionOpen();
	}
	if(isConnected){
		gameUpdate = new PlayerVars;
		isServer = nManager->isThisServer();
	} else{
		isMultiplayer = false;
	}
	cout<<"Current State: isConnected="<<boolalpha<<isConnected<<", isMutliplayer="<<isMultiplayer<<", isServer="<<isServer<<endl;
	sManager = new Sound();
	sManager->playBackground(-1);
	cerr << "Initing Player" << endl;
    serverPlayer->initPlayer(mSceneMgr, &mBullet, sManager, "PlayerEntity", "pnode", true);
    clientPlayer->initPlayer(mSceneMgr, &mBullet, sManager, "PlayerEntity2", "pnode2", false);

    if(isConnected && !isServer){
    	myself = clientPlayer;
    	myPos = &clientPos;
    	enemy = serverPlayer;
    	enemyPos = &serverPos;
    } else{
    	myself = serverPlayer;
    	myPos = &serverPos;
    	enemy = clientPlayer;
    	enemyPos = &clientPos;
    }

	cerr << "Finished scene" << endl;	

}


//-------------------------------------------------------------------------------------
void Cobalt::createFrameListener(void)
{
	BaseApplication::createFrameListener();

	mEnv.createFL(mTrayMgr);
	
}

PlayerVars* Cobalt::createPacket(void){
	//go to various other classes and fill in packet data
	
	//send my position
	gameUpdate->playerPosition[0] = myPos->x;
	gameUpdate->playerPosition[1] = myPos->y;
	gameUpdate->playerPosition[2] = myPos->z;

	PlayerVars* myPlayerVars = myself->getPlayerVars(); 
	gameUpdate->shootTimeRemaining = myPlayerVars->shootTimeRemaining;
	gameUpdate->server_health = myPlayerVars->server_health;
	gameUpdate->client_health = myPlayerVars->client_health;
	//send my animation states
	for (int i = 0; i < Player::animEnumCount; i++){
		gameUpdate->animationStateEnabled[i] = myPlayerVars->animationStateEnabled[i];
		gameUpdate->animationStateLoop[i] = myPlayerVars->animationStateLoop[i];
	}

	float** type1 = mBullet.getProjectiles(1);
	float** type2 = mBullet.getProjectiles(2);

	// type1ProjectilePos[20][3]; //20 projectiles, 3 axises for each projectile
	// type2ProjectilePos[20][3];
	for(int i=0; i<20; i++){
		for(int j=0; j<3; j++){
			gameUpdate->type1ProjectilePos[i][j] = type1[i][j];
			gameUpdate->type2ProjectilePos[i][j] = type2[i][j];
		}
	}

	mBullet.freeProjectiles(type1);
	mBullet.freeProjectiles(type2);

	if(isServer)
		gameUpdate->client_health -= mBullet.damageToPlayer(isServer);
	else
		gameUpdate->server_health -= mBullet.damageToPlayer(isServer);

	return gameUpdate;
}

//-------------------------------------------------------------------------------------
bool Cobalt::frameRenderingQueued(const Ogre::FrameEvent& evt)
{	//return True to continue rendering, false to drop out of the rendering loop
	
	bool ret = BaseApplication::frameRenderingQueued(evt);
	mEnv.frmqUpdate(evt, mTrayMgr);
	if(!isPaused)
	{
		
		timeElapsed += evt.timeSinceLastFrame;
		mGUI.setTime(timeLimit-timeElapsed);

		*myPos = myself->getPlayerPosition();
		*enemyPos = enemy->getPlayerPosition();

		myself->updatePosition(evt); //update myself normally
		myself->setCameraTarget(*enemyPos); //tell player class enemy position
		enemy->setCameraTarget(*myPos);
		
		mBullet.updateWorld(evt);

		
		


		if(myself->getLockedOn())
		{
			cameraPos = myself->getNewCameraPos();
			mCamera->setPosition(cameraPos);
			mCamera->lookAt(*enemyPos);

		}else
		{



			mCamera->yaw( Ogre::Degree(  -myself->getCurrentAxisState(RCONTROLX)
				*UnlockedCameraMovementSpeed*evt.timeSinceLastFrame));
			mCamera->pitch( Ogre::Degree( -myself->getCurrentAxisState(RCONTROLY)
				*UnlockedCameraMovementSpeed*evt.timeSinceLastFrame));

			myself->setCameraOrientation(mCamera->getOrientation());
			cameraPos = myself->getNewCameraPos();
			mCamera->setPosition(cameraPos);
			   
		}

		if(isMultiplayer)
		{
			bool packetReceived = nManager->checkForPackets(); //check for game updates and connection closed packets
			isConnected = nManager->isConnectionOpen();
			PlayerVars* gameUpdate = NULL;
			if(!isConnected){ mShutDown = true; return false; } //opponent closed connection
			if(packetReceived){ 
				gameUpdate = nManager->getGameUpdate(); 
				numPacketsReceived++;
				enemy->updatePositionFromPacket(evt, gameUpdate);
				mBullet.removeTempProjectiles();
				mBullet.putProjectiles(gameUpdate->type1ProjectilePos, 1);
				mBullet.putProjectiles(gameUpdate->type2ProjectilePos, 2);
			}



			if(packetReceived || numPacketsReceived < 3){
				//only send packet when we receive a packet so that we don't congest the network
				nManager->sendPacket( *createPacket() );
			}

			if(isServer){
				//I am server
				

			}else{
				//I am client
				if(packetReceived)
				{
					
				}
			}
		}
		else
		{
			//single player mode
			
			

		

		}
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
			myself->updateControlAxis(LCONTROLY, -1);
		}
	else if (arg.key == OIS::KC_A)
        {
			myself->updateControlAxis(LCONTROLX, -1);
        }
	else if (arg.key == OIS::KC_S)
        {
			myself->updateControlAxis(LCONTROLY, 1);
        }
	else if (arg.key == OIS::KC_D)
        {
			myself->updateControlAxis(LCONTROLX, 1);
        }
    else if (arg.key == OIS::KC_O)
        {

			myself->updateControlButton(RBUMP, 1);
        }
    else if (arg.key == OIS::KC_P)
    	{
    		myself->updateControlButton(LBUMP, 1);
    	}
     else if (arg.key == OIS::KC_L)
    	{
    		myself->updateControlButton(RJOYCLICK,1);
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
			myself->updateControlAxis(LCONTROLY, 0);
		}
	else if (arg.key == OIS::KC_A)
        {
			myself->updateControlAxis(LCONTROLX, 0);
        }
	else if (arg.key == OIS::KC_S)
        {
			myself->updateControlAxis(LCONTROLY, 0);
        }
	else if (arg.key == OIS::KC_D)
        {
			myself->updateControlAxis(LCONTROLX, 0);
        }
    else if (arg.key == OIS::KC_O)
        {

			myself->updateControlButton(RBUMP, 0);
        }
    else if (arg.key == OIS::KC_P)
        {
			myself->updateControlButton(LBUMP, 0);
        }
//mCameraMan->injectKeyUp(arg);
	if(CEGUI::System::getSingleton().injectKeyUp(arg.key)) return true;
    	mCameraMan->injectKeyUp(arg);
	return true;
}
//-------------------------------------------------------------------------------------
CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;
        break;
 
    case OIS::MB_Right:
        return CEGUI::RightButton;
        break;
 
    case OIS::MB_Middle:
        return CEGUI::MiddleButton;
        break;
 
    default:
        return CEGUI::LeftButton;
        break;
    }
}
//-------------------------------------------------------------------------------------
bool Cobalt::mouseMoved( const OIS::MouseEvent &evt )
{
	if (isPaused)
	{
		if(CEGUI::System::getSingleton().injectMouseMove(evt.state.X.rel, evt.state.Y.rel)) return true;
		mCameraMan->injectMouseMove(evt);
	}
	else
	{
		if (mTrayMgr->injectMouseMove(evt)) return true;
		mCameraMan->injectMouseMove(evt);
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool Cobalt::mousePressed( const OIS::MouseEvent &evt, OIS::MouseButtonID id )
{
	if (isPaused)
	{
		if(CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id))) return true;
	}
	else{
		if (mTrayMgr->injectMouseDown(evt, id)) return true;
		mCameraMan->injectMouseDown(evt, id);
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool Cobalt::mouseReleased( const OIS::MouseEvent &evt, OIS::MouseButtonID id )
{
	if (isPaused)
	{
		if(CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id))) return true;
	}
	else
	{
    	if (mTrayMgr->injectMouseUp(evt, id)) return true;
		mCameraMan->injectMouseUp(evt, id);
	}
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
	 	myself->updateControlAxis(axis, axisValue);

    return true;
 }

/*the different buttons are LBUMP,RBUMP, LJOYCLICK, RJOYCLICK
 RBUTTON1, RBUTTON2, RBUTTON3, RBUTTON4, LMIDBUTTON, RMIDBUTTON */
 bool Cobalt::buttonPressed( const OIS::JoyStickEvent &e, int button )
 {

 	myself->updateControlButton(button, true);
	
 	return true;
 }
 bool Cobalt::buttonReleased( const OIS::JoyStickEvent &e, int button )
 {
		myself->updatePlayerState(button, false);

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
