/*
-----------------------------------------------------------------------------
Filename:    Player.cpp
-----------------------------------------------------------------------------
*/

#include "Player.h"

using namespace std;
const float cameraRadius = 50.0; //how big the circle is that the camera orbits around the player
const float shootTimeout = 2.0/3.0; //amount of seconds that the shooting animation takes
float shootTimeRemaining = 0.0; //shoot animation has to be set to loop in order to repeat the animation multiple times
const float walkTime = .85;
float walkTimeRemaining = 0.0;





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
                Physics* Bullet, Sound* soundManager, std::string entName, std::string node, bool isServer)
{
        //cerr << "Beginning init player" << endl;

        mSceneMgr = SceneMgr;
        mBullet = Bullet;
        sManager = soundManager;
        mPlayerState = new PlayerVars;
        for (int i = 0; i < 5; ++i)
        {
                mCurrentControllerState[i] = 0;
        }

        Ogre::Vector3 shapeDim = Ogre::Vector3(20, 40, 20);
        Ogre::Vector3 position;
        if(isServer){
            position = Ogre::Vector3(700.0, 242.0, -750.0);
        } else{
            position = Ogre::Vector3(500.0, 242.0, -550.0);
        }

        ent = mSceneMgr->createEntity(entName,"robot.mesh");
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

        lockedOn = true;

        cameraTarget = Ogre::Vector3(0, 250, -1000);

		cout << "Finishing init player" << endl;

        distanceToTarget = 1;//Ogre::Math::Sqrt(Ogre::Math::Sqr(playerVector.x - cameraTarget.x) + Ogre::Math::Sqr(playerVector.z - cameraTarget.z));

        playerTargetCosTheta = 0;
        playerTargetSinTheta = 0;
}

std::string Player::getStringFromEnum(int animStateEnum)
{
    //enum robotStates { Die, Idle, Shoot, Slump, Walk };
  switch (animStateEnum)
  {
    case Die:       return "Die";
    case Idle:      return "Idle";
    case Shoot:     return "Shoot";
    case Slump:     return "Slump";
    case Walk:      return "Walk";
    default:        return "Invalid";
    
  };
}

void Player::enableState(int animStateEnum, bool enabled, bool loop){
    cout<<"AnimState: "<<getStringFromEnum(animStateEnum)<<", enabled: "<<boolalpha<<enabled<<", loop: "<<loop<<endl;
    for(int i=0; i<animEnumCount; i++){
        stateActive[i]=false;
        std::string animationState = getStringFromEnum(i);
        if(animationState != "Invalid"){
            ent->getAnimationState(animationState)->setEnabled(false);
            ent->getAnimationState(animationState)->setLoop(false);
        }
    }
    
    std::string animationState = getStringFromEnum(animStateEnum);
    if(animationState != "Invalid"){
        stateActive[animStateEnum]=enabled;
        ent->getAnimationState(animationState)->setLoop(loop);
        ent->getAnimationState(animationState)->setEnabled(enabled);
    }
}

void Player::updateAnimation(int animStateEnum, double seconds){
    std::string animationState = getStringFromEnum(animStateEnum);
    if(animationState != "Invalid"){
        ent->getAnimationState(animationState)->addTime(seconds);
    }
}

void Player::attack(bool val){
    std::cout<<"Attacking"<<std::endl;
    shootTimeRemaining = shootTimeout;
    enableState(Shoot, val, true);
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
    mLook = cameraTarget - getPlayerPosition();
    mLook.y = 0.0;
    mLook.normalise();

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

float Player::getCurrentAxis(int axis)
{
    return mCurrentControllerState[axis];
}

Ogre::Vector3 Player::getCameraTarget(void)
{
    return cameraTarget;
}

void Player::updatePosition(const Ogre::FrameEvent& evt)
{   

    printf("playervars: %d \n",sizeof (PlayerVars) );
    float distPerSec = 200;
    mDirection = Ogre::Vector3(0.0,0.0,0.0);
    
/*
    const float walkTime = 2.0;
    float walkTimeRemaining = 0.0;
*/
    if(mCurrentControllerState[LCONTROLX] != 0.0){
        
        if(stateActive[Walk]==false){
            enableState(Walk, true, true);
        }
        
        mDirection.x = mCurrentControllerState[LCONTROLX]*distPerSec;
    }
    if(mCurrentControllerState[LCONTROLY] != 0.0){
        if(stateActive[Walk]==false){
            enableState(Walk, true, true);
        }
        mDirection.z = mCurrentControllerState[LCONTROLY]*distPerSec;
    }
    if(mCurrentControllerState[LCONTROLY] == 0.0 && mCurrentControllerState[LCONTROLX] == 0.0){
        if(stateActive[Walk]==true){
            enableState(Walk, false, false);
        }
    }
    if(mCurrentControllerState[LCONTROLY] != 0.0 || mCurrentControllerState[LCONTROLX] != 0.0){
        if(walkTimeRemaining <=0.0){
            walkTimeRemaining = walkTime;
            sManager->playSoundFromEnum(Sound::Walk);
        }
        walkTimeRemaining -= evt.timeSinceLastFrame;
    }
    Ogre::Vector3 playerVector = getPlayerPosition();

    double a = cameraTarget.x;
    double b = cameraTarget.z;
    double x = playerVector.x;
    double y = playerVector.z;
    double r = distanceToTarget;

    Ogre::Real diffX = x - a;
    Ogre::Real diffZ = y - b;
    distanceToTarget = Ogre::Math::Sqrt(Ogre::Math::Sqr(diffX) + Ogre::Math::Sqr(diffZ));

    double facingAngle = acos(diffX/r); //radians of rotation from x axis to line segment connecting players where opponent is at origin
    if(lockedOn)
    {
        mDirection.x = 0.0;
        mDirection.z = 0.0;
        float scalingFactor = distPerSec/distanceToTarget;

        //towards and away from enemy
        mDirection.x += mCurrentControllerState[LCONTROLY]*diffX*scalingFactor;
        mDirection.z += mCurrentControllerState[LCONTROLY]*diffZ*scalingFactor;

        //left and right (Doesn't work yet)
        //Using http://math.stackexchange.com/questions/53875/calculating-point-around-circumference-of-circle-given-distance-travelled
        double pi = 3.14159265359;
        double circumference = 2*pi*distanceToTarget;
        double d = distPerSec * evt.timeSinceLastFrame*50.0;
        
        double theta = d/r;

        double newXCoord = (a+(x-a)*cos(theta)-(y-b)*sin(theta)); //raw x-z coordinate point
        double newYCoord = (b+(x-a)*sin(theta)+(y-b)*cos(theta)); //raw x-z coordinate point
        mDirection.x -= mCurrentControllerState[LCONTROLX]*(newXCoord-x); //amount of translation needed
        mDirection.z -= mCurrentControllerState[LCONTROLX]*(newYCoord-y); //amount of translation needed

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
    pnode->lookAt(cameraTarget, Ogre::Node::TS_WORLD, Ogre::Vector3::UNIT_X);
    Ogre::Vector3 pos = pnode->getPosition();
    trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
    Ogre::Quaternion qt = pnode->getOrientation();
    trans.setRotation(btQuaternion(qt.x, qt.y, qt.z, qt.w));
    mBody->getMotionState()->setWorldTransform(trans);

    if (mCurrentControllerState[RBUMP]) {
        //small and fast projectile
        attack(true);
        sManager->playSoundFromEnum(Sound::Shoot1);
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
        //large and slow projectile
        attack(true);
        sManager->playSoundFromEnum(Sound::Shoot2);
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

    for(int i=0; i<animEnumCount; i++){
        if(stateActive[i]==true){
            if(i==Shoot){
                if(shootTimeRemaining>0.0){
                    updateAnimation(i, evt.timeSinceLastFrame);
                    shootTimeRemaining-=evt.timeSinceLastFrame;
                }

            } else{
                 updateAnimation(i, evt.timeSinceLastFrame);
            }
        }
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

