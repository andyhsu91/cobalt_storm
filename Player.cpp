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
        for (int i = 0; i < 11; ++i)
        {
                mCurrentControllerState[i] = 0;
        }

        Ogre::Vector3 shapeDim = Ogre::Vector3(20, 40, 20);
        Ogre::Vector3 position = Ogre::Vector3(700, 230, -700);

        Ogre::Entity* ent = mSceneMgr->createEntity("PlayerEntity","robot.mesh");
        pnode = mSceneMgr->getRootSceneNode()->
                createChildSceneNode(node, position);

        pnode->attachObject(ent);
        pnode->scale(.5, .5, .5);
        ent->setMaterialName("Examples/Robot");
        ent->setCastShadows(true);

        //mBullet->setKinematicCharacter(pnode, shapeDim, position, 250.0);
        //mBullet->setKinematicCharacter(pnode, shapeDim, position, 250.0);
        mBody = mBullet->setRigidBoxBody(pnode, shapeDim, position, 200.0, true, false);

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

        bullet = 0;

		//cerr << "Finishing init player" << endl;
}


void Player::updatePosition(const Ogre::FrameEvent& evt)
{

    mDirection.z = mCurrentControllerState[LCONTROLY]*200;
    mDirection.x = mCurrentControllerState[LCONTROLX]*200;


    //pnode->translate(mDirection * evt.timeSinceLastFrame, Ogre::Node::TS_WORLD);
    printf(" mCurrentControllerStateX: %f\n",mCurrentControllerState[LCONTROLX]);

    //trans = mGhost->getWorldTransform();
    //trans *= new btQuaternion(btVector3(0,1,0),0.01);

    /*btMatrix3x3 orn = mGhost->getWorldTransform().getBasis();
    orn *= btMatrix3x3(btQuaternion(btVector3(0,1,0),-0.01));
    mBody->getWorldTransform ().setBasis(orn);*/

    /*mBody->getMotionState()->getWorldTransform(trans);
    btVector3 ori = trans.getOrigin();
    btVector3 move = btVector3(mDirection.x * evt.timeSinceLastFrame, 0, mDirection.z * evt.timeSinceLastFrame);
    ori += move;
    trans.setOrigin(ori);
    mBody->getMotionState()->setWorldTransform(trans);
    pnode->translate(mDirection * evt.timeSinceLastFrame, Ogre::Node::TS_WORLD);
    printf(" mCurrentControllerStateX: %f\n",mCurrentControllerState[LCONTROLX]);
    printf(" mCurrentControllerStateY: %f\n",mCurrentControllerState[LCONTROLY]);*/

    pnode->translate(mDirection * evt.timeSinceLastFrame, Ogre::Node::TS_WORLD);
    Ogre::Vector3 pos = pnode->getPosition();
    trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
    Ogre::Quaternion qt = pnode->getOrientation();
    trans.setRotation(btQuaternion(qt.x, qt.y, qt.z, qt.w));
    mBody->getMotionState()->setWorldTransform(trans);

    if (mCurrentControllerState[RBUMP]) {
        Ogre::Vector3 shapeDim = Ogre::Vector3(10, 10, 10);
        Ogre::Vector3 position = Ogre::Vector3(pnode->getPosition().x, pnode->getPosition().y + 40, pnode->getPosition().z);

        Ogre::Entity* ent = mSceneMgr->createEntity("Bullet" + Ogre::StringConverter::toString(bullet),
                                                    "sphere.mesh");
        Ogre::SceneNode* bnode = mSceneMgr->getRootSceneNode()->
                createChildSceneNode("bNode" + Ogre::StringConverter::toString(bullet++), position);

        bnode->attachObject(ent);
        bnode->scale(.05, .05, .05);
        ent->setMaterialName("Examples/Chrome");
        ent->setCastShadows(true);
        mBullet->setRigidBoxBody(bnode, shapeDim, position, 50.0, false, true);

        mCurrentControllerState[RBUMP] = 0;
    }
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