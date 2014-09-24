#include "room.h"
#include "lcz_generic.h"

irr::scene::IMeshSceneNode* room1::baseNode = nullptr;
btBvhTriangleMeshShape* room1::baseShape = nullptr;
irr::scene::IMeshSceneNode* room2::baseNode = nullptr;
btBvhTriangleMeshShape* room2::baseShape = nullptr;
irr::scene::IMeshSceneNode* room2c::baseNode = nullptr;
btBvhTriangleMeshShape* room2c::baseShape = nullptr;
irr::scene::IMeshSceneNode* room3::baseNode = nullptr;
btBvhTriangleMeshShape* room3::baseShape = nullptr;
irr::scene::IMeshSceneNode* room4::baseNode = nullptr;
btBvhTriangleMeshShape* room4::baseShape = nullptr;

void room1::setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape) {
	if (room1::baseNode==nullptr || room1::baseShape==nullptr) {
		room1::baseNode = inNode; room1::baseShape = inShape;
	}
}

void room2::setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape) {
	if (room2::baseNode==nullptr || room2::baseShape==nullptr) {
		room2::baseNode = inNode; room2::baseShape = inShape;
	}
}

void room2c::setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape) {
	if (room2c::baseNode==nullptr || room2c::baseShape==nullptr) {
		room2c::baseNode = inNode; room2c::baseShape = inShape;
	}
}

void room3::setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape) {
	if (room3::baseNode==nullptr || room3::baseShape==nullptr) {
		room3::baseNode = inNode; room3::baseShape = inShape;
	}
}

void room4::setBase(irr::scene::IMeshSceneNode* inNode,btBvhTriangleMeshShape* inShape) {
	if (room4::baseNode==nullptr || room4::baseShape==nullptr) {
		room4::baseNode = inNode; room4::baseShape = inShape;
	}
}

room1* room1::createNew(irr::core::vector3df inPosition,char inAngle) {
	room1* retRoom = new room1;
	retRoom->node = baseNode->clone();
	retRoom->node->setPosition(inPosition);
	retRoom->node->setRotation(irr::core::vector3df(0,inAngle*90.f,0));
	retRoom->angle = inAngle;

	//Add the Bullet rigid body
    retRoom->node->updateAbsolutePosition();
	irr::core::vector3df irrPos = retRoom->node->getAbsolutePosition();
	btVector3 btPos(irrPos.X, irrPos.Y, irrPos.Z);
	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(btPos);
	btQuaternion btRot;
	irr::core::vector3df irrRot = retRoom->node->getRotation();
	btRot.setEulerZYX(irrRot.Z,irrRot.Y,irrRot.X);

	btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

	btVector3 localInertia;
	room1::baseShape->calculateLocalInertia(0.0, localInertia);

    retRoom->rbody = new btRigidBody(0.0, MotionState, room1::baseShape, localInertia);
    room::dynamics->sharedRegisterRBody(retRoom->node,retRoom->rbody,0.f);

    return retRoom;
}
