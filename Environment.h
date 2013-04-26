/*
-----------------------------------------------------------------------------
Filename:    Environment.h
-----------------------------------------------------------------------------
*/
#ifndef __Environment_h_
#define __Environment_h_

#include <Ogre.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include <stdio.h>

#include "BaseApplication.h"
#include "Physics.h"

class Environment
{
	public:
    	Environment(void);
    	virtual ~Environment(void);
		void initEnvironment(Ogre::SceneManager* mSceneMgr, 
				Ogre::RenderWindow* mWindow, Physics* mBullet);
		void destroyScene(void);
		void frmqUpdate(const Ogre::FrameEvent& evt, OgreBites::SdkTrayManager* mTrayMgr);
		void createFL(OgreBites::SdkTrayManager* mTrayMgr);

	protected:	
	
	private:
		Ogre::SceneManager* mSceneMgr;
		Physics *mBullet;
		
		Ogre::TerrainGlobalOptions* mTerrainGlobals;
		Ogre::TerrainGroup* mTerrainGroup;
		bool mTerrainsImported;
		OgreBites::Label* mInfoLabel;
 
		void defineTerrain(long x, long y);
		void initBlendMaps(Ogre::Terrain* terrain);
		void configureTerrainDefaults(Ogre::Light* light);
		void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img);
	
};

#endif // #ifndef __Environment_h_
