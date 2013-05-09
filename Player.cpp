/*
-----------------------------------------------------------------------------
Filename:    Player.cpp
-----------------------------------------------------------------------------
*/

#include "Player.h"

using namespace std;
const float cameraRadius = 50.0; //how big the circle is that the camera orbits around the player
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
                Physics* Bullet, std::string entName, std::string node, bool isServer)
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
        Ogre::Vector3 position;
        if(isServer){
            position = Ogre::Vector3(700.0, 251.0, -750.0); //DO NOT SET Y POS TO 250.0!!!!!!
        } else{
            position = Ogre::Vector3(500.0, 251.0, -550.0); //DO NOT SET Y POS TO 250.0!!!!!!
        }

        Ogre::Entity* ent = mSceneMgr->createEntity(entName,"robot.mesh");
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

        lockedOn = false;

        cameraTarget = Ogre::Vector3(700, 250, -700);

		cout << "Finishing init player" << endl;

        distanceToTarget = 1;//Ogre::Math::Sqrt(Ogre::Math::Sqr(playerVector.x - cameraTarget.x) + Ogre::Math::Sqr(playerVector.z - cameraTarget.z));

        playerTargetCosTheta = 0;
        playerTargetSinTheta = 0;
}



Ogre::Vector3 Player::getPlayerPosition(void)
{
   return pnode->getPosition();
}

float Player::getDistanceToTarget(void)
{
   return distanceToTarget;
}

void Player::toggleLock(void){
    lockedOn = !lockedOn;
}

bool Player::getLockedOn(void)
{
    return lockedOn;
}

float Player::getPlayerTargetCosTheta(void)
{
    return playerTargetCosTheta;
}

float Player::getPlayerTargetSinTheta(void)
{
    return playerTargetSinTheta;
}

void Player::setCameraTarget(Ogre::Vector3 pos){

    cameraTarget = pos;
}

Ogre::Vector3 Player::getNewCameraPos(void){
    Ogre::Vector3 clientPos = cameraTarget;
    Ogre::Vector3 serverPos = getPlayerPosition();
    float zDist = clientPos.z-serverPos.z;
    float xDist = clientPos.x-serverPos.x;
    float distToTarget = sqrt( pow(xDist, 2.0) + pow(zDist, 2.0) );
    float scalingFactor = (cameraRadius/distToTarget);
    float slope = (clientPos.z - serverPos.z)/(clientPos.x - serverPos.x); //m=(y2-y1)/(x2-x1)
    float zIntercept = serverPos.z - slope*serverPos.x; //b = z - mx
    /*
        cout<<"serverPos:"<<serverPos<<endl;
        cout<<"clientPos:"<<clientPos<<endl;
        cout<<"ScalingFactor:"<<scalingFactor<<endl;
        cout<<"Dist:"<<distToTarget<<endl;
        cout<<"xDist:"<<xDist<<endl;
        cout<<"zDist:"<<zDist<<endl;
        cout<<"Camera X Pos:"<< serverPos.x + xDist*scalingFactor<<endl;
        cout<<"Camera Z Pos:"<<serverPos.z + zDist*scalingFactor<<endl;
    */
    if (distToTarget == 0.0) {
            distToTarget = .01; //stop divide by zero errors
        }

    Ogre::Vector3 newCameraPos = Ogre::Vector3(
            serverPos.x - xDist*scalingFactor,
            300,
            serverPos.z - zDist*scalingFactor);

    return newCameraPos;

}

Ogre::Vector3 Player::getCameraTarget(void)
{
    return cameraTarget;
}

void Player::updatePosition(const Ogre::FrameEvent& evt)
{   
    float distPerSec = 200;
    mDirection.x = mCurrentControllerState[LCONTROLX]*distPerSec;
    mDirection.z = mCurrentControllerState[LCONTROLY]*distPerSec;

    Ogre::Vector3 playerVector = getPlayerPosition();


    Ogre::Real diffX = playerVector.x - cameraTarget.x;
    Ogre::Real diffZ = playerVector.z - cameraTarget.z;
    distanceToTarget = Ogre::Math::Sqrt(Ogre::Math::Sqr(diffX) + Ogre::Math::Sqr(diffZ));

    //printf("mDirection.x * evt: %f\n", (mDirection.x*evt.timeSinceLastFrame));
    //printf("mDirection.z * evt: %f\n", (mDirection.z*evt.timeSinceLastFrame));

    

    if(lockedOn)
    {
        //playerTargetCosTheta = ((playerVector.x - cameraTarget.x)/distanceToTarget);
        //playerTargetSinTheta = ((playerVector.z - cameraTarget.z)/distanceToTarget);
        //TODO:: fix movement
        mDirection.x = 0.0;
        mDirection.z = 0.0;

        float scalingFactor = distPerSec/distanceToTarget;
        //towards and away from enemy
        mDirection.x += mCurrentControllerState[LCONTROLY]*diffX*scalingFactor;
        mDirection.z += mCurrentControllerState[LCONTROLY]*diffZ*scalingFactor;

        //left and right (Doesn't work yet)
        //Using http://math.stackexchange.com/questions/53875/calculating-point-around-circumference-of-circle-given-distance-travelled
        double pi = 3.14159265359;
        //double circumference = 2*pi*distanceToTarget;
        double d = distPerSec * evt.timeSinceLastFrame;
        double r = distanceToTarget;
        double theta = d/r;
        double a = cameraTarget.x;
        double b = cameraTarget.z;
        double x = playerVector.x;
        double y = playerVector.z;

        //mDirection.x += mCurrentControllerState[LCONTROLX]*(a+(x-a)*cos(theta)-(y-b)*sin(theta));
        //mDirection.z += mCurrentControllerState[LCONTROLX]*(b+(x-a)*sin(theta)+(y-b)*cos(theta));

        mDirection.x += mCurrentControllerState[LCONTROLX]*(a+(x-a)*cos(theta)-(y-b)*sin(theta));
        mDirection.z += mCurrentControllerState[LCONTROLX]*(b+(x-a)*sin(theta)+(y-b)*cos(theta));

    }

    //pnode->translate(mDirection * evt.timeSinceLastFrame, Ogre::Node::TS_WORLD);
    //printf(" mCurrentControllerStateX: %f\n",mCurrentControllerState[LCONTROLX]);
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
                createChildSceneNode("bnode" + Ogre::StringConverter::toString(bullet++), position);

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
                createChildSceneNode("bnode" + Ogre::StringConverter::toString(bullet++), position);

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

