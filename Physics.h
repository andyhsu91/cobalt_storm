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
#include "math.h"


class btKinematicCharacterController;

using namespace std;

class Physics
{
public:
    Physics(void);
    virtual ~Physics(void);
    enum bodyTypes { type1, type2, temp, other, rigid, kinematic};
	void initPhysics(Ogre::SceneManager* SceneMgr);
	void DestroyAllAttachedMovableObjects( Ogre::SceneNode* i_pSceneNode );
	void updateWorld(const Ogre::FrameEvent& evt);
	btRigidBody* setRigidBoxBody(Ogre::SceneNode *snode, 
		Ogre::Vector3 shapeDim, Ogre::Vector3 origin, double mass, bool kino);
	btRigidBody* createBullet(Ogre::SceneNode *snode,
		int weapon_type, Ogre::Vector3 origin, Ogre::Vector3 direction);
	btPairCachingGhostObject* setKinematicCharacter(Ogre::SceneNode *snode,
		          Ogre::Vector3 shapeDim, Ogre::Vector3 origin, double mass);
	//btGhostObject* setKinematicCharacter(Ogre::SceneNode *snode,
    //            Ogre::Vector3 shapeDim, Ogre::Vector3 origin, double mass);
	void setBallRestitution(double restit);

	void putProjectiles(float projectiles[][3], int typeOfProjectile); 
	void freeProjectiles(float** projectiles);
	float** getProjectiles(int typeOfProjectile);
	int damageToPlayer(bool isServer);


private:
	Ogre::SceneManager* mSceneMgr;
	btDbvtBroadphase* overlappingPairCache;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	float type1ProjectilePos[20][3]; //20 projectiles, 3 axises for each projectile
	float type2ProjectilePos[20][3];
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	btCollisionShape* shape;
	btRigidBody* body;
	btTransform startTransform;
	int bullet;
};

#endif // #ifndef __Physics_h_
