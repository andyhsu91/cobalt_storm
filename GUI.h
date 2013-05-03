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
		void initGUI(void);

	protected:	
	
	private:
		CEGUI::Window *sheet;
		CEGUI::OgreRenderer* mRenderer;
		CEGUI::Window *startBut;
	
};

#endif // #ifndef __Environment_h_
