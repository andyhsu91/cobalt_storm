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

        Ogre::Vector3 shapeDim = Ogre::Vector3(50, 50, 50);
        Ogre::Vector3 position = Ogre::Vector3(700, 450, -750);

        Ogre::Entity* ent = mSceneMgr->createEntity("PlayerEntity","cube.mesh");
        Ogre::SceneNode* pnode = mSceneMgr->getRootSceneNode()->
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

		//cerr << "Finishing init player" << endl;
}