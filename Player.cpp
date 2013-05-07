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
        Ogre::Vector3 position = Ogre::Vector3(700, 230, -750);

        Ogre::Entity* ent = mSceneMgr->createEntity("PlayerEntity","robot.mesh");
        pnode = mSceneMgr->getRootSceneNode()->
                createChildSceneNode(node, position);

        pnode->attachObject(ent);
        pnode->scale(.5, .5, .5);
        ent->setMaterialName("Examples/Robot");
        ent->setCastShadows(true);

        //mBullet->setKinematicCharacter(pnode, shapeDim, position, 250.0);
        mBody = mBullet->setRigidBoxBody(pnode, shapeDim, position, 5000.0, true);

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

        mLook = Ogre::Vector3(1.0, 0.0, 0.0);

        bullet = 0;

		cout << "Finishing init player" << endl;
}


Ogre::Vector3 Player::getPlayerPosition(void)
{
   return pnode->getPosition();
}

void Player::updatePosition(const Ogre::FrameEvent& evt)
{

    mDirection.z = mCurrentControllerState[LCONTROLY]*200;
    mDirection.x = mCurrentControllerState[LCONTROLX]*200;


    //pnode->translate(mDirection * evt.timeSinceLastFrame, Ogre::Node::TS_WORLD);
    printf(" mCurrentControllerStateX: %f\n",mCurrentControllerState[LCONTROLX]);
    //pnode->translate(mDirection * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
    //printf(" mCurrentControllerStateX: %f\n",mCurrentControllerState[LCONTROLX]);

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
        Ogre::Vector3 position = Ogre::Vector3(pnode->getPosition().x+10, pnode->getPosition().y+20, pnode->getPosition().z+10);

        Ogre::Entity* ent = mSceneMgr->createEntity("Bullet" + Ogre::StringConverter::toString(bullet),
                                                    "sphere.mesh");
        Ogre::SceneNode* bnode = mSceneMgr->getRootSceneNode()->
                createChildSceneNode("bNode" + Ogre::StringConverter::toString(bullet++), position);

        bnode->attachObject(ent);
        bnode->scale(.03, .03, .03);
        Ogre::MaterialPtr bMat = ent->getSubEntity(0)->getMaterial()->clone("newBallColor");
        bMat->getTechnique(0)->getPass(0)->setAmbient(1.0, 0.8, 0.0);
        bMat->getTechnique(0)->getPass(0)->setDiffuse(1.0, 0.8, 0.0, 1.0);
        ent->setMaterialName(bMat->getName());
        // ent->setMaterialName("Examples/Chrome");
        ent->setCastShadows(true);
        
        mBullet->createBullet(bnode, 1, position, mLook);

        mCurrentControllerState[RBUMP] = 0;
    }
    if (mCurrentControllerState[LBUMP]) {
        Ogre::Vector3 position = Ogre::Vector3(pnode->getPosition().x+10, pnode->getPosition().y+20, pnode->getPosition().z-10);

        Ogre::Entity* ent = mSceneMgr->createEntity("Bullet" + Ogre::StringConverter::toString(bullet),
                                                    "sphere.mesh");
        Ogre::SceneNode* bnode = mSceneMgr->getRootSceneNode()->
                createChildSceneNode("bNode" + Ogre::StringConverter::toString(bullet++), position);

        bnode->attachObject(ent);
        bnode->scale(.07, .07, .07);
        Ogre::MaterialPtr gMat = ent->getSubEntity(0)->getMaterial()->clone("newBombColor");
        gMat->getTechnique(0)->getPass(0)->setAmbient(0.2, 0.8, 0.2);
        gMat->getTechnique(0)->getPass(0)->setDiffuse(0.1, 0.8, 0.2, 1.0);
        ent->setMaterialName(gMat->getName());
        // ent->setMaterialName("Examples/Chrome");
        ent->setCastShadows(true);
        
        mBullet->createBullet(bnode, 2, position, mLook);

        mCurrentControllerState[LBUMP] = 0;
    }
    mPlayerState->playerPosition[X] = pnode->getPosition().x;
    mPlayerState->playerPosition[Y] = pnode->getPosition().y;
    mPlayerState->playerPosition[Z] = pnode->getPosition().z;

    // mBody->applyDamping(evt.timeSinceLastFrame);
    //printf("getPOSX %f \n",mPlayerState->playerPosition[X]);
    //printf(" mCurrentControllerStateX: %f\n",mCurrentControllerState[LCONTROLX]);
    //printf(" mCurrentControllerStateY: %f\n",mCurrentControllerState[LCONTROLY]);
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

