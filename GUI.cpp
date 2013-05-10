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
void GUI::initGUI(Ogre::SceneManager* pSceneMgr)
{
	//CEGUI init
	mSceneMgr = pSceneMgr;
	
	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	
	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	
	CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
}
void GUI::drawMenu()
{
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	mainMenu = wmgr.createWindow("DefaultWindow","CEGUIMainMenu/Menu");
	//Menu frame
	CEGUI::Window *frameMenu = wmgr.createWindow("TaharezLook/FrameWindow","CEGUI/MenuFrame");
	mainMenu->addChildWindow(frameMenu);
	frameMenu->setText("Main Menu");
	frameMenu->disable();
	frameMenu->setSize(CEGUI::UVector2(CEGUI::UDim(0.7, 0), CEGUI::UDim(0.7, 0)));
	frameMenu->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15,0),CEGUI::UDim(0.15, 0)));
	frameMenu->setAlpha(0.9f);

	//Start button
	CEGUI::Window *startBut = wmgr.createWindow("TaharezLook/Button", "CEGUI/StartButton");
	startBut->setText("Start");
	startBut->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	startBut->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05f, 0), CEGUI::UDim(0.9f, 0))); 	
	startBut->setAlpha(0.9f);	
	startBut->setProperty("NormalImage","set:TaharezLook image:full_image");
	frameMenu->addChildWindow(startBut);

	startBut->subscribeEvent(CEGUI::PushButton::EventClicked,
	CEGUI::Event::Subscriber(&GUI::startGame, this));

	//Settings button
	CEGUI::Window *settingBut = wmgr.createWindow("TaharezLook/Button", "CEGUI/SettingButton");
	settingBut->setText("Settings");
	settingBut->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	settingBut->setPosition(CEGUI::UVector2(CEGUI::UDim(0.65f, 0), CEGUI::UDim(0.9f, 0))); 	
	settingBut->setAlpha(0.9f);	
	frameMenu->addChildWindow(settingBut);

	//Quit button
	CEGUI::Window *quitBut = wmgr.createWindow("TaharezLook/Button", "CEGUI/QuitButton");
	quitBut->setText("Quit");
	quitBut->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	quitBut->setPosition(CEGUI::UVector2(CEGUI::UDim(0.82f, 0), CEGUI::UDim(0.9f, 0))); 	
	quitBut->setAlpha(0.9f);	
	frameMenu->addChildWindow(quitBut);

	CEGUI::System::getSingleton().setGUISheet(mainMenu);
}
//---------------------------------------------------------------------------
void GUI::drawHUD()
{
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	sheet = wmgr.createWindow("DefaultWindow", "CEGUIMainMenu/Sheet");
	//Create a health HUD
	CEGUI::Window *hud = wmgr.createWindow("TaharezLook/FrameWindow", "CEGUI/HudWindow");
	//adds the HUD to the sheet
	sheet->addChildWindow(hud);
	hud->setText("Health");
	hud->disable();
	hud->setSize(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.15, 0)));
	hud->setPosition(CEGUI::UVector2(CEGUI::UDim(0.62,0),CEGUI::UDim(0.82, 0)));
	hud->setAlpha(0.6f);

	//Create a time HUD
	CEGUI::Window *thud = wmgr.createWindow("TaharezLook/FrameWindow", "CEGUI/TimeHudWindow");
	//adds the Time HUD to the sheet
	sheet->addChildWindow(thud);
	thud->setText("Time");
	thud->disable();
	thud->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.1, 0)));
	thud->setPosition(CEGUI::UVector2(CEGUI::UDim(0.02,0),CEGUI::UDim(0.02, 0)));
	thud->setAlpha(0.8f);

	//Create a ammo HUD 1
	CEGUI::Window *ahud1 = wmgr.createWindow("TaharezLook/FrameWindow", "CEGUI/AmmoHudWindow1");
	//adds the HUD to the sheet
	sheet->addChildWindow(ahud1);
	ahud1->setText("Weapon 1");
	ahud1->disable();
	ahud1->setSize(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.05, 0)));
	ahud1->setPosition(CEGUI::UVector2(CEGUI::UDim(0.02,0),CEGUI::UDim(0.70, 0)));
	ahud1->setAlpha(0.6f);

	//Create a ammo HUD 2
	CEGUI::Window *ahud2 = wmgr.createWindow("TaharezLook/FrameWindow", "CEGUI/AmmoHudWindow2");
	//adds the HUD to the sheet
	sheet->addChildWindow(ahud2);
	ahud2->setText("Weapon 2");
	ahud2->disable();
	ahud2->setSize(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.05, 0)));
	ahud2->setPosition(CEGUI::UVector2(CEGUI::UDim(0.07,0),CEGUI::UDim(0.81, 0)));
	ahud2->setAlpha(0.6f);

	//Create a ammo HUD 3
	CEGUI::Window *ahud3 = wmgr.createWindow("TaharezLook/FrameWindow", "CEGUI/AmmoHudWindow3");
	//adds the HUD to the sheet
	sheet->addChildWindow(ahud3);
	ahud3->setText("Weapon 3");
	ahud3->disable();
	ahud3->setSize(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.05, 0)));
	ahud3->setPosition(CEGUI::UVector2(CEGUI::UDim(0.12,0),CEGUI::UDim(0.92, 0)));
	ahud3->setAlpha(0.6f);


	//Create a Health bar
	health = static_cast<CEGUI::ProgressBar*>(wmgr.createWindow("TaharezLook/ProgressBar", "CEGUI/Health"));
	//adds the health bar to the HUD
	hud->addChildWindow(health);
	health->disable();
	health->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.2, 0)));
	health->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1,0),CEGUI::UDim(0.2, 0)));
	health->setProgress(1.0);
	health->setAlwaysOnTop(true);
	health->setAlpha(1.7f);

	//Create a Health bar
	ehealth = static_cast<CEGUI::ProgressBar*>(wmgr.createWindow("TaharezLook/ProgressBar", "CEGUI/EnemyHealth"));
	//adds the health bar to the HUD
	hud->addChildWindow(ehealth);
	ehealth->disable();
	ehealth->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.2, 0)));
	ehealth->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1,0),CEGUI::UDim(0.5, 0)));
	ehealth->setProgress(1.0);
	ehealth->setAlwaysOnTop(true);
	ehealth->setAlpha(1.7f);

	//Creates an ammo-bar
	ammo1 = static_cast<CEGUI::ProgressBar*>(wmgr.createWindow("TaharezLook/ProgressBar", "CEGUI/Ammo1"));
	ahud1->addChildWindow(ammo1);
	//disable the possibility to interact with the object
	ammo1->disable();
	ammo1->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.6, 0)));
	ammo1->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1,0),CEGUI::UDim(0.1, 0)));
	ammo1->setStepSize(0.03);
	ammo1->setProgress(0.90);
	ammo1->setAlwaysOnTop(true);
	ammo1->setAlpha(1.7);

	//Creates an ammo-bar
	ammo2 = static_cast<CEGUI::ProgressBar*>(wmgr.createWindow("TaharezLook/ProgressBar", "CEGUI/Ammo2"));
	ahud2->addChildWindow(ammo2);
	//disable the possibility to interact with the object
	ammo2->disable();
	ammo2->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.6, 0)));
	ammo2->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1,0),CEGUI::UDim(0.1, 0)));
	ammo2->setStepSize(0.03);
	ammo2->setProgress(0.90);
	ammo2->setAlwaysOnTop(true);
	ammo2->setAlpha(1.7);

	//Creates an ammo-bar
	ammo3 = static_cast<CEGUI::ProgressBar*>(wmgr.createWindow("TaharezLook/ProgressBar", "CEGUI/Ammo3"));
	ahud3->addChildWindow(ammo3);
	//disable the possibility to interact with the object
	ammo3->disable();
	ammo3->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.6, 0)));
	ammo3->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1,0),CEGUI::UDim(0.1, 0)));
	ammo3->setStepSize(0.03);
	ammo3->setProgress(0.90);
	ammo3->setAlwaysOnTop(true);
	ammo3->setAlpha(1.7);

	CEGUI::Imageset MenuImageset = CEGUI::ImagesetManager::getSingleton().createFromImageFile 	("CrosshairIS","crosshair.png");
	MenuImageset.defineImage("crosshair", CEGUI::Point(0.0f,0.0f), CEGUI::Size( 1.0f, 1.0f ), CEGUI::Point(0.0f,0.0f)); // Whole Image

	//time
	timeDisplay = wmgr.createWindow("TaharezLook/StaticText", "CEGUI/TimeText");
	//adds the text to the Sheet
	thud->addChildWindow(timeDisplay);
	//disable the possibility to interact with the object
	timeDisplay->disable();
	timeDisplay->setProperty("FrameEnabled", "False");
	timeDisplay->setProperty("BackgroundEnabled", "False");
	timeDisplay->setProperty("HorzFormatting", "WordWrapCentred");
	timeDisplay->setText("0");
	timeDisplay->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.8, 0)));
	timeDisplay->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3,0),CEGUI::UDim(0.1, 0)));
	timeDisplay->setAlwaysOnTop(true);

	/*
	//crosshair
	CEGUI::Window* Crosshair = wmgr.createWindow("TaharezLook/StaticImage", "boogers");
	sheet->addChildWindow( Crosshair );
	Crosshair->setPosition( CEGUI::UVector2( CEGUI::UDim( 0.25f, 0.25f ), CEGUI::UDim( 0.25f, 0.25f) ) );
	Crosshair->setSize( CEGUI::UVector2( CEGUI::UDim( .5f, .5f ), CEGUI::UDim( .5f, .5f ) ) );
	Crosshair->setProperty("Image", "set:CrosshairIS image:crosshair");
*/
/*
	//Creates a "Ammo" text
	CEGUI::Window *ammoText = wmgr.createWindow("TaharezLook/StaticText", "CEGUI/AmmoText");
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
*/

	//Sets the current sheet to be used
	CEGUI::System::getSingleton().setGUISheet(sheet);
}
//---------------------------------------------------------------------------
void GUI::setHealth(float vhealth)
{
	this->health->setProgress(vhealth);
}
//---------------------------------------------------------------------------
void GUI::setEnemyHealth(float vhealth)
{
	this->ehealth->setProgress(vhealth);
}
//---------------------------------------------------------------------------
void GUI::setTime(Ogre::Real time)
{
	sprintf(timeString, "%.1f", time);
	this->timeDisplay->setText(timeString);
}
//---------------------------------------------------------------------------
void GUI::setAmmo(int ammo, int weapon)
{
	float tmp = ammo/100;
	switch(weapon){
		case 1:
			this->ammo1->setProgress(tmp);
			break;
		case 2:
			this->ammo2->setProgress(tmp);
			break;
		case 3:
			this->ammo3->setProgress(tmp);
			break;
		default:
			break;
	};
}
//---------------------------------------------------------------------------
bool GUI::startGame(const CEGUI::EventArgs &e)
{
	drawHUD();
	return true;
}
