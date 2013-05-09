/*
-----------------------------------------------------------------------------
Filename:    GUI.h
-----------------------------------------------------------------------------
*/
#ifndef __GUI_h_
#define __GUI_h_

#include <Ogre.h>
#include <stdio.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>

#include "BaseApplication.h"

class GUI
{
	public:
    	GUI(void);
    	virtual ~GUI(void);
		void initGUI(Ogre::SceneManager*);
		void setHealth(float);
		void setEnemyHealth(float);
		void setAmmo(int, int);
		void setTime(Ogre::Real);

	protected:	
	
	private:
		char timeString[65];
		CEGUI::Window *timeDisplay;
		CEGUI::OgreRenderer* mRenderer;
		CEGUI::Window *sheet;
		CEGUI::ProgressBar *health;
		CEGUI::ProgressBar *ehealth;
		CEGUI::ProgressBar *ammo1;
		CEGUI::ProgressBar *ammo2;
		CEGUI::ProgressBar *ammo3;

		Ogre::SceneManager* mSceneMgr;
	
};

#endif
