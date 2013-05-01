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

#define STANDING 0
#define WALKING  1
#define RUNNING  2
#define DASHING  3
#define JUMPING  4
#define SHOOTING 5
#define FALLING  6
#define MELEEING 7
#define HIT      8
#define DOWN     9
#define DEATH    10

#define X        0
#define Y        1
#define Z        2

struct PlayerVars {
	int health;
	int weaponamt1;
	int weaponamt2;
	int weaponamt3;
	
	bool playerState[15];
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
	Player();
    ~Player(void);
    void initPlayer(Ogre::SceneManager* mSceneMgr,
    	Physics* mBullet, std::string node);
	btRigidBody* getRigidBody(void);
	void updatePosition(const Ogre::FrameEvent& evt);
	void updatePosition(const Ogre::FrameEvent& evt, PlayerVars* update);
	PlayerVars* getPlayerState(void);

private:
	Ogre::SceneManager* mSceneMgr;
	Physics* mBullet;
	btKinematicCharacterController* mPlayer;
	btTransform trans;
	
	PlayerVars* mPlayerState;
	bool forceUpdate;
};

#endif // #ifndef __Player_h_
