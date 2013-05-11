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
void GUI::initGUI(Ogre::SceneManager* pSceneMgr, bool *pGamepaused, bool *pGamequit, bool *inMenu)
{
	//CEGUI init
	mSceneMgr = pSceneMgr;
	isPaused = pGamepaused;
	mShutDown = pGamequit;
	inMainMenu = inMenu;
	
	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	
	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	
	CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");

	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	mainMenu = wmgr.createWindow("DefaultWindow","CEGUIMainMenu/Menu");
	sheet = wmgr.createWindow("DefaultWindow", "CEGUIMainMenu/Sheet");
	pauseMenu = wmgr.createWindow("DefaultWindow","CEGUIMainMenu/PauseMenu");
	victoryScreen = wmgr.createWindow("DefaultWindow","CEGUIMainMenu/VictoryScreen");
	defeatScreen = wmgr.createWindow("DefaultWindow","CEGUIMainMenu/DefeatScreen");
	drawHUD();
	drawPause();
	drawVictory();
	drawDefeat();
}
//---------------------------------------------------------------------------
void GUI::drawMenu(void)
{
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
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
	startBut->setPosition(CEGUI::UVector2(CEGUI::UDim(0.18f, 0), CEGUI::UDim(0.78f, 0))); 	
	startBut->setAlpha(0.9f);	
	//startBut->setProperty("NormalImage","set:TaharezLook image:full_image");
	startBut->setAlwaysOnTop(true);
	mainMenu->addChildWindow(startBut);

	startBut->subscribeEvent(CEGUI::PushButton::EventClicked,
	CEGUI::Event::Subscriber(&GUI::startGame, this));

	//Settings button
	CEGUI::Window *settingBut = wmgr.createWindow("TaharezLook/Button", "CEGUI/SettingButton");
	settingBut->setText("Settings");
	settingBut->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	settingBut->setPosition(CEGUI::UVector2(CEGUI::UDim(0.52f, 0), CEGUI::UDim(0.78f, 0))); 
	//settingBut->setProperty("NormalImage","set:TaharezLook image:full_image");	
	settingBut->setAlpha(0.9f);	
	settingBut->setAlwaysOnTop(true);
	mainMenu->addChildWindow(settingBut);

	//Quit button
	CEGUI::Window *quitBut = wmgr.createWindow("TaharezLook/Button", "CEGUI/QuitButton");
	quitBut->setText("Quit");
	quitBut->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	quitBut->setPosition(CEGUI::UVector2(CEGUI::UDim(0.68f, 0), CEGUI::UDim(0.78f, 0))); 	
	//quitBut->setProperty("NormalImage","set:TaharezLook image:full_image");
	quitBut->setAlpha(0.9f);	
	quitBut->setAlwaysOnTop(true);
	quitBut->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&GUI::quitGame, this));
	mainMenu->addChildWindow(quitBut);

	CEGUI::System::getSingleton().setGUISheet(mainMenu);
	CEGUI::MouseCursor::getSingleton().show(); 
}
//---------------------------------------------------------------------------
void GUI::drawPause(void)
{
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	//Menu frame
	CEGUI::Window *pauseBG = wmgr.createWindow("TaharezLook/FrameWindow","CEGUI/PauseBackground");
	pauseMenu->addChildWindow(pauseBG);
	pauseBG->setText("Pause");
	pauseBG->disable();
	pauseBG->setSize(CEGUI::UVector2(CEGUI::UDim(2.0, 0), CEGUI::UDim(2.0, 0)));
	pauseBG->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0),CEGUI::UDim(0, 0)));
	pauseBG->setAlpha(0.7f);

	//Start button
	CEGUI::Window *resumeBut = wmgr.createWindow("TaharezLook/Button", "CEGUI/ResumeButton");
	resumeBut->setText("Resume");
	resumeBut->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	resumeBut->setPosition(CEGUI::UVector2(CEGUI::UDim(0.18f, 0), CEGUI::UDim(0.78f, 0))); 	
	resumeBut->setAlpha(0.9f);	
	//resumeBut->setProperty("NormalImage","set:TaharezLook image:full_image");
	resumeBut->setAlwaysOnTop(true);
	pauseMenu->addChildWindow(resumeBut);

	resumeBut->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&GUI::resumeGame, this));

	//Settings button
	CEGUI::Window *settingPauseBut = wmgr.createWindow("TaharezLook/Button", "CEGUI/PauseSettingButton");
	settingPauseBut->setText("Settings");
	settingPauseBut->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	settingPauseBut->setPosition(CEGUI::UVector2(CEGUI::UDim(0.52f, 0), CEGUI::UDim(0.78f, 0))); 
	//settingPauseBut->setProperty("NormalImage","set:TaharezLook image:full_image");	
	settingPauseBut->setAlpha(0.9f);	
	settingPauseBut->setAlwaysOnTop(true);
	pauseMenu->addChildWindow(settingPauseBut);

	//Quit button
	CEGUI::Window *quitPauseBut = wmgr.createWindow("TaharezLook/Button", "CEGUI/PauseQuitButton");
	quitPauseBut->setText("Quit");
	quitPauseBut->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	quitPauseBut->setPosition(CEGUI::UVector2(CEGUI::UDim(0.68f, 0), CEGUI::UDim(0.78f, 0))); 	
	//quitPauseBut->setProperty("NormalImage","set:TaharezLook image:full_image");
	quitPauseBut->setAlpha(0.9f);	
	quitPauseBut->setAlwaysOnTop(true);
	quitPauseBut->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&GUI::quitGame, this));
	pauseMenu->addChildWindow(quitPauseBut); 

	CEGUI::System::getSingleton().setGUISheet(mainMenu);
}
//---------------------------------------------------------------------------
void GUI::drawHUD()
{
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	pauseMenu = wmgr.createWindow("DefaultWindow", "CEGUIMainMenu/Pause");
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

	//Sets the current sheet to be used
	CEGUI::System::getSingleton().setGUISheet(sheet);

	CEGUI::MouseCursor::getSingleton().hide(); 
}
//---------------------------------------------------------------------------
void GUI::drawVictory(void)
{
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	//Menu frame
	CEGUI::Window *victoryFrame = wmgr.createWindow("TaharezLook/FrameWindow","CEGUI/VictoryFrame");
	victoryScreen->addChildWindow(victoryFrame);
	victoryFrame->setText("Victory");
	victoryFrame->disable();
	victoryFrame->setSize(CEGUI::UVector2(CEGUI::UDim(0.7, 0), CEGUI::UDim(0.7, 0)));
	victoryFrame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15,0),CEGUI::UDim(0.15, 0)));
	victoryFrame->setAlpha(0.9f);

	//Start button
	CEGUI::Window *OKBut = wmgr.createWindow("TaharezLook/Button", "CEGUI/OKButton");
	OKBut->setText("OK");
	OKBut->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	OKBut->setPosition(CEGUI::UVector2(CEGUI::UDim(0.18f, 0), CEGUI::UDim(0.78f, 0))); 	
	OKBut->setAlpha(0.9f);	
	//OKBut->setProperty("NormalImage","set:TaharezLook image:full_image");
	OKBut->setAlwaysOnTop(true);
	OKBut->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&GUI::resumeGame, this));
	victoryScreen->addChildWindow(OKBut);

	//Win message
	CEGUI::Window *winMessage = wmgr.createWindow("TaharezLook/StaticText", "CEGUI/WinMessage");
	//adds the text to the Sheet
	victoryScreen->addChildWindow(winMessage);
	//disable the possibility to interact with the object
	winMessage->disable();
	winMessage->setProperty("FrameEnabled", "False");
	winMessage->setProperty("BackgroundEnabled", "False");
	winMessage->setProperty("HorzFormatting", "WordWrapCentred");
	winMessage->setText("Good job, you win.");
	winMessage->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.5, 0)));
	winMessage->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3,0),CEGUI::UDim(0.1, 0)));
	winMessage->setAlwaysOnTop(true);
}
//---------------------------------------------------------------------------
void GUI::drawDefeat(void)
{
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	//Menu frame
	CEGUI::Window *defeatFrame = wmgr.createWindow("TaharezLook/FrameWindow","CEGUI/DefeatFrame");
	defeatScreen->addChildWindow(defeatFrame);
	defeatFrame->setText("Defeat");
	defeatFrame->disable();
	defeatFrame->setSize(CEGUI::UVector2(CEGUI::UDim(0.7, 0), CEGUI::UDim(0.7, 0)));
	defeatFrame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15,0),CEGUI::UDim(0.15, 0)));
	defeatFrame->setAlpha(0.9f);

	//Start button
	CEGUI::Window *OKBut = wmgr.createWindow("TaharezLook/Button", "CEGUI/OKButton2");
	OKBut->setText("OK");
	OKBut->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	OKBut->setPosition(CEGUI::UVector2(CEGUI::UDim(0.18f, 0), CEGUI::UDim(0.78f, 0))); 	
	OKBut->setAlpha(0.9f);	
	//OKBut->setProperty("NormalImage","set:TaharezLook image:full_image");
	OKBut->setAlwaysOnTop(true);
	OKBut->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&GUI::resumeGame, this));
	defeatScreen->addChildWindow(OKBut);

	//Win message
	CEGUI::Window *loseMessage = wmgr.createWindow("TaharezLook/StaticText", "CEGUI/LoseMessage");
	//adds the text to the Sheet
	defeatScreen->addChildWindow(loseMessage);
	//disable the possibility to interact with the object
	loseMessage->disable();
	loseMessage->setProperty("FrameEnabled", "False");
	loseMessage->setProperty("BackgroundEnabled", "False");
	loseMessage->setProperty("HorzFormatting", "WordWrapCentred");
	loseMessage->setText("You suck, you lose.");
	loseMessage->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.5, 0)));
	loseMessage->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3,0),CEGUI::UDim(0.1, 0)));
	loseMessage->setAlwaysOnTop(true);
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
	CEGUI::System::getSingleton().setGUISheet(sheet);
	CEGUI::MouseCursor::getSingleton().hide();
	*inMainMenu = false;
	*isPaused = false;
	return true;
}
bool GUI::resumeGame(const CEGUI::EventArgs &e)
{
	std::cout<<"Game Resumed"<<std::endl;
	CEGUI::System::getSingleton().setGUISheet(sheet);
	*isPaused = false;
	CEGUI::MouseCursor::getSingleton().hide();
	return true;
}
bool GUI::quitGame(const CEGUI::EventArgs &e)
{
    *mShutDown = true;
    return true;
}
void GUI::pauseGame(void)
{
	std::cout<<"Game Paused"<<std::endl;
	CEGUI::MouseCursor::getSingleton().show(); 
	CEGUI::System::getSingleton().setGUISheet(pauseMenu);
	*isPaused = true;
}
void GUI::resumeGame(void)
{
	std::cout<<"Game Resumed"<<std::endl;
	CEGUI::System::getSingleton().setGUISheet(sheet);
	*isPaused = false;
	CEGUI::MouseCursor::getSingleton().hide();
}
void GUI::showVictory(void)
{
	CEGUI::MouseCursor::getSingleton().show(); 
	CEGUI::System::getSingleton().setGUISheet(victoryScreen);
	*isPaused = true;
}
void GUI::showDefeat(void)
{
	CEGUI::MouseCursor::getSingleton().show(); 
	CEGUI::System::getSingleton().setGUISheet(defeatScreen);
	*isPaused = true;
}
