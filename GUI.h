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
		void initGUI(Ogre::SceneManager*, bool*, bool*, bool*);
		void setHealth(float);
		void setEnemyHealth(float);
		void setAmmo(int, int);
		void setTime(Ogre::Real);
		void drawMenu(void);
		void drawHUD(void);
		void drawPause(void);
		bool openSettingsMenu(const CEGUI::EventArgs &e);
		bool startGame(const CEGUI::EventArgs &e);
		bool quitGame(const CEGUI::EventArgs &e);
		bool resumeGame(const CEGUI::EventArgs &e);
		void resumeGame(void);
		void pauseGame(void);

	protected:	
	
	private:
		char timeString[65];
		CEGUI::Window *timeDisplay;
		CEGUI::OgreRenderer* mRenderer;
		CEGUI::Window *sheet;
		CEGUI::Window *mainMenu;
		CEGUI::Window *pauseMenu;
		CEGUI::ProgressBar *health;
		CEGUI::ProgressBar *ehealth;
		CEGUI::ProgressBar *ammo1;
		CEGUI::ProgressBar *ammo2;
		CEGUI::ProgressBar *ammo3;
		bool *isPaused;
		bool *mShutDown;
		bool *inMainMenu;

		Ogre::SceneManager* mSceneMgr;
	
};

#endif
