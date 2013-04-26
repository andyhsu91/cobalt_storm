/*
-----------------------------------------------------------------------------
Filename:    Cobalt.cpp
-----------------------------------------------------------------------------
*/
 
#include "Cobalt.h"

using namespace std;

static Physics mBullet;
static Environment mEnv;
 
//-------------------------------------------------------------------------------------
Cobalt::Cobalt(void)
{
}
//-------------------------------------------------------------------------------------
Cobalt::~Cobalt(void)
{
}
//-------------------------------------------------------------------------------------
void Cobalt::createCamera(void)
{
	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");
	
	mCamera->setPosition(Ogre::Vector3(700, 400, -700));
	mCamera->lookAt(Ogre::Vector3(1963, 50, 1660));
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
	// create your scene here :)
	cerr << "Initing Bullet" << endl;	
	mBullet.initPhysics();
	
	cerr << "Initing Env" << endl;
	mEnv.initEnvironment(mSceneMgr, mWindow, &mBullet);
	
	
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
{
    bool ret = BaseApplication::frameRenderingQueued(evt);
    
    mBullet.updateWorld(evt);
    mEnv.frmqUpdate(evt, mTrayMgr);
    return ret;
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
