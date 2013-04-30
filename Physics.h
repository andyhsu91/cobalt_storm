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
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include <stdio.h>
#include "BulletCollision/CollisionShapes/btConvexHullShape.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"

class btKinematicCharacterController;

class Physics
{
public:
    Physics(void);
    virtual ~Physics(void);
	void initPhysics();
	void updateWorld(const Ogre::FrameEvent& evt);
	btRigidBody* setRigidBoxBody(Ogre::SceneNode *snode, 
		Ogre::Vector3 shapeDim, Ogre::Vector3 origin, double mass);
	btPairCachingGhostObject* setKinematicCharacter(Ogre::SceneNode *snode,
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
