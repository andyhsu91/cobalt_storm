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

        Ogre::Vector3 shapeDim = Ogre::Vector3(5, 5, 5);
        Ogre::Vector3 position = Ogre::Vector3(700, 250, -750);

        Ogre::Entity* ent = mSceneMgr->createEntity("PlayerEntity","robot.mesh");
        pnode = mSceneMgr->getRootSceneNode()->
                createChildSceneNode(node, position);

        pnode->attachObject(ent);
        pnode->scale(.5, .5, .5);
        ent->setMaterialName("Examples/Robot");
        ent->setCastShadows(true);

        //mGhost = mBullet->setKinematicCharacter(pnode, shapeDim, position, 250.0);
        //mBullet->setKinematicCharacter(pnode, shapeDim, position, 250.0);
        mBody = mBullet->setRigidBoxBody(pnode, shapeDim, position, 250.0);
        mBody->setCollisionFlags(mBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);

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

    //trans = mGhost->getWorldTransform();
    //trans *= new btQuaternion(btVector3(0,1,0),0.01);

    /*btMatrix3x3 orn = mGhost->getWorldTransform().getBasis();
    orn *= btMatrix3x3(btQuaternion(btVector3(0,1,0),-0.01));
    mBody->getWorldTransform ().setBasis(orn);*/

    mBody->getMotionState()->getWorldTransform(trans);
    btVector3 ori = trans.getOrigin();
    btVector3 move = btVector3(mDirection.x * evt.timeSinceLastFrame, 0, mDirection.y * evt.timeSinceLastFrame);
    ori += move;
    trans.setOrigin(ori);
    mBody->getMotionState()->setWorldTransform(trans);
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