#include "3dworld.h"
#include "player.h"

player::player(world* own,irr::scene::ISceneManager* smgr,irrDynamics* dyn,MainEventReceiver* receiver,float iheight,float iradius,float mass) {
    owner = own;
    irrSmgr = smgr;
    dynamics = dyn;
    irrReceiver = receiver;

	height = iheight*RoomScale;
	radius = iradius*RoomScale;

    selfRotation.X = selfRotation.Y = selfRotation.Z = 0;

	for (unsigned char i=0;i<5;i++) {
		breathSound[i][0] = sound::getSound(std::string("SFX/9341/breath")+std::to_string(i)+std::string(".ogg"),false);
		breathSound[i][1] = sound::getSound(std::string("SFX/9341/breath")+std::to_string(i)+std::string("gas.ogg"),false);
	}

	for (unsigned char i=0;i<4;i++) {
		stepSound[0][0][i] = sound::getSound(std::string("SFX/Step")+std::to_string(i+1)+std::string(".ogg"),false);
		stepSound[0][1][i] = sound::getSound(std::string("SFX/Run")+std::to_string(i+1)+std::string(".ogg"),false);
		stepSound[1][0][i] = sound::getSound(std::string("SFX/StepMetal")+std::to_string(i+1)+std::string(".ogg"),false);
		stepSound[1][1][i] = sound::getSound(std::string("SFX/RunMetal")+std::to_string(i+1)+std::string(".ogg"),false);
		if (i<3) {
			stepSound[2][0][i] = sound::getSound(std::string("SFX/StepPD")+std::to_string(i+1)+std::string(".ogg"),false);
			stepSound[3][1][i] = sound::getSound(std::string("SFX/StepForest")+std::to_string(i+1)+std::string(".ogg"),false);
		}
	}

    // Add camera
    Camera = irrSmgr->addCameraSceneNode(0,irr::core::vector3df(0,0,0),irr::core::vector3df(0,0,-1));
    Camera->setPosition(irr::core::vector3df(15*204.8f*RoomScale, 10*RoomScale, 15*204.8f*RoomScale));
    Camera->setTarget(irr::core::vector3df(0, 0, 0));
    Camera->setNearValue(2.0*RoomScale);
    Camera->setFarValue(200.0*RoomScale);

    irrSmgr->setActiveCamera(Camera);

    //Add capsule
    Capsule = dynamics->addPlayerColliderObject(Camera,height,radius,mass,1,1);
    Capsule->setAngularFactor(btVector3(0,0,0)); //don't let the capsule rotate until the player dies
    Capsule->setSleepingThresholds (0.0, 0.0);
    Capsule->setGravity(btVector3(0.f,-450.f*RoomScale,0.f));
	Capsule->setActivationState(DISABLE_DEACTIVATION);

    //clear inventory
    for (irr::u32 i=0;i<inventory_size;i++) {
        inventory[i]=nullptr;
    }
}

void player::teleport(irr::core::vector3df position) {
	btTransform oTrans = Capsule->getCenterOfMassTransform();
	oTrans.setOrigin(btVector3(position.X,position.Y,position.Z));
	Capsule->setCenterOfMassTransform(oTrans);
}

void player::resetSpeeds() {
	Capsule->setLinearVelocity(btVector3(dynSpeed[0],Capsule->getLinearVelocity()[1],dynSpeed[2]));
}

void player::update() {

	dynSpeed = Capsule->getLinearVelocity();

	float fpsFactor = owner->getFPSfactor();

	BlinkTimer-=fpsFactor*0.09;

    if (BlinkTimer<=-1.785) {
        BlinkTimer = 100;
    }

    if (irrReceiver->IsKeyDown(irr::KEY_SPACE)) {
        if (BlinkTimer>0) { BlinkTimer = 0.0; }
        if (BlinkTimer<-0.8925) { BlinkTimer = -0.8925; }
    }

    while (yaw>180.0) { yaw-=360.0; }
    while (yaw<-180.0) { yaw+=360.0; }

    while (pitch>180.0) { pitch-=360.0; }
    while (pitch<-180.0) { pitch+=360.0; }

    if (pitch>70.0) pitch = 70.0;
    if (pitch<-70.0) pitch = -70.0;

	float prevShake = shake;
	if (shakeFactor>0.f) {
		shake+=std::max(std::min(shakeFactor*0.15f,10.0f),6.f)*fpsFactor;
	}
    while (shake>720.f) shake-=720.f;
    while (shake<0.f) shake+=720.f;

    btVector3 speed = Capsule->getLinearVelocity();

    if (speed[1]-20.f>prevLinearVelocity[1]) {
		if (dynamics->rayTest(Capsule->getCenterOfMassPosition(),Capsule->getCenterOfMassPosition()-btVector3(0,height/2.f,0))) {
			unsigned char chosen = rand()%4;
			stepSound[owner->pickPlayerTriangle()][1][chosen]->playSound(false);
		}
    }

	shakeFactor = 0.f;

	Capsule->setGravity(btVector3(0.f,-450.f*RoomScale,0.f));

    if (!dead) {
		Capsule->setFriction(0.01f);
		sprintTimer=std::min(std::max(0.f,sprintTimer-0.1f*fpsFactor),10.f);
		resetSpeeds();
		if ((irrReceiver->IsKeyDown(irr::KEY_KEY_W)
			|| irrReceiver->IsKeyDown(irr::KEY_KEY_S)
			|| irrReceiver->IsKeyDown(irr::KEY_KEY_A)
			|| irrReceiver->IsKeyDown(irr::KEY_KEY_D)
			) && std::abs(speed[1]) < 20.f) {

			float walkSpeed = 40.f;
			if (crouchState<0.015f) {
				if (irrReceiver->IsKeyDown(irr::KEY_LSHIFT)) {
					if (sprintTimer>0.f && sprintTimer<10.f) Stamina-=5.f;
					sprintTimer = 20.f;
					if (Stamina>0) walkSpeed=120.f-((100.f-Stamina)*(100.f-Stamina)*0.004);
					Stamina-=0.2*fpsFactor;
					if (Stamina<0) Stamina = -10.f;

					if (Stamina<5.f) {
						if (!breathSound[currBreathSound][0]->isPlaying()) {
							currBreathSound = 0;
							breathSound[currBreathSound][0]->playSound(false);
						}
					} else if (Stamina<50.f) {
						if (!breathSound[currBreathSound][0]->isPlaying()) {
							currBreathSound = (rand()%3)+1;
							breathSound[currBreathSound][0]->playSound(false);
						}
					}
				} else {
					Stamina=std::min(Stamina+0.15f*fpsFactor,100.f);
				}
			} else {
				if (crouchState>0.5f) walkSpeed = 10.f; else walkSpeed = 20.f;
				Stamina=std::min(Stamina+0.15f*fpsFactor,100.f);
			}
			if (irrReceiver->IsKeyDown(irr::KEY_KEY_W)) {
				dir = 0.f;
				if (irrReceiver->IsKeyDown(irr::KEY_KEY_A)) {
					dir -= 45;
				} else if (irrReceiver->IsKeyDown(irr::KEY_KEY_D)) {
					dir += 45;
				}
			} else if (irrReceiver->IsKeyDown(irr::KEY_KEY_S)) {
				dir = 180;
				if (irrReceiver->IsKeyDown(irr::KEY_KEY_A)) {
					dir += 45;
				} else if (irrReceiver->IsKeyDown(irr::KEY_KEY_D)) {
					dir -= 45;
				}
			} else {
				if (irrReceiver->IsKeyDown(irr::KEY_KEY_A)) {
					dir = -90;
				} else if (irrReceiver->IsKeyDown(irr::KEY_KEY_D)) {
					dir = 90;
				}
			}

			if ((prevShake<=270.f && shake>270.f) || (prevShake<=630.f && shake>630.f)) {
				unsigned char chosen = rand()%4;
				stepSound[owner->pickPlayerTriangle()][walkSpeed>40.f][chosen]->playSound(false);
			}

			dir-=90;
			dir *= irr::core::DEGTORAD;

			if (walkingSpeed>walkSpeed) {
				walkingSpeed=std::max(walkSpeed,walkingSpeed+(walkSpeed-walkingSpeed)*0.25f*fpsFactor);
			} else {
				walkingSpeed=std::min(walkingSpeed+(walkSpeed-walkingSpeed)*0.05f*fpsFactor,walkSpeed);
			}

			std::cout<<walkingSpeed<<"\n";

			shakeFactor = walkSpeed;
		} else {
			Stamina=std::min(Stamina+0.15*fpsFactor,100.0);
			if (std::abs(speed[1])<10.f) {
				walkingSpeed = walkingSpeed-(walkingSpeed*0.5f*fpsFactor);
				if (walkingSpeed<=0.02f) walkingSpeed = 0.f;
			} else {
				if (walkingSpeed>=60.f) walkingSpeed=std::max(60.f,walkingSpeed-(walkingSpeed*0.5f*fpsFactor));
			}
		}
		if (irrReceiver->IsKeyDown(irr::KEY_LCONTROL)) {
			crouched = true;
		} else {
			btVector3 start = Capsule->getCenterOfMassPosition();
			btVector3 finish = start + btVector3(0,height*0.85f,0);
			start += btVector3(0,(height * 0.25f),0);
			if (!dynamics->rayTest(start,finish)) {
				if (!dynamics->rayTest(start+btVector3(radius*1.15f,0,radius*1.15f),finish+btVector3(radius*1.15f,0,radius*1.15f))) {
					if (!dynamics->rayTest(start+btVector3(-radius*1.15f,0,radius*1.15f),finish+btVector3(-radius*1.15f,0,radius*1.15f))) {
						if (!dynamics->rayTest(start+btVector3(radius*1.15f,0,-radius*1.15f),finish+btVector3(radius*1.15f,0,-radius*1.15f))) {
							if (!dynamics->rayTest(start+btVector3(-radius*1.15f,0,-radius*1.15f),finish+btVector3(-radius*1.15f,0,-radius*1.15f))) {
								crouched = false; //don't stand up if there's something above your head
							}
						}
					}
				}
			}
		}
		Capsule->setLinearVelocity(dynSpeed+btVector3(std::cos(dir+yaw*irr::core::DEGTORAD)*walkingSpeed*RoomScale,0.f,-std::sin(dir+yaw*irr::core::DEGTORAD)*walkingSpeed*RoomScale));
	} else {
		Capsule->setFriction(3.0f);
		Capsule->setRollingFriction(25.0f);
		Capsule->setRestitution(0.f);
		btVector3 start = Capsule->getCenterOfMassPosition();
		btVector3 finish = start + btVector3(0,height*0.85f,0);
		start += btVector3(0,(height * 0.25f),0);
		if (!dynamics->rayTest(start,finish)) {
			if (!dynamics->rayTest(start+btVector3(radius*1.15f,0,radius*1.15f),finish+btVector3(radius*1.15f,0,radius*1.15f))) {
				if (!dynamics->rayTest(start+btVector3(-radius*1.15f,0,radius*1.15f),finish+btVector3(-radius*1.15f,0,radius*1.15f))) {
					if (!dynamics->rayTest(start+btVector3(radius*1.15f,0,-radius*1.15f),finish+btVector3(radius*1.15f,0,-radius*1.15f))) {
						if (!dynamics->rayTest(start+btVector3(-radius*1.15f,0,-radius*1.15f),finish+btVector3(-radius*1.15f,0,-radius*1.15f))) {
							crouched = false; //don't stand up if there's something above your head
						}
					}
				}
			}
		}
	}

    if (irrReceiver->IsKeyDown(irr::KEY_KEY_Q)) {
		Capsule->setAngularFactor(btVector3(1,1,1)); dead = true;
	}
    if (irrReceiver->IsKeyDown(irr::KEY_KEY_E)) {
		dead = false;
		Capsule->setAngularFactor(btVector3(0,0,0));
		const btQuaternion identity(0,0,0,1);
		btTransform Transform = Capsule->getCenterOfMassTransform();
		Transform.setRotation(identity);
		Capsule->setCenterOfMassTransform(Transform);
		Capsule->setAngularVelocity(btVector3(0,0,0));
	}

	bool changed = false;
	if (crouched) {
		if (crouchState<0.5f) changed = true;

		float prevState = crouchState;
		crouchState=std::min(0.5f,crouchState+(0.5f-crouchState)*fpsFactor*0.2f);
		if (crouchState>0.4998f) crouchState = 0.5f;

		if (changed) { //must unregister body to perform changes to its shape
			dynamics->unregisterRBody(Capsule);
			Capsule->getCollisionShape()->setLocalScaling(btVector3(1.f,1.f-crouchState,1.f));
			Capsule->translate(btVector3(0.f,(prevState-crouchState)*height*0.49f,0.f));
			dynamics->registerNewRBody(Camera,Capsule,-1,1,1,irr::core::vector3df(0,(-(height/2.f)*(1.f-crouchState))+(radius/1.5f),0));
			Capsule->setGravity(btVector3(0.f,-450.f*RoomScale,0.f));
		}
	} else {
		if (crouchState>0.0f) changed = true;

		float prevState = crouchState;
		crouchState=std::max(0.f,crouchState+(-crouchState)*fpsFactor*0.1f);
		if (crouchState<0.002f) crouchState = 0.f;

		if (changed) { //must unregister body to perform changes to its shape
			dynamics->unregisterRBody(Capsule);
			Capsule->getCollisionShape()->setLocalScaling(btVector3(1.f,1.f-crouchState,1.f));
			Capsule->translate(btVector3(0.f,(prevState-crouchState)*height*0.49f,0.f));
			dynamics->registerNewRBody(Camera,Capsule,-1,1,1,irr::core::vector3df(0,(-(height/2.f)*(1.f-crouchState))+(radius/1.5f),0));
			Capsule->setGravity(btVector3(0.f,-450.f*RoomScale,0.f));
		}
	}
	//std::cout<<Capsule->getCollisionShape()->getLocalScaling()[1]<<" "<<crouchState<<"\n";

	prevLinearVelocity = Capsule->getLinearVelocity();

    //std::cout<<"seesNode: "<<seesMeshNode(testNode)<<"\n";
}

void player::updateHead() {
	Camera->updateAbsolutePosition();
    Camera->setPosition(Camera->getAbsolutePosition()+irr::core::vector3df(0.f,std::sin(shake*irr::core::DEGTORAD)*(0.4f-(crouchState*0.4f)),0.f));

    irr::core::vector3df cdir(0,1,0);
    irr::core::matrix4 rotMatrix;

    selfRotation.X = pitch;
    selfRotation.Y = yaw;

    rotMatrix.setRotationDegrees(irr::core::vector3df(0,0,std::max(std::min(std::sin((shake)*irr::core::DEGTORAD/2.f)*2.5f,8.f),-8.f)*0.25f));
    rotMatrix.transformVect(cdir);

    rotMatrix.setRotationDegrees(selfRotation);
    rotMatrix.transformVect(cdir);

    rotMatrix.setRotationDegrees(Camera->getRotation());
    rotMatrix.transformVect(cdir);
    Camera->setUpVector(cdir);
    cdir.X = 0;
    cdir.Y = 0;
    cdir.Z = 1;

    rotMatrix.setRotationDegrees(selfRotation);
    rotMatrix.transformVect(cdir);

    rotMatrix.setRotationDegrees(Camera->getRotation());
    rotMatrix.transformVect(cdir);
    Camera->updateAbsolutePosition();

    sound::setListenerPos(Camera->getAbsolutePosition());
    sound::setListenerOrientation(Camera->getUpVector(),cdir);

    Camera->setTarget(Camera->getAbsolutePosition()+cdir);

	lastMouseDown[0] = irrReceiver->IsMouseDown(0);
	lastMouseDown[1] = irrReceiver->IsMouseDown(1);
}

bool player::addToInventory(item* it) {
    for (irr::u32 i=0;i<inventory_size;i++) {
        if (inventory[i]==nullptr) {
            inventory[i]=it;
            it->Pick();
            return true;
        }
    }
    return false;
}
item* player::takeFromInventory(unsigned char slot) {
	slot%=inventory_size;
	if (inventory[slot]!=nullptr) {
		Camera->updateAbsolutePosition();
		inventory[slot]->Unpick(Camera->getAbsolutePosition());
		item* it = inventory[slot];
		inventory[slot]=nullptr;
		return it;
	}
	return nullptr;
}

const irr::scene::SViewFrustum* player::getViewFrustum() {
	return Camera->getViewFrustum();
}

bool player::seesMeshNode(irr::scene::IMeshSceneNode* node) {
	irr::scene::SViewFrustum frust = *Camera->getViewFrustum();
	//transform the frustum to the node's current absolute transformation
    irr::core::matrix4 invTrans(node->getAbsoluteTransformation(), irr::core::matrix4::EM4CONST_INVERSE);
    //invTrans.makeInverse();
    frust.transform(invTrans);

	return seesBoundingBox(node->getBoundingBox(),frust);
}

bool player::seesBoundingBox(irr::core::aabbox3df bbox,irr::scene::SViewFrustum frust) {
    //irr::scene::SViewFrustum frust = *Camera->getViewFrustum();

    irr::core::vector3df edges[8];
    bbox.getEdges(edges);

    bool result = false;

    for (irr::s32 i=0; i<irr::scene::SViewFrustum::VF_PLANE_COUNT; ++i) {
        bool boxInFrustum=false;
        for (irr::u32 j=0; j<8; ++j)
        {
            if (frust.planes[i].classifyPointRelation(edges[j]) != irr::core::ISREL3D_FRONT)
            {
                boxInFrustum=true;
                break;
            }
        }

        if (!boxInFrustum)
        {
            result = true;
            break;
        }
    }
    return !result;
}

unsigned char world::pickPlayerTriangle() {
	irr::core::triangle3df hitTriangle;
	irr::core::vector3df intersection;
	irr::core::line3d<irr::f32> ray;
	ray.start = mainPlayer->getPosition();
	ray.end = ray.start + irr::core::vector3df(0.f,-204.8*RoomScale,0.f);

	irr::scene::ISceneNode * selectedSceneNode = irrColl->getSceneNodeAndCollisionPointFromRay(ray,intersection,hitTriangle);

	std::string triTexName;
	unsigned char materialType = 0;
	if (getNodeTriangleTextureName(selectedSceneNode,hitTriangle,triTexName)) {
		trimFileName(triTexName);

		if (triTexName=="metal3.jpg" ||
			triTexName=="dirtymetal.jpg" ||
			triTexName=="metalpanels.jpg" ||
			triTexName=="metalpanels2.jpg") {
			materialType = 1;
		} else {
			materialType = 0;
		}
	}

	return materialType;
}


std::string player::getItemName(unsigned char slot) {
	slot%=inventory_size;
	if (inventory[slot]==nullptr) return std::string("");
	return inventory[slot]->getInvName();
}

std::string player::getItemInvImg(unsigned char slot) {
	slot%=inventory_size;
	if (inventory[slot]==nullptr) return std::string("");
	return inventory[slot]->getInvImgPath();
}
