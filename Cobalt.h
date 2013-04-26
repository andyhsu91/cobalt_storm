/*
-----------------------------------------------------------------------------
Filename:    Cobalt.h
-----------------------------------------------------------------------------
*/
#ifndef __Cobalt_h_
#define __Cobalt_h_
 
#include "BaseApplication.h"
#include "Environment.h"
#include "Physics.h"
 
class Cobalt : public BaseApplication
{
public:
	Cobalt(void);
	virtual ~Cobalt(void);
 
protected:
	virtual void createCamera(void);
	virtual void createScene(void);
	virtual void createFrameListener(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	
private:
	

};
 
#endif // #ifndef __Cobalt_h_
