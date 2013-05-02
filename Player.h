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

#define LCONTROLX 0
#define LCONTROLY 1
#define RCONTROLX 3
#define RCONTROLY 4

#define LTRIG 2
#define RTRIG 2

#define LBUMP 4
#define RBUMP 5

#define LJOYCLICK 8
#define RJOYCLICK 9

#define RBUTTON1 0
#define RBUTTON2 1
#define RBUTTON3 2
#define RBUTTON4 3

#define LMIDBUTTON 6
#define RMIDBUTTON 7

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

	void updateControlAxis(int axis, float value);
	void updatePlayerState(int action, bool value);

	PlayerVars* getPlayerState(void);

private:
	Ogre::SceneManager* mSceneMgr;
	Physics* mBullet;
	btKinematicCharacterController* mPlayer;
	btTransform trans;
	
	PlayerVars* mPlayerState;
	CurrentControllerState* mCurrentControls;
	bool forceUpdate;

	/*holds the current controller/keyboardstate of the player
	these values will then be used to update the player movement based on
	frametime in the updatePosition method. LCONTROLX, LCONTROLY, RCONTROLX, RCONTROLY*/
	float mCurrentControllerState[4];

};

#endif // #ifndef __Player_h_
