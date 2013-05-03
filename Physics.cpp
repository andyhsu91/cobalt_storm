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
//---------------------------------------------------------------------------
void Physics::initPhysics()
{
	cerr << "Enter bullet init" << endl;
	
	collisionConfiguration = new btDefaultCollisionConfiguration(); //safe
	dispatcher = new btCollisionDispatcher(collisionConfiguration); //safe
	overlappingPairCache = new btDbvtBroadphase(); //safe
	overlappingPairCache->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	solver = new btSequentialImpulseConstraintSolver; //safe

	dynamicsWorld = new btDiscreteDynamicsWorld
		(dispatcher, overlappingPairCache, solver, collisionConfiguration); //safe
	
	dynamicsWorld->setGravity(btVector3(0, gravity, 0));
	//dynamicsWorld->setInternalGhostPairCallback(new btGhostPairCallback());
	
	cerr << "Finished bullet init" << endl;
}
//---------------------------------------------------------------------------
void Physics::updateWorld(const Ogre::FrameEvent& evt)
{
	dynamicsWorld->stepSimulation(evt.timeSinceLastFrame,1);

	//moves positions of all objects
	for (int j=dynamicsWorld->getNumCollisionObjects()-1; j>=0 ;j--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
		//obj->setRestitution(restitution);
		btRigidBody* body = btRigidBody::upcast(obj);
		if (obj->getCollisionFlags() == btCollisionObject::CF_CHARACTER_OBJECT) {
			btGhostObject* ghost = btGhostObject::upcast(obj);
			Ogre::SceneNode *mNode = static_cast<Ogre::SceneNode *>(body->getUserPointer());
			if (mNode) {
				btTransform trans = ghost->getWorldTransform();               
				btQuaternion rot = trans.getRotation();
				btVector3 pos = trans.getOrigin();

				mNode->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
				mNode->setPosition(pos.x(), pos.y(), pos.z());
			}
		}
		else if (body && body->getMotionState())
		{
			Ogre::SceneNode *mNode = static_cast<Ogre::SceneNode *>(body->getUserPointer());
			btTransform trans;
			body->getMotionState()->getWorldTransform(trans);
			btQuaternion rot = trans.getRotation();
			btVector3 pos = trans.getOrigin();

			mNode->setOrientation(Ogre::Quaternion(rot.w(), rot.x(), rot.y(), rot.z()));
			mNode->setPosition(Ogre::Vector3(pos.x(),pos.y(),pos.z()));
		}
	}
}
//---------------------------------------------------------------------------
btRigidBody* Physics::setRigidBoxBody(Ogre::SceneNode *snode, 
		Ogre::Vector3 shapeDim, Ogre::Vector3 origin, double mass, bool kino, bool bull)
{
	if (bull)
		shape = new btSphereShape(sr*shapeDim.x/2.0);
	else
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
	body->setRestitution(restitution);
	body->setUserPointer((void *) (snode));

	if (kino) {
        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        body->setActivationState(DISABLE_DEACTIVATION);
	}

	if (bull) {
		body->setLinearVelocity(btVector3(200.0, 100.0, 0.0));
	}

	dynamicsWorld->addRigidBody(body); 
	
	return body;
}
//---------------------------------------------------------------------------
btGhostObject* Physics::setKinematicCharacter(Ogre::SceneNode *snode,
                Ogre::Vector3 shapeDim, Ogre::Vector3 origin, double mass)
{
	shape = new btBoxShape(btVector3(sr*shapeDim.x/2.0, sr*shapeDim.y/2.0, sr*shapeDim.z/2.0));
    collisionShapes.push_back(shape);

    startTransform.setIdentity();
    startTransform.setOrigin(btVector3(origin.x, origin.y, origin.z));

	btGhostObject* ghost = new btGhostObject();
	ghost->setWorldTransform(startTransform);
	//btConvexShape* capsule = new btCapsuleShape(characterWidth,characterHeight);
	ghost->setCollisionShape (shape);
	ghost->setCollisionFlags (ghost->getCollisionFlags() | btCollisionObject::CF_CHARACTER_OBJECT);
	ghost->setActivationState(DISABLE_DEACTIVATION);
	ghost->setUserPointer((void *) (snode));

	//btScalar stepHeight = btScalar(0.35);
	//btKinematicCharacterController* character = new btKinematicCharacterController (ghost,capsule,stepHeight);

	//dynamicsWorld->addCollisionObject(ghost,btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
	dynamicsWorld->addCollisionObject(ghost,btBroadphaseProxy::SensorTrigger,btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);
	//dynamicsWorld->addAction(character);

	return ghost;
}
void Physics::setBallRestitution(double restit) 
{
	restitution = restit;
}


























