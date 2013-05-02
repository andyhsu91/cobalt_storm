/*
-----------------------------------------------------------------------------
Filename:    Player.cpp
-----------------------------------------------------------------------------
*/

#include "Player.h"

using namespace std;

//-------------------------------------------------------------------------------------
Player::Player(void)
{
}
//-------------------------------------------------------------------------------------
Player::~Player(void)
{
}
//-------------------------------------------------------------------------------------
void Player::initPlayer(Ogre::SceneManager* SceneMgr,
                Physics* Bullet, std::string node)
{
        //cerr << "Beginning init player" << endl;

        mSceneMgr = SceneMgr;
        mBullet = Bullet;
        mPlayerState = new PlayerVars;
        for (int i = 0; i < 5; ++i)
        {
                mCurrentControllerState[i] = 0;
        }

        Ogre::Vector3 shapeDim = Ogre::Vector3(50, 50, 50);
        Ogre::Vector3 position = Ogre::Vector3(700, 450, -750);

        Ogre::Entity* ent = mSceneMgr->createEntity("PlayerEntity","cube.mesh");
         pnode = mSceneMgr->getRootSceneNode()->
                createChildSceneNode(node, position);

        pnode->attachObject(ent);
        pnode->scale(.5, .5, .5);
        ent->setMaterialName("Examples/Chrome");
        ent->setCastShadows(true);

        mBullet->setRigidBoxBody(pnode, shapeDim, position, 250.0);

        mPlayerState->health = 100;
        mPlayerState->weaponamt1 = -1;
        mPlayerState->weaponamt2 = 10;
        mPlayerState->weaponamt3 = 2;

        for (int i = 0; i < sizeof(mPlayerState->playerState) / sizeof(bool); i++)
                mPlayerState->playerState[i] = false;
        mPlayerState->playerState[FALLING] = true;

        mPlayerState->playerPosition[X] = position.x;
        mPlayerState->playerPosition[Y] = position.y;
        mPlayerState->playerPosition[Z] = position.z;

        mDirection = Ogre::Vector3::ZERO;

		//cerr << "Finishing init player" << endl;
}


void Player::updatePosition(const Ogre::FrameEvent& evt)
{

mDirection.z = mCurrentControllerState[LCONTROLY]*250;
mDirection.x = mCurrentControllerState[LCONTROLX]*250;


pnode->translate(mDirection * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
printf(" mCurrentControllerStateX: %f\n",mCurrentControllerState[LCONTROLX]);
}

void Player::updateControlAxis(int axis, float value)
{
       // printf("updating %d to %f\n", axis ,value);
        mCurrentControllerState[axis]=value;
}

void Player::updatePlayerState(int state, bool value)
{
        mPlayerState->playerState[state]=value;
}