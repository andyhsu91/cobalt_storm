/*
-----------------------------------------------------------------------------
Filename:    GUI.H
-----------------------------------------------------------------------------
*/

#include <Ogre.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <string>
#include "Player.h"

using namespace std;

class GUI 
{
	public:
		GUI(void);
		~GUI(void);
		void initGUI(Ogre::SceneManager* pSceneMgr);
		void setHealth(float health);
		void setAmmo(int ammo);
	protected:
		CEGUI::Window *sheet;
		CEGUI::WindowManager *win;
		CEGUI::Window *si;
	private:	
		Ogre::SceneManager* mSceneMgr;
		CEGUI::ProgressBar *health;
		CEGUI::ProgressBar *ammo;
		Ogre::RenderWindow* mGUIWindow;
		Ogre::SceneManager* mSceneManager;
		CEGUI::OgreCEGUIRenderer* mGUIRenderer;
		CEGUI::System* mGUISystem;
		bool rendering;		
		Ogre::Root* ogreRoot;
		WinSettings mWinSettings;
		scenedata::SceneModelEvent::VisObjectsListT flipVisibiltyObjects;
};
