/*
-----------------------------------------------------------------------------
Filename:    Physics.cpp
-----------------------------------------------------------------------------
*/
#include "Physics.h"
#include <cstdlib>

using namespace std;


static double gravity = -100;
static bool debug = true;
static double restitution = 0.50;
static double sr = 1.0; //scale ratio

double type1Mass = 50.0;
double type2Mass = 275.0;
float invalid = -999.999;

Physics::Physics(void)
{
}
//---------------------------------------------------------------------------
Physics::~Physics(void)
{
	//Remove the rigidbodies from the dynamics world and delete them
	for (int i=dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}

	//Delete collision shapes
	for (int j=0;j<collisionShapes.size();j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	//Delete dynamics world
	delete dynamicsWorld;
	//Delete solver
	delete solver;
	//Delete broadphase
	delete overlappingPairCache;
	//Delete dispatcher
	delete dispatcher;
	delete collisionConfiguration;
	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	collisionShapes.clear();
}

//code from http://www.ogre3d.org/forums/viewtopic.php?f=2&t=53647&start=0 by Jabberwocky
/*void Physics::DestroyAllAttachedMovableObjects( Ogre::SceneNode* i_pSceneNode )
{
	if ( !i_pSceneNode )
	{
		ASSERT( false );
		return;
	}

	// Destroy all the attached objects
	Ogre::SceneNode::ObjectIterator itObject = i_pSceneNode->getAttachedObjectIterator();

	while ( itObject.hasMoreElements() )
	{
		MovableObject* pObject = static_cast<MovableObject*>(itObject.getNext());
		i_pSceneNode->getCreator()->destroyMovableObject( pObject );
	}

	// Recurse to child SceneNodes
	Ogre::SceneNode::ChildNodeIterator itChild = i_pSceneNode->getChildIterator();

	while ( itChild.hasMoreElements() )
	{
		SceneNode* pChildNode = static_cast<SceneNode*>(itChild.getNext());
		DestroyAllAttachedMovableObjects( pChildNode );
	}
}*/

//code from https://github.com/alanjrogers/bullet-physics/blob/master/Demos/ConcaveDemo/ConcavePhysicsDemo.cpp     http://www.bulletphysics.org/Bullet/phpBB3/viewtopic.php?p=&f=9&t=1943
static bool myContactAddedCallback(btManifoldPoint& cp,const btCollisionObject* colObj0, int partId0, int index0, const btCollisionObject* colObj1, int partId1, int index1)
{
	Ogre::SceneNode *obj0 = static_cast<Ogre::SceneNode*>(colObj0->getUserPointer());
	Ogre::SceneNode* obj1 = static_cast<Ogre::SceneNode*>(colObj1->getUserPointer());

	std::string obj0_name = (obj0->getName().c_str());
	std::string obj1_name = (obj1->getName().c_str());

	if (obj0_name.find("bnode")) {

	}
	else if (obj0_name.find("pnode")) {

	}
	else if (obj0_name.find("snode")) {

	}


	return true;
}

extern ContactAddedCallback gContactAddedCallback;

//---------------------------------------------------------------------------
void Physics::initPhysics(Ogre::SceneManager* SceneMgr)
{
	cerr << "Entered bullet init" << endl;

	mSceneMgr = SceneMgr;
	
	collisionConfiguration = new btDefaultCollisionConfiguration(); //safe
	dispatcher = new btCollisionDispatcher(collisionConfiguration); //safe
	overlappingPairCache = new btDbvtBroadphase(); //safe
	//overlappingPairCache->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	solver = new btSequentialImpulseConstraintSolver; //safe

	dynamicsWorld = new btDiscreteDynamicsWorld
		(dispatcher, overlappingPairCache, solver, collisionConfiguration); //safe
	
	dynamicsWorld->setGravity(btVector3(0, gravity, 0));
	//dynamicsWorld->setInternalGhostPairCallback(new btGhostPairCallback());

	gContactAddedCallback = myContactAddedCallback;
	
	type1 = "type1";
	type2 = "type2";
	temp = 	"temp";
	other = "other";
	rigid = "rigid";

	cerr << "Finished bullet init" << endl;
}

//---------------------------------------------------------------------------
void Physics::updateWorld(const Ogre::FrameEvent& evt)
{	
	cout<<"Entered Physics::updateWorld()"<<endl;
	dynamicsWorld->stepSimulation(evt.timeSinceLastFrame,1);

	//moves positions of all objects
	for (int j=dynamicsWorld->getNumCollisionObjects()-1; j>=0 ;j--)
	{	
		//cout<<"Num Objects: "<<dynamicsWorld->getNumCollisionObjects()<<endl;
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
		//obj->setRestitution(restitution);
		btRigidBody* body = btRigidBody::upcast(obj);
		//body->applyGravity();
		btScalar timeStep = evt.timeSinceLastFrame;
		body->applyDamping(timeStep);
		/*if (obj->getCollisionFlags() == btCollisionObject::CF_CHARACTER_OBJECT) {
			btGhostObject* ghost = btGhostObject::upcast(obj);
			Ogre::SceneNode *mNode = static_cast<Ogre::SceneNode *>(body->getUserPointer());
			if (mNode) {
				btTransform trans = ghost->getWorldTransform();               
				btQuaternion rot = trans.getRotation();
				btVector3 pos = trans.getOrigin();

				mNode->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
				mNode->setPosition(pos.x(), pos.y(), pos.z());
			}
		}*/
		cout<<"Got This Far 1"<<endl;
		if (body && body->getMotionState())
		{
			Ogre::SceneNode *mNode = static_cast<Ogre::SceneNode *>(body->getUserPointer());
			btTransform trans;
			body->getMotionState()->getWorldTransform(trans);
			//body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT );
			btQuaternion rot = trans.getRotation();
			btVector3 pos = trans.getOrigin();
			cout<<"Got This Far 2"<<endl;
			//mNode->setOrientation(Ogre::Quaternion(rot.w(), rot.x(), rot.y(), rot.z()));
			cout<<"Got This Far 3"<<endl;
			mNode->setPosition(Ogre::Vector3(pos.x(),pos.y(),pos.z()));
			cout<<"Got This Far 4"<<endl;
			if (pos.y() < 0.0) {
				dynamicsWorld->removeCollisionObject(obj);
				mSceneMgr->destroySceneNode(mNode);
			}
		}
	}
	cout<<"Exiting Physics::updateWorld()"<<endl;
}

void Physics::putProjectiles(float** projectiles, int typeOfProjectile){
	//inefficient
	cout<<"Entered Physics::putProjectiles()"<<endl;
	//delete old temp projectiles from the network
	for (int j=0; j<dynamicsWorld->getNumCollisionObjects(); j++)
	{	
		//check if type 
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
		void* myPointer = obj->getUserPointer();
		if(myPointer == &temp){
			Ogre::SceneNode *mNode = static_cast<Ogre::SceneNode *>(body->getUserPointer());
			dynamicsWorld->removeCollisionObject(obj);
			mSceneMgr->destroySceneNode(mNode);
		}
	}

	//put new temp projectiles from the network
	for(int i=0; i<20; i++){
		
		if(projectiles[i][0]==invalid && projectiles[i][1]==invalid && projectiles[i][2]==invalid){
			break;
		}

		Ogre::Vector3 pos = Ogre::Vector3(projectiles[i][0], projectiles[i][1], projectiles[i][2]);
		
		Ogre::Vector3 vel = Ogre::Vector3(0.0, 0.0, 0.0);

		createBullet(sceneNode, -typeOfProjectile, pos, vel); //if the typeOfProjectile is negative, the userPointer is "temp"
		//(Ogre::SceneNode *snode, int weapon_type, Ogre::Vector3 origin, Ogre::Vector3 direction)
	}
	cout<<"Exiting Physics::putProjectiles()"<<endl;
}

void Physics::freeProjectiles(float** projectiles){
	for(int i=0; i<20; i++){
		delete [] projectiles[i];
	}
	delete [] projectiles;
}

float** Physics::getProjectiles(int typeOfProjectile){
	cout<<"Entered Physics::getProjectiles()"<<endl;
	//allocate space on heap, and initialize values to invalid
	float **projPos = new float*[20];
	for(int i=0; i<20; i++){
		projPos[i] = new float[3];
		for(int j=0; j<3; j++){
			projPos[i][j] = invalid;//will later ignore projectiles at -999.999
		}
	}
	
	int currIndex = 0;
	//iterate through objects created and being handled in Bullet Physics Engine
	for (int j=0; j<dynamicsWorld->getNumCollisionObjects() && currIndex<20; j++) //none of the first 4 objects are projectiles
	{	
		//check if type 1 or 2
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
		void* myPointer = obj->getUserPointer();
		int currObjType = -1;
		if(myPointer == &type1){
			currObjType = 1;
		}else if(myPointer == &type2){
			currObjType = 2;
		} else{
			continue;
		}
		
		//get position of projectile
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;
		body->getMotionState()->getWorldTransform(trans);
		btVector3 pos = trans.getOrigin();

		//get mass of body
		/*float bodyMass = 0.0;
		if(body->getInvMass() != 0.0){
			bodyMass = 1.0/(body->getInvMass()); //should not divide by zero 
		} else{
			continue; 
		}

		//check if type 1 or 2
		int currProjType = -1;
		if(abs(bodyMass-type1Mass) < 0.2){
			currProjType = 1;
		} else if(abs(bodyMass-type2Mass) < 0.2){
			currProjType = 2;
		} */
		//if there is a match add it to array
		if(currObjType == typeOfProjectile){
			projPos[currIndex][0] = pos.x();
			projPos[currIndex][1] = pos.y();
			projPos[currIndex][2] = pos.z();
			currIndex++;
		}

	}
	cout<<"Exiting Physics::getProjectiles()"<<endl;
	return projPos;
}


//---------------------------------------------------------------------------
btRigidBody* Physics::setRigidBoxBody(Ogre::SceneNode *snode, 
		Ogre::Vector3 shapeDim, Ogre::Vector3 origin, double mass, bool kino)
{
	cout<<"Entered Physics::setRigidBoxBody()"<<endl;
	shape = new btBoxShape(btVector3(sr*shapeDim.x/2.0, sr*shapeDim.y/2.0, sr*shapeDim.z/2.0));
	collisionShapes.push_back(shape);
	
	startTransform.setIdentity();
	
	btVector3 localInertia(0,0,0);
	
	if(mass != 0.f)
		shape->calculateLocalInertia(mass, localInertia);

	startTransform.setOrigin(btVector3(origin.x, origin.y, origin.z));
	
	btDefaultMotionState* motionState = new btDefaultMotionState(startTransform);
//	MyMotionState* motionState = new MyMotionState(snode);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	//body->setCcdMotionThreshold(0);
	//body->setCcdSweptSphereRadius(0.2f);
	body->setDamping(0.0f, 5.0f);
	//body->forceActivationState(DISABLE_DEACTIVATION);
	body->setRestitution(restitution);
	body->setUserPointer(&rigid);

	if (kino) {
        //body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	}

	dynamicsWorld->addRigidBody(body,3,3); 
	cout<<"Exiting Physics::setRigidBoxBody()"<<endl;
	return body;
}
btRigidBody* Physics::createBullet(Ogre::SceneNode *snode,
		int weapon_type, Ogre::Vector3 origin, Ogre::Vector3 direction)
{
	cout<<"Entered Physics::createBullet()"<<endl;
	sceneNode = snode;
	double mass = 1.0;
	if (weapon_type == 1 || weapon_type == -1) {
		shape = new btSphereShape(sr*3.0);
		mass = type1Mass;
	}
	else if (weapon_type == 2 || weapon_type == -2) {
		shape = new btSphereShape(sr*7.0);
		mass = type2Mass;
	}

	collisionShapes.push_back(shape);

	startTransform.setIdentity();

	btVector3 localInertia(0,0,0);

	if (mass != 0.f)
		shape->calculateLocalInertia(mass, localInertia);

	startTransform.setOrigin(btVector3(origin.x, origin.y, origin.z));

	btDefaultMotionState* motionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	body->setCollisionFlags(body->getCollisionFlags());
	//body->setDamping(0.0f, 1.0f);
	//body->forceActivationState(DISABLE_DEACTIVATION);
	body->setRestitution(restitution);
	if(weapon_type==1){
		body->setUserPointer(&type1);
	} else if(weapon_type==2){
		body->setUserPointer(&type2);
	} else if(weapon_type < 0){
		body->setUserPointer(&temp);
	} 
	else{
		body->setUserPointer(&other);
	}

	dynamicsWorld->addRigidBody(body,2,1);
	if (weapon_type == 1 || weapon_type == -1)
		body->setLinearVelocity(btVector3(direction.x * 2000.0, direction.y * 2000.0 + 20.0, direction.z * 2000.0));
	else if (weapon_type == 2 || weapon_type == -2) {
		body->setLinearVelocity(btVector3(direction.x * 400.0, 400.0, direction.z * 400.0));
		body->setGravity(btVector3(0, -700, 0));
	}

	cout<<"Exiting Physics::createBullet()"<<endl;
	return body;
}
//---------------------------------------------------------------------------
btPairCachingGhostObject* Physics::setKinematicCharacter(Ogre::SceneNode *snode,
                Ogre::Vector3 shapeDim, Ogre::Vector3 origin, double mass)
{
	// shape = new btBoxShape(btVector3(sr*shapeDim.x/2.0, sr*shapeDim.y/2.0, sr*shapeDim.z/2.0));
    // collisionShapes.push_back(shape);
	cout<<"Entered Physics::setKinematicCharacter()"<<endl;
    startTransform.setIdentity();
    startTransform.setOrigin(btVector3(origin.x, origin.y, origin.z));

	btPairCachingGhostObject* ghost = new btPairCachingGhostObject();
	ghost->setWorldTransform(startTransform);
	btScalar characterHeight = 40.0;
	btScalar characterWidth = 20.0;
	btConvexShape* capsule = new btCapsuleShape(characterWidth,characterHeight);
	ghost->setCollisionShape (shape);
	ghost->setCollisionFlags (btCollisionObject::CF_CHARACTER_OBJECT);
	//ghost->setActivationState(DISABLE_DEACTIVATION);
	ghost->setUserPointer((void *) (snode));

    ghost->setWorldTransform(startTransform);
	btScalar stepHeight = btScalar(0.35);
	btKinematicCharacterController* character = new btKinematicCharacterController (ghost,capsule,stepHeight);

	dynamicsWorld->addCollisionObject(ghost,btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
	//dynamicsWorld->addCollisionObject(ghost,btBroadphaseProxy::SensorTrigger,btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);
	dynamicsWorld->addAction(character);
	cout<<"Exiting Physics::setKinematicCharacter()"<<endl;
	return ghost;
}
//---------------------------------------------------------------------------
/*btGhostObject* Physics::setKinematicCharacter(Ogre::SceneNode *snode,
                Ogre::Vector3 shapeDim, Ogre::Vector3 origin, double mass)
{
	// shape = new btBoxShape(btVector3(sr*shapeDim.x/2.0, sr*shapeDim.y/2.0, sr*shapeDim.z/2.0));
    // collisionShapes.push_back(shape);

    startTransform.setIdentity();
    startTransform.setOrigin(btVector3(origin.x, origin.y, origin.z));

	btGhostObject* ghost = new btGhostObject();
	ghost->setWorldTransform(startTransform);
	btScalar characterHeight = 40.0;
	btScalar characterWidth = 20.0;
	btConvexShape* capsule = new btCapsuleShape(characterWidth,characterHeight);
	ghost->setCollisionShape (shape);
	ghost->setCollisionFlags (btCollisionObject::CF_CHARACTER_OBJECT);
	//ghost->setActivationState(DISABLE_DEACTIVATION);
	ghost->setUserPointer((void *) (snode));

    ghost->setWorldTransform(startTransform);
	btScalar stepHeight = btScalar(0.35);
	btKinematicCharacterController* character = new btKinematicCharacterController (ghost,capsule,stepHeight);

	dynamicsWorld->addCollisionObject(ghost,btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
	//dynamicsWorld->addCollisionObject(ghost,btBroadphaseProxy::SensorTrigger,btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);
	dynamicsWorld->addAction(character);

	return ghost;
}*/
void Physics::setBallRestitution(double restit) 
{
	restitution = restit;
}


























