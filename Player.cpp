/*
-----------------------------------------------------------------------------
Filename:    Player.cpp
-----------------------------------------------------------------------------
*/

#include "Player.h"

using namespace std;

//static variables
const float cameraRadius = 50.0; //how big the circle is that the camera orbits around the player
const float shootTimeout = 0.6; //amount of seconds that the shooting animation takes
const float walkTime = .85;
const double pi = 3.14159265359;





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
        cerr << "Beginning init player" << endl;
        cerr << "\n\nNetwork Packet is "<<sizeof(PlayerVars)<<" bytes. Upper limit is 1500 bytes.\n\n"<< endl;
        mSceneMgr = SceneMgr;
        mBullet = Bullet;
        sManager = soundManager;
        mPlayerState = new PlayerVars;
        mPlayerState->shootTimeRemaining = 0.0;
        walkTimeRemaining = 0.0;
        for (int i = 0; i < 5; ++i)
        {
                mCurrentControllerAxisState[i] = 0;
        }
           for (int i = 0; i < 1; ++i)
        {
                mCurrentControllerButtonState[i] = 0;
        }

        Ogre::Vector3 shapeDim = Ogre::Vector3(50, 80, 50);
        Ogre::Vector3 position;
        if (isServer) {
            position = Ogre::Vector3(700.0, 242.0, -750.0);
            pSig = "bnode1";
        } else {
            position = Ogre::Vector3(500.0, 242.0, -550.0);
            pSig = "bnode2";
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

        mPlayerState->server_health = 100;
        mPlayerState->client_health = 100;
        mPlayerState->weaponamt1 = 100;
        mPlayerState->weaponamt2 = 10;
        mPlayerState->weaponamt3 = 1;

        for (int i = 0; i < sizeof(mPlayerState->playerState) / sizeof(bool); i++)
                mPlayerState->playerState[i] = false;
        mPlayerState->playerState[FALLING] = true;

        mPlayerState->playerPosition[0] = position.x;
        mPlayerState->playerPosition[1] = position.y;
        mPlayerState->playerPosition[2] = position.z;

        mLook = Ogre::Vector3(1.0, 0.0, 0.0);

        bullet = 0;
        regenTime = 0.0;
        wep1Regen = 0;
        wep2Regen = 0;

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
        mPlayerState->animationStateEnabled[i]=false;
        std::string animationState = getStringFromEnum(i);
        if(animationState != "Invalid"){
            ent->getAnimationState(animationState)->setEnabled(false);
            ent->getAnimationState(animationState)->setLoop(false);
        }
    }
    
    std::string animationState = getStringFromEnum(animStateEnum);
    if(animationState != "Invalid"){
        mPlayerState->animationStateEnabled[animStateEnum]=enabled;
        mPlayerState->animationStateLoop[animStateEnum]=loop;
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
    //std::cout<<"Attacking"<<std::endl;
    mPlayerState->shootTimeRemaining = shootTimeout;
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
    float slope = (zDist)/(xDist); //m=(y2-y1)/(x2-x1)
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

        Ogre::Vector3 newCameraPos;
    if(lockedOn)
    {
        //float x = r*cos(t) + h;
        //float y = r*sin(t) + k;
       
    newCameraPos = Ogre::Vector3(
            serverPos.x - xDist*scalingFactor,
            300,
            serverPos.z - zDist*scalingFactor);
    }else
    {

         newCameraPos = Ogre::Vector3(
             50*cos(-CameraOrientation.getYaw().valueRadians()-pi*1.5) + serverPos.x,
            300,
            50*sin(-CameraOrientation.getYaw().valueRadians()-pi*1.5) + serverPos.z);

         //setCameraTargeta(serverPos);
    }

    CameraPosition = newCameraPos;
    return newCameraPos;

}

float Player::getCurrentAxisState(int axis)
{
    return mCurrentControllerAxisState[axis];
}

bool Player::getCurrentButtonState(int button)
{
    return mCurrentControllerButtonState[button];
}

PlayerVars* Player::getPlayerVars(void){

    return mPlayerState;
}

void Player::updatePlayerState(int state, bool value)
{
        mPlayerState->playerState[state]=value;
}

bool Player::getPlayerState(int state)
{
    return mPlayerState->playerState[state];
}

Ogre::Vector3 Player::getCameraTarget(void)
{
    return cameraTarget;
}

void Player::setCameraOrientation(Ogre::Quaternion rotation)
{
    CameraOrientation= rotation;
}

void Player::regenAmmo(const Ogre::FrameEvent& evt) {
    regenTime += evt.timeSinceLastFrame;
    int regenInt = (int)regenTime;
    int add1 = regenInt - wep1Regen;
    int add2 = (regenInt/3.0) - wep2Regen;
    mPlayerState->weaponamt1 += (add1 * 2);
    mPlayerState->weaponamt2 += add2;

    wep1Regen += add1;
    wep2Regen += add2;

    if(mPlayerState->weaponamt1 > wep1max)
        mPlayerState->weaponamt1 = wep1max;
    if(mPlayerState->weaponamt2 > wep2max)
        mPlayerState->weaponamt2 = wep2max;
}

void Player::updatePositionFromPacket(const Ogre::FrameEvent& evt, PlayerVars* packet){
    //update player based on packet recieved over the network

    Ogre::Vector3 currPos = getPlayerPosition();

    float xTrans = packet->playerPosition[0] - currPos.x;
    float yTrans = packet->playerPosition[1] - currPos.y;
    float zTrans = packet->playerPosition[2] - currPos.z;

    mPlayerState->shootTimeRemaining = packet->shootTimeRemaining;

    for(int i=0; i<animEnumCount; i++){
        if(mPlayerState->animationStateEnabled[i] != packet->animationStateEnabled[i]){
                enableState(i, packet->animationStateEnabled[i], packet->animationStateLoop[i]);
        }
        if(mPlayerState->animationStateEnabled[i]==true){
            if(i==Shoot){
                if(mPlayerState->shootTimeRemaining>0.0){
                    updateAnimation(i, evt.timeSinceLastFrame);
                    mPlayerState->shootTimeRemaining-=evt.timeSinceLastFrame;
                } else{

                }
            }
            else{
                updateAnimation(i, evt.timeSinceLastFrame);
            }
        }
    }


    pnode->translate(Ogre::Vector3(xTrans, yTrans, zTrans), Ogre::Node::TS_WORLD);
    pnode->lookAt(cameraTarget, Ogre::Node::TS_WORLD, Ogre::Vector3::UNIT_X);

    btTransform enemyTrans;
    enemyTrans.setIdentity();
    enemyTrans.setOrigin(btVector3(packet->playerPosition[0], packet->playerPosition[1], packet->playerPosition[2]));

    btCollisionObject* eObj = mBullet->collisionObject(pnode);
    btRigidBody* body = btRigidBody::upcast(eObj);
    body->getMotionState()->setWorldTransform(enemyTrans);

    body->getMotionState()->getWorldTransform(enemyTrans);
    btVector3 vec3 = enemyTrans.getOrigin();
    //cout << "Enemy SceneNode " << pnode->getPosition().x << pnode->getPosition().y << pnode->getPosition().z << endl;
    //cout << "Enemy Origin: " << vec3.getX() << " " << vec3.getY() << " " << vec3.getZ() << endl;
}

void Player::playerKilled(void){
    enableState(Die, true, false);
}

void Player::updatePosition(const Ogre::FrameEvent& evt)
{   
    //size of packet over the network;
    //printf("playervars: %d \n",sizeof (PlayerVars) );
    //cout << "P1 Health: " << mPlayerState->server_health << endl;
    //cout << "P2 Health: " << mPlayerState->client_health << endl;
    float distPerSec = 200;
    mDirection = Ogre::Vector3(0.0,0.0,0.0);
    
/*
    const float walkTime = 2.0;
    float walkTimeRemaining = 0.0;
*/
    if(mCurrentControllerAxisState[LCONTROLX] != 0.0){
        
        if(mPlayerState->animationStateEnabled[Walk]==false){
            enableState(Walk, true, true);
        }
        
        mDirection.x = mCurrentControllerAxisState[LCONTROLX]*distPerSec;
    }
    if(mCurrentControllerAxisState[LCONTROLY] != 0.0){
        if(mPlayerState->animationStateEnabled[Walk]==false){
            enableState(Walk, true, true);
        }
        mDirection.z = mCurrentControllerAxisState[LCONTROLY]*distPerSec;
    }
    if(abs(mCurrentControllerAxisState[LCONTROLY]) < 0.05 && abs(mCurrentControllerAxisState[LCONTROLX]) < 0.05){
        if(mPlayerState->animationStateEnabled[Walk]==true){
            enableState(Walk, false, false);
        }
    }
    if(mCurrentControllerAxisState[LCONTROLY] != 0.0 || mCurrentControllerAxisState[LCONTROLX] != 0.0){
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
        mDirection.x += mCurrentControllerAxisState[LCONTROLY]*diffX*scalingFactor;
        mDirection.z += mCurrentControllerAxisState[LCONTROLY]*diffZ*scalingFactor;

        //left and right (Doesn't work yet)
        //Using http://math.stackexchange.com/questions/53875/calculating-point-around-circumference-of-circle-given-distance-travelled
        
        double circumference = 2*pi*distanceToTarget;
        double d = distPerSec * evt.timeSinceLastFrame*50.0;
        
        double theta = d/r;

        double newXCoord = (a+(x-a)*cos(theta)-(y-b)*sin(theta)); //raw x-z coordinate point
        double newYCoord = (b+(x-a)*sin(theta)+(y-b)*cos(theta)); //raw x-z coordinate point
        mDirection.x -= mCurrentControllerAxisState[LCONTROLX]*(newXCoord-x); //amount of translation needed
        mDirection.z -= mCurrentControllerAxisState[LCONTROLX]*(newYCoord-y); //amount of translation needed

    }else
    {
        //xcos - ysin
        //xsin + ycos

        float camCos = cos(-CameraOrientation.getYaw().valueRadians());
        float camSin = sin(-CameraOrientation.getYaw().valueRadians());
        float controlX = mCurrentControllerAxisState[LCONTROLX]*distPerSec;
        float controlY = mCurrentControllerAxisState[LCONTROLY]*distPerSec;
        mDirection.x =  controlX*camCos - controlY*camSin;
        mDirection.z =  controlX*camSin + controlY*camCos;
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

    if (getPlayerState(SHOOTING2) && mPlayerState->weaponamt1 > 0) {
        //small and fast projectile
        attack(true);
        sManager->playSoundFromEnum(Sound::Shoot1);
        //printf("LookX: %f, LookZ: %f, calcX: %f, calcZ: %f\n",mLook.x, mLook.z, (mLook.x*10.0) - ((mLook.z)*10.0), (mLook.z*10.0) + ((mLook.x)*10.0));
        //Ogre::Vector3 position = Ogre::Vector3(pnode->getPosition().x + (mLook.x*10.0) - ((1-mLook.z)*10.0),
        //                                       pnode->getPosition().y+20, pnode->getPosition().z + (mLook.z*10.0) + ((1-mLook.x)*10.0));
        Ogre::Vector3 position = Ogre::Vector3(pnode->getPosition().x + (mLook.x*30.0), pnode->getPosition().y+20, pnode->getPosition().z + (mLook.z*30.0));

        Ogre::Entity* ent = mSceneMgr->createEntity("Bullet" + Ogre::StringConverter::toString(bullet),
                                                    "sphere.mesh");
        Ogre::SceneNode* bnode = mSceneMgr->getRootSceneNode()->
                createChildSceneNode(pSig + Ogre::StringConverter::toString(1) + Ogre::StringConverter::toString(bullet++), position);

        bnode->attachObject(ent);
        bnode->scale(.03, .03, .03);
        Ogre::MaterialPtr bMat = ent->getSubEntity(0)->getMaterial()->clone("newBallColor");
        bMat->getTechnique(0)->getPass(0)->setAmbient(1.0, 0.8, 0.0);
        bMat->getTechnique(0)->getPass(0)->setDiffuse(1.0, 0.8, 0.0, 1.0);
        ent->setMaterialName(bMat->getName());
        // ent->setMaterialName("Examples/Chrome");
        ent->setCastShadows(true);
        
        mBullet->createBullet(bnode, 1, position, mLook);

        mPlayerState->weaponamt1 = mPlayerState->weaponamt1 - 1;
        updatePlayerState(SHOOTING2,false);
    }
    if (getPlayerState(SHOOTING1) && mPlayerState->weaponamt2 > 0) {
        //large and slow projectile
        attack(true);
        sManager->playSoundFromEnum(Sound::Shoot2);
        //Ogre::Vector3 position = Ogre::Vector3(pnode->getPosition().x+10, pnode->getPosition().y+20, pnode->getPosition().z-10);
        Ogre::Vector3 position = Ogre::Vector3(pnode->getPosition().x + (mLook.x*30.0), pnode->getPosition().y+20, pnode->getPosition().z + (mLook.z*30.0));

        Ogre::Entity* ent = mSceneMgr->createEntity("Bullet" + Ogre::StringConverter::toString(bullet),
                                                    "sphere.mesh");
        Ogre::SceneNode* bnode = mSceneMgr->getRootSceneNode()->
                createChildSceneNode(pSig + Ogre::StringConverter::toString(2) + Ogre::StringConverter::toString(bullet++), position);

        bnode->attachObject(ent);
        bnode->scale(.07, .07, .07);
        Ogre::MaterialPtr gMat = ent->getSubEntity(0)->getMaterial()->clone("newBombColor");
        gMat->getTechnique(0)->getPass(0)->setAmbient(0.2, 0.8, 0.2);
        gMat->getTechnique(0)->getPass(0)->setDiffuse(0.1, 0.8, 0.2, 1.0);
        ent->setMaterialName(gMat->getName());
        // ent->setMaterialName("Examples/Chrome");
        ent->setCastShadows(true);
        
        mBullet->createBullet(bnode, 2, position, mLook);

        mPlayerState->weaponamt2 = mPlayerState->weaponamt2 - 1;
        updatePlayerState(SHOOTING1,false);
    }

    for(int i=0; i<animEnumCount; i++){
        if(mPlayerState->animationStateEnabled[i]==true){
            if(i==Shoot){
                if(mPlayerState->shootTimeRemaining>0.0){
                    updateAnimation(i, evt.timeSinceLastFrame);
                    mPlayerState->shootTimeRemaining-=evt.timeSinceLastFrame;
                } else{
                    enableState(Shoot, false, false);
                }

            } else{
                 updateAnimation(i, evt.timeSinceLastFrame);
            }
        }
    }
    mPlayerState->playerPosition[0] = pnode->getPosition().x;
    mPlayerState->playerPosition[1] = pnode->getPosition().y;
    mPlayerState->playerPosition[2] = pnode->getPosition().z;

    // mBody->applyDamping(evt.timeSinceLastFrame);
    //printf("getPOSX %f \n",mPlayerState->playerPosition[X]);
    //printf(" mCurrentControllerStateX: %f\n",mCurrentControllerState[LCONTROLX]);
    //printf(" mCurrentControllerStateY: %f\n",mCurrentControllerState[LCONTROLY]);
}

void Player::updateControlAxis(int axis, float value)
{
        mCurrentControllerAxisState[axis]=value;
}

void Player::updateControlButton(int button, float value)
{
        mCurrentControllerButtonState[button]=value;
switch(button)
    {
        case LBUMP:
        updatePlayerState(SHOOTING1, value);
        break;
        case RBUMP:
        updatePlayerState(SHOOTING2, value);
        break;
        case LJOYCLICK:
        updatePlayerState(JUMPING, value);
        break;
        case RJOYCLICK:
        toggleLock();
        break;


    }

}
