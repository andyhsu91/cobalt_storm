/*
-----------------------------------------------------------------------------
Filename:    GUI.cpp
-----------------------------------------------------------------------------
*/
 
#include "GUI.h"

using namespace std;

//---------------------------------------------------------------------------
GUI::GUI(void)
{
}
//---------------------------------------------------------------------------
GUI::~GUI(void)
{
}
//---------------------------------------------------------------------------
GUI::initGUI(Ogre::SceneManager* pSceneMgr)
{
	//CEGUI init
	mSceneMgr = pSceneMgr;
	
	mSceneMgr->setupRenderSystem();
	mSceneMgr->createRenderWindow();
	mGUIWindow = mSceneMgr->getRenderWin();

	mGUIRenderer = new CEGUI::OgreCEGUIRenderer(mGUIWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mSceneMgr);
	// create the root CEGUI class
    mGUISystem = new CEGUI::System(mGUIRenderer);
	// tell us a lot about what is going on (see CEGUI.log in the working directory)
	CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);

	//Selects the skin
	CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");
	//Select default mouse cursor and default font
	mGUISystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseTarget");
	mGUISystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");
	CEGUI::MouseCursor::getSingleton().setImage(CEGUI::System::getSingleton().getDefaultMouseCursor());
	//Create a default window
	win = CEGUI::WindowManager::getSingletonPtr();
    sheet = win->createWindow("DefaultGUISheet", "CEGUI/Sheet");

	//Create a quit-button
	CEGUI::Window *quit = win->createWindow("TaharezLook/Button", "CEGUI/QuitButton");
    quit->setText("Quit");
    quit->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	//attach the button to the sheet and the current sheet to our sheet
	sheet->addChildWindow(quit);

	//Create a HUD
	CEGUI::Window *hud = win->createWindow("TaharezLook/FrameWindow", "CEGUI/HudWindow");
	//adds the HUD to the sheet
	sheet->addChildWindow(hud);
	hud->setText("HUD");
	hud->disable();
	hud->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(0.2, 0)));
	hud->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0),CEGUI::UDim(0.8, 0)));

	//Create a Health bar
	health = static_cast<CEGUI::ProgressBar*>(win->createWindow("TaharezLook/ProgressBar", "CEGUI/Health"));
	//adds the health bar to the HUD
	hud->addChildWindow(health);
	health->disable();
	health->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.4, 0)));
	health->setPosition(CEGUI::UVector2(CEGUI::UDim(0.02,0),CEGUI::UDim(0.4, 0)));
	health->setProgress(1.0);
	health->setAlwaysOnTop(true);

	//Creates a "Health" text
	CEGUI::Window *healthText = win->createWindow("TaharezLook/StaticText", "CEGUI/HealthText");
	//adds the text to the HUD
	sheet->addChildWindow(healthText);
	//disable the possibility to interact with the object
	healthText->disable();
	healthText->setProperty("FrameEnabled", "False");
	healthText->setProperty("BackgroundEnabled", "False");
	healthText->setProperty("HorzFormatting", "WordWrapCentred");
	healthText->setText("Health");
	healthText->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.2, 0)));
	healthText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.07,0),CEGUI::UDim(0.76, 0)));
	healthText->setAlwaysOnTop(true);

	//Creates an ammo-bar
	ammo = static_cast<CEGUI::ProgressBar*>(win->createWindow("TaharezLook/ProgressBar", "CEGUI/Ammo"));
	hud->addChildWindow(ammo);
	//disable the possibility to interact with the object
	ammo->disable();
	ammo->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.4, 0)));
	ammo->setPosition(CEGUI::UVector2(CEGUI::UDim(0.60,0),CEGUI::UDim(0.4, 0)));
	ammo->setStepSize(0.03);
	ammo->setProgress(0.90);
	ammo->setAlwaysOnTop(true);

	//Creates a "Ammo" text
	CEGUI::Window *ammoText = win->createWindow("TaharezLook/StaticText", "CEGUI/AmmoText");
	//adds the text to the Sheet
	sheet->addChildWindow(ammoText);
	//disable the possibility to interact with the object
	ammoText->disable();
	ammoText->setProperty("FrameEnabled", "False");
	ammoText->setProperty("BackgroundEnabled", "False");
	ammoText->setProperty("HorzFormatting", "WordWrapCentred");
	ammoText->setText("Ammo");
	ammoText->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.2, 0)));
	ammoText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.65,0),CEGUI::UDim(0.76, 0)));
	ammoText->setAlwaysOnTop(true);

	//Creates a radar/mini-map
	CEGUI::Imageset* radar = CEGUI::ImagesetManager::getSingletonPtr()->createImagesetFromImageFile("radar","Radar.jpg");
	CEGUI::Window *si = win->createWindow((CEGUI::utf8*)"TaharezLook/StaticImage", "R2TWindow");
    si->setSize(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.2f, 0)));
    si->setPosition(CEGUI::UVector2(CEGUI::UDim(0.8f, 0), CEGUI::UDim(0, 0)));
	si->setProperty("Image","set:radar image:full_image" );
	sheet->addChildWindow(si); 

	//Sets the current sheet to be used
	mGUISystem->setGUISheet(sheet);
}
//---------------------------------------------------------------------------
void GUI::setHealth(float health)
{
	mSceneMgr->health->setProgress(health);
}
//---------------------------------------------------------------------------
void GUI::setAmmo(int ammo)
{
	float tmp = ammo/100;
	mSceneMgr->ammo->setProgress(tmp);
}
//---------------------------------------------------------------------------
