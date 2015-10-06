#include "NPC173.h"
#include "../World.h"
#include "../Player.h"

#include <cmath>

btCollisionShape* NPC173::shape = nullptr;
//irr::scene::IMeshSceneNode* NPC096::baseNode = nullptr;
irr::scene::IMeshSceneNode* NPC173::baseNode = nullptr;
irr::scene::IMeshSceneNode* NPC173::baseOcclusionNode = nullptr;
irr::video::IVideoDriver* NPC173::driver = nullptr;

NPC173::NPC173() {
    node = NPC173::baseNode->clone();
    occlusionNode = NPC173::baseOcclusionNode->clone();
    occlusionNode->getMaterial(0).FrontfaceCulling = true;
    occlusionNode->getMaterial(0).BackfaceCulling = false;
    occlusionNode->getMaterial(0).DiffuseColor = irr::video::SColor(0,0,0,0);
    //driver->addOcclusionQuery(occlusionNode,NPC173::baseOcclusionNode->getMesh());
    collider = NPC::dynamics->addPlayerColliderObject(node,12.f*RoomScale,4.f*RoomScale,64000.f);//CapsuleObject(node,12.f,3.f,16000.f);
    NPC::dynamics->unregisterRBody(collider);
    NPC::dynamics->registerNewRBody(node,collider,-1,-1,~0,irr::core::vector3df(0.f,5.8f*RoomScale,0.f));
    collider->setLinearFactor(btVector3(1.f,1.f,1.f));
    collider->setAngularFactor(btVector3(0.f,0.f,0.f));
    collider->setGravity(btVector3(0.f,-4000.f*RoomScale,0.f));
    memDir = irr::core::vector3df(0.f,0.f,0.f);
    collider->setCcdMotionThreshold(3.f*RoomScale);
	collider->setCcdSweptSphereRadius(1.5f*RoomScale);
	collider->setLinearVelocity(btVector3(0.f,0.f,0.f));
	/*prevPos[0] = irr::core::vector3df(-100.f);
    prevPos[1] = irr::core::vector3df(100.f);*/
}

NPC173* NPC173::createNPC173() {
    NPC173* returnNPC = new NPC173;
    return returnNPC;
}

void NPC173::update() {
    //collider->setLinearVelocity(btVector3(0.f,0.f,0.f));
    //std::cout<<"173update"<<prevPos[0].X<<"___"<<prevPos[1].X<<"\n";

    bool seesPlayer = false;
    if (NPC::dynamics->rayTestPoint(irrToBtVec(NPC::player->getPosition()),collider->getCenterOfMassPosition()).equals(btToIrrVec(collider->getCenterOfMassPosition()),4.f*RoomScale)) {
        seesPlayer = true;
        irr::core::vector3df dirToPlayer = (NPC::player->getPosition()-node->getPosition());
        dirToPlayer.Y = 0.f;
        memDir = dirToPlayer.normalize();
    } else {
        //boxNode->setPosition(NPC::dynamics->rayTestPoint(irrToBtVec(NPC::player->getPosition()),collider->getCenterOfMassPosition()));
    }

    if (!NPC::player->seesMeshNode(static_cast<irr::scene::IMeshSceneNode*>(node)) || (NPC::player->blinkTimer<=-0.25f && NPC::player->blinkTimer>=-0.75f)/* || !(driver->getOcclusionQueryResult(occlusionNode)>0)*/) {
        collider->setLinearFactor(btVector3(1.f,1.f,1.f));
        moveDir = memDir*450.f*RoomScale;
        moveDir.Y = collider->getLinearVelocity()[1];
        collider->setLinearVelocity(irrToBtVec(moveDir));
        collider->setFriction(0.f);
        collider->setDamping(0.f,0.f);
        collider->setRestitution(0.f);
        collider->forceActivationState(ACTIVE_TAG); collider->activate();
        if (!seesPlayer) { //doesn't see player
            if (NPC::dynamics->rayTest(collider->getCenterOfMassPosition(),collider->getCenterOfMassPosition()+irrToBtVec(memDir*5.5f*RoomScale))) { //ran into a wall, change direction randomly
                memDir.X = rand()%500-250;
                memDir.Z = rand()%500-250;
                memDir = memDir.normalize();
            }
        }
    } else {
        collider->setLinearFactor(btVector3(0.f,1.f,0.f));
        if (collider->getLinearVelocity()[1]<=0.f) {
            collider->setLinearVelocity(btVector3(0.f,collider->getLinearVelocity()[1],0.f));
        } else {
            collider->setLinearVelocity(btVector3(0.f,-collider->getLinearVelocity()[1],0.f));
        }
    }
}

void NPC173::updateModel() {
    irr::core::vector3df rot;
    irr::core::vector3df pointAt;
    pointAt = -moveDir;
    rot.Y = (std::atan2(pointAt.X,pointAt.Z)*irr::core::RADTODEG);
    node->setRotation(rot);
    occlusionNode->setPosition(node->getPosition()+irr::core::vector3df(0.f,14.f*RoomScale,0.f));
}

void NPC173::teleport(irr::core::vector3df newPos) {
    btTransform oTrans = collider->getCenterOfMassTransform();
	oTrans.setOrigin(btVector3(newPos.X,newPos.Y,newPos.Z));
	collider->setCenterOfMassTransform(oTrans);
	collider->setLinearVelocity(btVector3(0,0,0));
	node->setPosition(newPos);
}
