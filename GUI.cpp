/*
-----------------------------------------------------------------------------
Filename:    GUI.cpp
-----------------------------------------------------------------------------
*/

#include "GUI.h"
using namespace std;

//-------------------------------------------------------------------------------------
GUI::GUI(void)
{
}
//-------------------------------------------------------------------------------------
GUI::~GUI(void)
{
}
//-------------------------------------------------------------------------------------
void GUI::initGUI()
{	
	using namespace CEGUI;
	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
 
	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
 
	CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();

	sheet = wmgr.createWindow("DefaultWindow", "CEGUIMainMenu/Sheet");

	startBut = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/StartButton");
	startBut->setText("START");
	startBut->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	startBut->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45f, 0), CEGUI::UDim(0.45f, 0))); 	


	DefaultWindow* healthDisplay = static_cast<DefaultWindow*>(wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/HealthText"));
	healthDisplay->setText("Health : ");
	healthDisplay->setProperty("TextColours", "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000");
	healthDisplay->setProperty("VertFormatting", "TopAligned");
	healthDisplay->setProperty("HorzFormatting", "LeftAligned"); 
	
	
}
//-------------------------------------------------------------------------------------

