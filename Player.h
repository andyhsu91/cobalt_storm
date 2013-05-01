/*
-----------------------------------------------------------------------------
Filename:    Player.h
-----------------------------------------------------------------------------
*/
#ifndef __Player_h_
#define __Player_h_

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <Ogre.h>
#include <stdio.h>
#include "Physics.h"

struct PlayerVars {
	int health;
	int weaponamt1;
	int weaponamt2;
	int weaponamt3;
	
	bool playerState[8];
	float playerPosition[3];
	

	//Need to add projectile positions to client
	//All projectile directions processed on server
	float serverProjectilePos[3][3]; //3 projectiles, 3 axises for each projectile
	float serverProjectileDir[3][3];
	float clientProjectilePos[3][3];
	float clientProjectileDir[3][3];

};


class Player
{
public:
    Player(Ogre::SceneManager* pSceneMgr, Physics* sim, std::string node);
    ~Player(void);
	btRigidBody* getRigidBody(void);
	void updatePosition(const Ogre::FrameEvent& evt);
	void updatePosition(const Ogre::FrameEvent& evt, PlayerVars* update);
	PlayerVars* getPlayerState(void);

private:
	Ogre::SceneManager* mSceneMgr;
	Physics* mBullet;
	btRigidBody* mPlayer;
	btTransform trans;
	
	PlayerVars* mPlayerState;
	bool forceUpdate;
};

#endif // #ifndef __Player_h_
