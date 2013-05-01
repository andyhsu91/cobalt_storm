/*
-----------------------------------------------------------------------------
Filename:    Environment.cpp
-----------------------------------------------------------------------------
*/

#include "Environment.h"

using namespace std;

//-------------------------------------------------------------------------------------
Environment::Environment(void) :
	mTerrainGlobals(0),
	mTerrainGroup(0),
	mTerrainsImported(false),
	mInfoLabel(0)
{
}
//-------------------------------------------------------------------------------------
Environment::~Environment(void)
{
}
//-------------------------------------------------------------------------------------
void Environment::initEnvironment(Ogre::SceneManager* SceneMgr, 
		Ogre::RenderWindow* mWindow, Physics* Bullet)
{	
	mSceneMgr = SceneMgr;
	mBullet = Bullet;
	
	Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
	Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(7);
 
	Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
	lightdir.normalise();
 
	Ogre::Light* light = mSceneMgr->createLight("tstLight");
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(lightdir);
	light->setDiffuseColour(Ogre::ColourValue::White);
	light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));
 
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.3, 0.3, 0.3));
 
	mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
 
	mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(mSceneMgr, Ogre::Terrain::ALIGN_X_Z, 513, 12000.0f);
	mTerrainGroup->setFilenameConvention(Ogre::String("GTFinal"), Ogre::String("dat"));
	mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);
 
	configureTerrainDefaults(light);
 
	for (long x = 0; x <= 0; ++x)
		for (long y = 0; y <= 0; ++y)
			defineTerrain(x, y);
 	
	// sync load since we want everything in place when we start
	mTerrainGroup->loadAllTerrains(true);
 
	if (mTerrainsImported)
	{
		Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
		while(ti.hasMoreElements())
		{
			Ogre::Terrain* t = ti.getNext()->instance;
			initBlendMaps(t);
		}
	}
 
	mTerrainGroup->freeTemporaryResources();
	
	{
		int dim = 1000;
		
    	//Water-----------------------------------------------------------------------
    	{   
    		Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
 
	   		Ogre::MeshManager::getSingleton().createPlane("base", 
	    		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	    	    plane, 10000, 10000, 100, 100, true, 1, 10, 10, Ogre::Vector3::UNIT_X);
    	
    		Ogre::Vector3 shapeDim = Ogre::Vector3(dim, 1, dim);
    		Ogre::Vector3 position = Ogre::Vector3(0, 200, 0);
    		    			    
			Ogre::Entity* ent = mSceneMgr->createEntity("WaterEntity", "base");
    		Ogre::SceneNode* snode = mSceneMgr->getRootSceneNode()->
    			createChildSceneNode("snode0", position);
 			    		
 			snode->attachObject(ent);
    		ent->setMaterialName("Examples/WaterStream");
    		ent->setCastShadows(false);
    	}
    	//Ground----------------------------------------------------------------------
    	{   
	    	//Ogre::Vector3 shapeDim = Ogre::Vector3(dim/100, 1, dim/100);
		Ogre::Vector3 shapeDim = Ogre::Vector3(500, 10, 500);
    		Ogre::Vector3 position = Ogre::Vector3(700, 225, -700);

		//Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create("RustedMetal", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		//Ogre::TextureUnitState* tuisTexture = mat->getTechnique(0)->getPass(0)->createTextureUnitState("RustedMetal.jpg");

		//Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
		//Ogre::MeshManager::getSingleton().createPlane("platform",
                //        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                //    plane, 1000, 1000, 20, 20, true, 1, 10, 10, Ogre::Vector3::UNIT_Z);
	
		//Ogre::Entity* ent = mSceneMgr->createEntity("GroundEntity", "cube.mesh");
		Ogre::Entity* ent = mSceneMgr->createEntity("GroundEntity", "cube.mesh");
    		Ogre::SceneNode* snode = mSceneMgr->getRootSceneNode()->
    			createChildSceneNode("snode1", position);
    		
 		snode->attachObject(ent);
 		//snode->scale(shapeDim.x, .01, shapeDim.z);
		snode->scale(5, .1, 5);
	 		//snode->translate(position);
    		//ent->setMaterialName("Examples/Rocky");
		ent->setMaterialName("Examples/RustySteel");
    		ent->setCastShadows(true);

		mBullet->setRigidBoxBody(snode, shapeDim, position, 0.0);
    	}
        //Dynamic--------------------------------------------------------------------
        {
		//Ogre::Vector3 shapeDim = Ogre::Vector3(.5, .5, .5);
		Ogre::Vector3 shapeDim = Ogre::Vector3(10, 10, 10);
		Ogre::Vector3 position = Ogre::Vector3(700, 500, -700);

		Ogre::Entity* ent = mSceneMgr->createEntity("DynamicEntity","cube.mesh");
		Ogre::SceneNode* snode = mSceneMgr->getRootSceneNode()->
			createChildSceneNode("snode2", position);

		snode->attachObject(ent);
		//snode->scale(shapeDim.x, shapeDim.y, shapeDim.z);
		snode->scale(.1, .1, .1);
		ent->setMaterialName("Examples/Rocky");
		ent->setCastShadows(true);

		mBullet->setRigidBoxBody(snode, shapeDim, position, 100.0);
	}
	}
	
	Ogre::ColourValue fadeColour(0.9, 0.9, 0.9);
	//Ogre::ColourValue fadeColour(0.2, 0.2, 0.2);
	mSceneMgr->setFog(Ogre::FOG_LINEAR, fadeColour, 0.0, 10, 5000);
	mWindow->getViewport(0)->setBackgroundColour(fadeColour);
	
	Ogre::Plane plane;
	plane.d = 100;
	plane.normal = Ogre::Vector3::NEGATIVE_UNIT_Y;
 
	//mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8, 500);
	mSceneMgr->setSkyPlane(true, plane, "Examples/CloudySky", 500, 75, true, 0.5, 150, 150);
	
}
//-------------------------------------------------------------------------------------
void Environment::frmqUpdate(const Ogre::FrameEvent& evt, OgreBites::SdkTrayManager* mTrayMgr)
{
	if (mTerrainGroup->isDerivedDataUpdateInProgress())
	{
		mTrayMgr->moveWidgetToTray(mInfoLabel, OgreBites::TL_TOP, 0);
		mInfoLabel->show();
		if (mTerrainsImported)
		{
			mInfoLabel->setCaption("Building terrain, please wait...");
		}
		else
		{
			mInfoLabel->setCaption("Updating textures, patience...");
		}
	}
	else
	{
		mTrayMgr->removeWidgetFromTray(mInfoLabel);
		mInfoLabel->hide();
		if (mTerrainsImported)
		{
			mTerrainGroup->saveAllTerrains(true);
			mTerrainsImported = false;
		}
	}
}
//-------------------------------------------------------------------------------------
void Environment::createFL(OgreBites::SdkTrayManager* mTrayMgr)
{
	mInfoLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "TInfo", "", 350);
}
//-------------------------------------------------------------------------------------
void Environment::configureTerrainDefaults(Ogre::Light* light)
{
	// Configure global
	mTerrainGlobals->setMaxPixelError(8);
	// testing composite map
	mTerrainGlobals->setCompositeMapDistance(3000);
 
	// Important to set these so that the terrain knows what to use for derived (non-realtime) data
	mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
	mTerrainGlobals->setCompositeMapAmbient(mSceneMgr->getAmbientLight());
	mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());
 
	// Configure default import settings for if we use imported image
	Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
	defaultimp.terrainSize = 513;
	defaultimp.worldSize = 12000.0f;
	defaultimp.inputScale = 600;
	defaultimp.minBatchSize = 33;
	defaultimp.maxBatchSize = 65;

/*	// textures
	defaultimp.layerList.resize(3);
	defaultimp.layerList[0].worldSize = 100;
	defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
	defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
	defaultimp.layerList[1].worldSize = 30;
	defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
	defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
	defaultimp.layerList[2].worldSize = 200;
	defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
	defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
*/
}
//-------------------------------------------------------------------------------------
void Environment::defineTerrain(long x, long y)
{
	Ogre::String filename = mTerrainGroup->generateFilename(x, y);
	if (Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename))
	{
		mTerrainGroup->defineTerrain(x, y);
	}
	else
	{
		Ogre::Image img;
		getTerrainImage(x % 2 != 0, y % 2 != 0, img);
		mTerrainGroup->defineTerrain(x, y, &img);
		mTerrainsImported = true;
	}
}
//-------------------------------------------------------------------------------------
void Environment::getTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
{
	img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	if (flipX)
		img.flipAroundY();
	if (flipY)
		img.flipAroundX();
 
}
//-------------------------------------------------------------------------------------
void Environment::initBlendMaps(Ogre::Terrain* terrain)
{
	Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
	Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
	Ogre::Real minHeight0 = 70;
	Ogre::Real fadeDist0 = 40;
	Ogre::Real minHeight1 = 70;
	Ogre::Real fadeDist1 = 15;
	float* pBlend0 = blendMap0->getBlendPointer();
	float* pBlend1 = blendMap1->getBlendPointer();
	for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
	{
		for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
		{
			Ogre::Real tx, ty;
 
			blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
			Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
			Ogre::Real val = (height - minHeight0) / fadeDist0;
			val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
			*pBlend0++ = val;

			val = (height - minHeight1) / fadeDist1;
			val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
			*pBlend1++ = val;
		}
	}
	blendMap0->dirty();
	blendMap1->dirty();
	blendMap0->update();
	blendMap1->update();
}
//-------------------------------------------------------------------------------------
void Environment::destroyScene(void)
{
    OGRE_DELETE mTerrainGroup;
    OGRE_DELETE mTerrainGlobals;
}
