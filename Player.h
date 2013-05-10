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
#include "Sound.h"

enum States { STANDING,
 WALKING,
 RUNNING,
 DASHING,
 JUMPING,
 SHOOTING1,
 SHOOTING2,
 SHOOTING3,
 FALLING,
 MELEEING,
 HIT, 
 DOWN,
 DEATH};

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
	//Upper limit of this struct is 1500 bytes
	int health;
	int weaponamt1;
	int weaponamt2;
	int weaponamt3;
	
	bool playerState[15];
	float playerPosition[3];
	float timeRemaining;
	

	//Need to add projectile positions to client
	//All projectile directions processed on server
	float type1ProjectilePos[20][3]; //20 projectiles, 3 axises for each projectile
	float type2ProjectilePos[20][3];
	//float serverProjectileDir[3][3];
	//float clientProjectilePos[3][3];
	//float clientProjectileDir[3][3];

};




class Player
{
public:
	Player();
    ~Player(void);
    enum robotStates { Die, Idle, Shoot, Slump, Walk, animEnumCount }; //animEnumCount should always be last
    void initPlayer(Ogre::SceneManager* mSceneMgr,
    	Physics* mBullet, Sound* soundManager, std::string entName, std::string node, bool isServer);
	btRigidBody* getRigidBody(void);
	void updatePosition(const Ogre::FrameEvent& evt);
	void updatePositionFromPacket(const Ogre::FrameEvent& evt, PlayerVars* packet);

	void updateControlAxis(int axis, float value);
	void updateControlButton(int button, float value);
	void updatePlayerState(int action, bool value);

	Ogre::Vector3 getPlayerPosition(void);

	float getCurrentAxisState(int axis);
	bool getCurrentButtonState(int button);

	bool getPlayerState(int);
	float getDistanceToTarget(void);
	Ogre::Vector3 getCameraTarget(void);
	void toggleLock(void);
	bool getLockedOn(void);
	void setCameraTarget(Ogre::Vector3 pos);
	Ogre::Vector3 getNewCameraPos(void);
	float getPlayerTargetCosTheta(void);
	float getPlayerTargetSinTheta(void);
	std::string getStringFromEnum(int animStateEnum);
	void enableState(int animStateEnum, bool enabled, bool loop);
	void updateAnimation(int animStateEnum, double seconds);
	void attack(bool val);

private:
	Ogre::Entity* ent;
	Ogre::SceneManager* mSceneMgr;
	Physics* mBullet;
	Sound* sManager;
	btKinematicCharacterController* mPlayer;
	bool stateActive[animEnumCount];
	//btPairCachingGhostObject* mGhost;
	btRigidBody* mBody;
	btTransform trans;
	
	Ogre::SceneNode* pnode;
	Ogre::Vector3 mDirection;
	Ogre::Vector3 mLook;
	PlayerVars* mPlayerState;
	bool forceUpdate;
	int bullet;
	bool lockedOn;
	
	float playerTargetCosTheta;
    float playerTargetSinTheta;
	Ogre::Vector3 cameraTarget;

    Ogre::Real distanceToTarget;
    

	/*holds the current controller/keyboardstate of the player
	these values will then be used to update the player movement based on
	frametime in the updatePosition method. LCONTROLX, LCONTROLY, RCONTROLX, RCONTROLY*/
	float mCurrentControllerAxisState[5];
	bool mCurrentControllerButtonState[11];

};

#endif // #ifndef __Player_h_
