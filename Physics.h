/*
-----------------------------------------------------------------------------
Filename:    Physics.h
-----------------------------------------------------------------------------
*/
#ifndef __Physics_h_
#define __Physics_h_

#include <btBulletDynamicsCommon.h>
#include <Ogre.h>
#include <btBulletCollisionCommon.h>
#include <stdio.h>

class Physics
{
public:
    Physics(void);
    virtual ~Physics(void);
	void initPhysics();
	void updateWorld(const Ogre::FrameEvent& evt);
	btRigidBody* setRigidBoxBody(Ogre::SceneNode *snode, 
		Ogre::Vector3 shapeDim, Ogre::Vector3 origin, double mass);
	void setBallRestitution(double restit);

private:
	Ogre::SceneManager* mSceneMgr;
	btDbvtBroadphase* overlappingPairCache;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	
	btCollisionShape* shape;
	btRigidBody* body;
	btTransform startTransform;
	
};

#endif // #ifndef __Physics_h_
