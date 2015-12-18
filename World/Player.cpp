#include "World.h"
#include "Items/Items.h"
#include "Player.h"

Player::Player(World* own,irr::scene::ISceneManager* smgr,irrDynamics* dyn,MainEventReceiver* receiver,float iheight,float iradius,float mass) {
    owner = own;
    irrSmgr = smgr;
    dynamics = dyn;
    irrReceiver = receiver;

	height = iheight*RoomScale;
	radius = iradius*RoomScale;

    selfRotation.X = selfRotation.Y = selfRotation.Z = 0;

	for (unsigned char i=0;i<5;i++) {
		breathSound[i][0] = Sound::getSound(std::string("SFX/9341/breath")+std::to_string(i)+std::string(".ogg"),false,1);
		breathSound[i][1] = Sound::getSound(std::string("SFX/9341/breath")+std::to_string(i)+std::string("gas.ogg"),false,1);
	}

	for (unsigned char i=0;i<4;i++) {
		stepSound[0][0][i] = Sound::getSound(std::string("SFX/Step")+std::to_string(i+1)+std::string(".ogg"),false,1);
		stepSound[0][1][i] = Sound::getSound(std::string("SFX/Run")+std::to_string(i+1)+std::string(".ogg"),false,1);
		stepSound[1][0][i] = Sound::getSound(std::string("SFX/StepMetal")+std::to_string(i+1)+std::string(".ogg"),false,1);
		stepSound[1][1][i] = Sound::getSound(std::string("SFX/RunMetal")+std::to_string(i+1)+std::string(".ogg"),false,1);
		if (i<3) {
			stepSound[2][0][i] = Sound::getSound(std::string("SFX/StepPD")+std::to_string(i+1)+std::string(".ogg"),false,1);
			stepSound[3][0][i] = Sound::getSound(std::string("SFX/StepForest")+std::to_string(i+1)+std::string(".ogg"),false,1);
		}
	}

    // Add camera
    camera = irrSmgr->addCameraSceneNode(0,irr::core::vector3df(0,0,0),irr::core::vector3df(0,0,-1));
    camera->setPosition(irr::core::vector3df(15*204.8f*RoomScale, 10*RoomScale, 15*204.8f*RoomScale));
    camera->setTarget(irr::core::vector3df(0, 0, 0));
    camera->setNearValue(3.0*RoomScale);
    camera->setFarValue(200.0*RoomScale);
    camera->setFOV(70.f*irr::core::DEGTORAD);

    irrSmgr->setActiveCamera(camera);

//#if 0
    //Add capsule
    capsule = dynamics->addPlayerColliderObject(camera,height*0.95f,radius,mass,1,1);//dynamics->addCapsuleObject(camera,height,radius,mass,~0,~0);//;
    capsule->setAngularFactor(btVector3(0,0,0)); //don't let the capsule rotate until the Player dies
    capsule->setSleepingThresholds (0.0, 0.0);
    capsule->setGravity(btVector3(0.f,-450.f*RoomScale,0.f));
	capsule->setActivationState(DISABLE_DEACTIVATION);
//#endif

    // Create the shape
    btConvexHullShape *mShape = new btConvexHullShape();

	irr::f32 oRadius = radius;
	irr::f32 oheight = height-oRadius*0.1f;
	irr::f32 oradius = radius-oRadius*0.1f;
	for (int i=0;i<50;i++) {
		irr::f32 fi = (irr::f32)i*7.2f*irr::core::DEGTORAD;
		mShape->addPoint(btVector3(std::cos(fi)*0.1f*oradius,-oheight*0.5f,std::sin(fi)*0.1f*oradius));
		mShape->addPoint(btVector3(std::cos(fi)*oradius,-oheight*0.5f+oradius*1.1f,std::sin(fi)*oradius));
		mShape->addPoint(btVector3(std::cos(fi)*oradius,0.f,std::sin(fi)*oradius));
	}
	mShape->setMargin(oRadius*0.02f);

	standingShape = capsule->getCollisionShape();
	crouchingShape = mShape;

//#ifdef PLAYER_PENETRATION_RECOVER
	ghostObject = new btPairCachingGhostObject();
	ghostObject->setWorldTransform(capsule->getCenterOfMassTransform());
	ghostObject->setCollisionShape(standingShape);
	capsule->getCollisionShape()->setLocalScaling(btVector3(1.f,1.f,1.f));
	ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	dynamics->getCollisionWorld()->addCollisionObject(ghostObject,btBroadphaseProxy::CharacterFilter,btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);

	controller = new CharacterController(ghostObject,static_cast<btConvexShape*>(standingShape),500.f,1);

	linearVelocity.setValue(0,0,0);
//#endif

    //clear inventory
    for (irr::u32 i=0;i<inventory_size;i++) {
        inventory[i]=nullptr;
    }
}

Player::~Player() {
	//dynamics->unregisterRBody(capsule);
	if (irrSmgr->getActiveCamera()==camera) irrSmgr->setActiveCamera(nullptr);
	camera->remove();
	for (unsigned char i=0;i<5;i++) {
		if (breathSound[i][0]!=nullptr) breathSound[i][0]->drop();
		if (breathSound[i][1]!=nullptr) breathSound[i][1]->drop();
	}
	for (unsigned char i=0;i<4;i++) {
		if (stepSound[0][0][i]!=nullptr) stepSound[0][0][i]->drop();
		if (stepSound[0][1][i]!=nullptr) stepSound[0][1][i]->drop();
		if (stepSound[1][0][i]!=nullptr) stepSound[1][0][i]->drop();
		if (stepSound[1][1][i]!=nullptr) stepSound[1][1][i]->drop();
		if (i<3) {
			if (stepSound[2][0][i]!=nullptr) stepSound[2][0][i]->drop();
			if (stepSound[3][0][i]!=nullptr) stepSound[3][0][i]->drop();
		}
	}
}

void Player::teleport(irr::core::vector3df position) {
    controller->teleport(irrToBtVec(position));
	/*btTransform oTrans = capsule->getCenterOfMassTransform();
	oTrans.setOrigin(btVector3(position.X,position.Y,position.Z));
	capsule->setCenterOfMassTransform(oTrans);
	capsule->setLinearVelocity(btVector3(0,0,0));*/
}

void Player::resetSpeeds() {
	//capsule->setLinearVelocity(btVector3(dynSpeed[0],capsule->getLinearVelocity()[1],dynSpeed[2]));
}

void Player::booststamina(float add,float clamp) {
    stamina+=add;
    stamina = std::min(stamina,100.f);
    if (clamp>0.f) {
        if (stamina<clamp) {
            stamina = (clamp*0.05f+stamina*(1.f-0.05f));
        }
    } else {
        if (stamina>-clamp) {
            stamina = (-clamp*0.05f+stamina*(1.f-0.05f));
        }
    }
}

void Player::update() {

    blinkTimer-=0.09f;

    if (blinkTimer<=-1.f) {
        blinkTimer = 100.f;
    }

    if (irrReceiver->IsKeyDown(irr::KEY_SPACE)) {
        if (blinkTimer>0.f) { blinkTimer = 0.f; }
        if (blinkTimer<-0.5f) { blinkTimer = -0.5f; }
    }

    while (yaw>180.0) { yaw-=360.0; }
    while (yaw<-180.0) { yaw+=360.0; }

    while (pitch>180.0) { pitch-=360.0; }
    while (pitch<-180.0) { pitch+=360.0; }

    if (pitch>70.0) pitch = 70.0;
    if (pitch<-70.0) pitch = -70.0;

    //float dir = 0.f;
    float prevShake = shake;
    if (shakeFactor>0.f) {
        shake+=shakeFactor;
    }
    while (shake>720.f) shake-=720.f;
    while (shake<0.f) shake+=720.f;

    if (!noclip) {
        if (irrReceiver->IsKeyDown(irr::KEY_KEY_N) && !irrReceiver->IsPrevKeyDown(irr::KEY_KEY_N)) {
			noclip = true;
			irrReceiver->perLoopUpdate();
		}

        bool isWalking = false;
        bool isForced = false;
        if (forceDir.getLengthSQ()>0.01f) {
            isForced = true;
        }
        if (irrReceiver->IsKeyDown(irr::KEY_KEY_W)) {
            isWalking = true;
            dir = 0.f;
            if (irrReceiver->IsKeyDown(irr::KEY_KEY_A)) {
                dir -= 45;
            } else if (irrReceiver->IsKeyDown(irr::KEY_KEY_D)) {
                dir += 45;
            }
        } else if (irrReceiver->IsKeyDown(irr::KEY_KEY_S)) {
            isWalking = true;
            dir = 180;
            if (irrReceiver->IsKeyDown(irr::KEY_KEY_A)) {
                dir += 45;
            } else if (irrReceiver->IsKeyDown(irr::KEY_KEY_D)) {
                dir -= 45;
            }
        } else {
            if (irrReceiver->IsKeyDown(irr::KEY_KEY_A)) {
                isWalking = true;
                dir = -90;
            } else if (irrReceiver->IsKeyDown(irr::KEY_KEY_D)) {
                isWalking = true;
                dir = 90;
            }
        }

        irr::core::vector3df d(0.f,0.f,0.f);// = (tTarget-camera->getPosition());
        shakeFactor = 0.f;
        if (isWalking || isForced) {
            float walkSpeed = 0.f;

            if (isWalking) {
                walkSpeed = 13.f;
                if (crouchState<0.015f) {
                    if (irrReceiver->IsKeyDown(irr::KEY_LSHIFT)) {
                        if (sprintTimer>0.f && sprintTimer<10.f) stamina-=5.f;
                        sprintTimer = 20.f;
                        if (stamina>0) walkSpeed=40.f-((100.f-stamina)*(100.f-stamina)*0.001);
                        stamina-=0.2;
                        if (stamina<0) stamina = -10.f;

                        if (stamina<5.f) {
                            if (!breathSound[currBreathSound][0]->isPlaying()) {
                                currBreathSound = 0;
                                breathSound[currBreathSound][0]->playSound(false);
                            }
                        } else if (stamina<50.f) {
                            if (!breathSound[currBreathSound][0]->isPlaying()) {
                                currBreathSound = (rand()%3)+1;
                                breathSound[currBreathSound][0]->playSound(false,(50.f-stamina)/50.f);
                            }
                            //stamina = 100.f;
                        }
                    } else {
                        stamina=std::min(stamina+0.15f,100.f);
                    }
                } else {
                    if (crouchState>0.4f) walkSpeed = 9.f; else walkSpeed = 13.f;
                    stamina=std::min(stamina+0.15f,100.f);
                }

                d.X = std::cos((dir+yaw-90.f)*irr::core::DEGTORAD);
                d.Y = 0.f;
                d.Z = -std::sin((dir+yaw-90.f)*irr::core::DEGTORAD);
                d = d.normalize();
                d *= walkSpeed*0.025f*RoomScale/0.75f;
            } else {
                stamina=std::min(stamina+0.15f,100.f);
            }
            if (isForced) {
                walkSpeed+=forceDir.getLength()*13.f;
            }

            shakeFactor = sqrt(walkSpeed/13.f)*7.f;
            //std::cout<<"::: "<<shakeFactor<<"\n";
            if ((prevShake<=270.f && shake>270.f) || (prevShake<=630.f && shake>630.f)) {
                unsigned char chosen = rand()%4;
                stepSound[owner->pickPlayerTriangle()][walkSpeed>13.f][chosen]->playSound(false,std::min(walkSpeed/13.f,1.f));
            }
        } else {
            d = -btToIrrVec(linearVelocity);
            stamina=std::min(stamina+0.15f,100.f);
        }

        linearVelocity = (linearVelocity*0.8f)+irrToBtVec(d*0.2f);
        controller->setWalkDirection(linearVelocity*playerControl+irrToBtVec(forceDir*13.f*0.025f*RoomScale/0.75f));
        playerControl = 1.f;
        forceDir=irr::core::vector3df(0.f,0.f,0.f);
        controller->updateAction(dynamics->getCollisionWorld(),1.f/60.f);

        if (irrReceiver->IsKeyDown(irr::KEY_LCONTROL)) {
            crouched = true;
        } else if (crouched) {
            btVector3 start = irrToBtVec(getPosition());
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

        bool changed = false;
        if (crouched) {
            float prevState = crouchState;
            crouchState=std::min(0.5f,crouchState+(0.5f-crouchState)*0.2f);
            if (crouchState>0.4998f) crouchState = 0.5f;

            if (prevState<0.5f && crouchState>=0.5f) { changed = true; }

            if (changed) {
                controller->m_convexShape = static_cast<btConvexShape*>(crouchingShape);
                controller->getGhostObject()->setCollisionShape(crouchingShape);
            }
        } else {
            float prevState = crouchState;
            crouchState=std::max(0.f,crouchState+(-crouchState)*0.1f);
            if (crouchState<0.002f) crouchState = 0.f;

            if (prevState>=0.5f && crouchState<0.5f) { changed = true; }

            if (changed) {
                controller->m_convexShape = static_cast<btConvexShape*>(standingShape);
                controller->getGhostObject()->setCollisionShape(standingShape);
            }
        }

        if (wearingGasMask<inventory_size) {
            if (inventory[wearingGasMask]!=nullptr) {
                inventory[wearingGasMask]->updateWearing();
            } else {
                wearingGasMask = inventory_size;
            }
        }

        if (wearing714<inventory_size) {
            if (inventory[wearing714]!=nullptr) {
                inventory[wearing714]->updateWearing();
            } else {
                wearing714 = inventory_size;
            }
        }

        if (selectedItem<inventory_size) {
            if (inventory[selectedItem]==nullptr) {
                selectedItem = inventory_size;
            } else if (inventory[selectedItem]->updateItem()==false) {
                selectedItem = inventory_size;
            }
        }
    } else {

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
		dir+=yaw;
		dir-=90;
		dir *= irr::core::DEGTORAD;
		shake = 0.f;
		if (irrReceiver->IsKeyDown(irr::KEY_KEY_W) || irrReceiver->IsKeyDown(irr::KEY_KEY_A) || irrReceiver->IsKeyDown(irr::KEY_KEY_S) || irrReceiver->IsKeyDown(irr::KEY_KEY_D)) {
            float yMult = 1.f;
            if (!irrReceiver->IsKeyDown(irr::KEY_KEY_W)) { yMult = 0.f; }
            if (irrReceiver->IsKeyDown(irr::KEY_KEY_S)) { yMult=-1.f; }
            camera->setPosition(camera->getAbsolutePosition()+irr::core::vector3df(std::cos(dir)*std::cos(pitch*irr::core::DEGTORAD)*RoomScale,-std::sin(pitch*irr::core::DEGTORAD)*RoomScale*yMult,-std::sin(dir)*std::cos(pitch*irr::core::DEGTORAD)*RoomScale));
            camera->updateAbsolutePosition();
            controller->teleport(irrToBtVec(camera->getAbsolutePosition()-irr::core::vector3df(0.f,height*0.5f,0.f)));
        }

		if (irrReceiver->IsKeyDown(irr::KEY_KEY_N) && !irrReceiver->IsPrevKeyDown(irr::KEY_KEY_N)) {
			noclip = false;
			camera->updateAbsolutePosition();
			teleport(camera->getAbsolutePosition()-irr::core::vector3df(0.f,height*0.5f,0.f));
			irrReceiver->perLoopUpdate();
		}
	}
}

void Player::resetCam() {
    camera->setPosition(tPos); camera->updateAbsolutePosition();
	camera->setTarget(tTarget);
	camera->setUpVector(tUpVec);
}

void Player::reflectNX() {
    resetCam();

    irr::core::vector3df prevTarget(camera->getTarget()-camera->getAbsolutePosition());
    irr::core::vector3df prevUpVector(camera->getUpVector());

    irr::core::vector3df refPos;
    owner->pickPlayerTriangle(&refPos,irr::core::vector3df(-500.f,0.f,0.f));
    camera->setPosition(refPos-irr::core::vector3df(camera->getAbsolutePosition().X-refPos.X,0.f,0.f));

    camera->updateAbsolutePosition();

    prevTarget.X=-prevTarget.X;
    camera->setTarget(camera->getAbsolutePosition()+prevTarget);
    prevUpVector.X=-prevUpVector.X;
    camera->setUpVector(prevUpVector);
}

void Player::reflectPX() {
    resetCam();

    irr::core::vector3df prevTarget(camera->getTarget()-camera->getAbsolutePosition());
    irr::core::vector3df prevUpVector(camera->getUpVector());

    irr::core::vector3df refPos;
    owner->pickPlayerTriangle(&refPos,irr::core::vector3df(500.f,0.f,0.f));
    camera->setPosition(refPos+irr::core::vector3df(refPos.X-camera->getAbsolutePosition().X,0.f,0.f));

    camera->updateAbsolutePosition();

    prevTarget.X=-prevTarget.X;
    camera->setTarget(camera->getAbsolutePosition()+prevTarget);
    prevUpVector.X=-prevUpVector.X;
    camera->setUpVector(prevUpVector);
}

void Player::reflectNY() {
    resetCam();

    irr::core::vector3df prevTarget(camera->getTarget()-camera->getAbsolutePosition());
    irr::core::vector3df prevUpVector(camera->getUpVector());

    irr::core::vector3df refPos;
    owner->pickPlayerTriangle(&refPos,irr::core::vector3df(0.f,-500.f,0.f));
    camera->setPosition(refPos-irr::core::vector3df(0.f,camera->getAbsolutePosition().Y-refPos.Y,0.f));

    camera->updateAbsolutePosition();

    prevTarget.Y=-prevTarget.Y;
    camera->setTarget(camera->getAbsolutePosition()+prevTarget);
    prevUpVector.Y=-prevUpVector.Y;
    camera->setUpVector(prevUpVector);
    camera->updateMatrices();
}

void Player::reflectPY() {
    resetCam();

    irr::core::vector3df prevTarget(camera->getTarget()-camera->getAbsolutePosition());
    irr::core::vector3df prevUpVector(camera->getUpVector());

    irr::core::vector3df refPos;
    owner->pickPlayerTriangle(&refPos,irr::core::vector3df(0.f,500.f,0.f));
    camera->setPosition(refPos+irr::core::vector3df(0.f,refPos.Y-camera->getAbsolutePosition().Y,0.f));

    camera->updateAbsolutePosition();

    prevTarget.Y=-prevTarget.Y;
    camera->setTarget(camera->getAbsolutePosition()+prevTarget);
    prevUpVector.Y=-prevUpVector.Y;
    camera->setUpVector(prevUpVector);
}

void Player::reflectNZ() {
    resetCam();

    irr::core::vector3df prevTarget(camera->getTarget()-camera->getAbsolutePosition());
    irr::core::vector3df prevUpVector(camera->getUpVector());

    irr::core::vector3df refPos;
    owner->pickPlayerTriangle(&refPos,irr::core::vector3df(0.f,0.f,-500.f));
    camera->setPosition(refPos-irr::core::vector3df(0.f,0.f,camera->getAbsolutePosition().Z-refPos.Z));

    camera->updateAbsolutePosition();

    prevTarget.Z=-prevTarget.Z;
    camera->setTarget(camera->getAbsolutePosition()+prevTarget);
    prevUpVector.Z=-prevUpVector.Z;
    camera->setUpVector(prevUpVector);
}

void Player::reflectPZ() {
    resetCam();

    irr::core::vector3df prevTarget(camera->getTarget()-camera->getAbsolutePosition());
    irr::core::vector3df prevUpVector(camera->getUpVector());

    irr::core::vector3df refPos;
    owner->pickPlayerTriangle(&refPos,irr::core::vector3df(0.f,0.f,500.f));
    camera->setPosition(refPos+irr::core::vector3df(0.f,0.f,refPos.Z-camera->getAbsolutePosition().Z));

    camera->updateAbsolutePosition();

    prevTarget.Z=-prevTarget.Z;
    camera->setTarget(camera->getAbsolutePosition()+prevTarget);
    prevUpVector.Z=-prevUpVector.Z;
    camera->setUpVector(prevUpVector);
}

void Player::updateHead() {

	if (!dead) {
#ifdef PLAYER_PENETRATION_RECOVER
		if (crouchState<=0.f) {
			ghostObject->setWorldTransform(capsule->getCenterOfMassTransform());
			unsigned char rfp = 0;
			while (rfp++<8 && recoverFromPenetration()) {}
			capsule->setCenterOfMassTransform(ghostObject->getWorldTransform());
			btTransform Transform;
			capsule->getMotionState()->getWorldTransform(Transform);
			btVector3 Point = Transform.getOrigin();
			camera->setPosition(irr::core::vector3df(Point[0],Point[1]+(height/2.f)-(radius/1.5f),Point[2]));
		}
#endif
        if (!noclip) {
            camera->setPosition(btToIrrVec(controller->getPosition())+irr::core::vector3df(0.f,((height*0.55f)*(1.f-crouchState))-(radius*0.666f),0.f));
        } else {
            camera->setPosition(btToIrrVec(controller->getPosition())+irr::core::vector3df(0.f,height*0.5f,0.f));
        }
        camera->updateAbsolutePosition();
        camera->setPosition(camera->getAbsolutePosition()+irr::core::vector3df(0.f,std::sin(shake*irr::core::DEGTORAD)*RoomScale/0.75f*(0.4f-(crouchState*0.4f)),0.f));
	}
	//std::cout<<camera->getAbsolutePosition().X<<" "<<camera->getAbsolutePosition().Y<<" "<<camera->getAbsolutePosition().Z<<"\n";

    irr::core::vector3df cdir(0,1,0);
    irr::core::matrix4 rotMatrix;

    selfRotation.X = pitch;
    selfRotation.Y = yaw;

    rotMatrix.setRotationDegrees(irr::core::vector3df(0,0,std::max(std::min(std::sin((shake)*irr::core::DEGTORAD/2.f)*2.5f,8.f),-8.f)*0.25f));
    rotMatrix.transformVect(cdir);

    rotMatrix.setRotationDegrees(selfRotation);
    rotMatrix.transformVect(cdir);

    rotMatrix.setRotationDegrees(camera->getRotation());
    rotMatrix.transformVect(cdir);
    camera->setUpVector(cdir);
    cdir.X = 0;
    cdir.Y = 0;
    cdir.Z = 1;

    rotMatrix.setRotationDegrees(selfRotation);
    rotMatrix.transformVect(cdir);

    rotMatrix.setRotationDegrees(camera->getRotation());
    rotMatrix.transformVect(cdir);
    camera->updateAbsolutePosition();

    Sound::setListenerPos(camera->getAbsolutePosition());
    Sound::setListenerOrientation(camera->getUpVector(),cdir);

    camera->setTarget(camera->getAbsolutePosition()+cdir);

	lastMouseDown[0] = irrReceiver->IsMouseDown(0);
	lastMouseDown[1] = irrReceiver->IsMouseDown(1);

	tPos = camera->getAbsolutePosition();
	tTarget = camera->getTarget();
	tUpVec = camera->getUpVector();
}

bool Player::addToInventory(Item* it) {
    for (irr::u32 i=0;i<inventory_size;i++) {
        if (inventory[i]==nullptr) {
            inventory[i]=it;
            it->pick();
            return true;
        }
    }
    return false;
}

Item* Player::takeFromInventory(unsigned char slot,bool rmove) {
	slot%=inventory_size;
	if (inventory[slot]!=nullptr) {
        Item* it = inventory[slot];
        if (rmove) {
            camera->updateAbsolutePosition();
            inventory[slot]->unpick(camera->getAbsolutePosition());
            inventory[slot]=nullptr;
        }
		return it;
	}
	return nullptr;
}

const irr::scene::SViewFrustum* Player::getViewFrustum() {
	return camera->getViewFrustum();
}

bool Player::seesMeshNode(irr::scene::IMeshSceneNode* node) {
	irr::scene::SViewFrustum frust = *camera->getViewFrustum();
	//transform the frustum to the node's current absolute transformation
    irr::core::matrix4 invTrans(node->getAbsoluteTransformation(), irr::core::matrix4::EM4CONST_INVERSE);
    //invTrans.makeInverse();
    frust.transform(invTrans);

	return seesBoundingBox(node->getBoundingBox(),frust);
}

bool Player::seesBoundingBox(irr::core::aabbox3df bbox,irr::scene::SViewFrustum frust) {
    //irr::scene::SViewFrustum frust = *camera->getViewFrustum();

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

unsigned char World::pickPlayerTriangle(irr::core::vector3df* intersec,const irr::core::vector3df customEnd) {
	irr::core::triangle3df hitTriangle;
	irr::core::vector3df intersection;
	irr::core::line3d<irr::f32> ray;
	ray.start = mainPlayer->getPosition();
    ray.end = ray.start + customEnd;

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

    if (intersec!=nullptr) { *intersec = intersection; }

	return materialType;
}

/*
std::string Player::getItemName(unsigned char slot) {
	slot%=inventory_size;
	if (inventory[slot]==nullptr) return std::string("");
	return inventory[slot]->getInvName();
}

std::string Player::getItemInvImg(unsigned char slot) {
	slot%=inventory_size;
	if (inventory[slot]==nullptr) return std::string("");
	return inventory[slot]->getInvImgPath();
}

ItemTempIDs Player::getItemTempID(unsigned char slot) {
    slot%=inventory_size;
    if (inventory[slot]==nullptr) return ItemTempIDs::ITEM_NULL;
    return inventory[slot]->getTempID();
}
*/

unsigned char Player::moveToSlot(unsigned char srcSlot,unsigned char destSlot) {
	srcSlot%=inventory_size; destSlot%=inventory_size;
	if (inventory[srcSlot]==nullptr) return 0;
	if (inventory[destSlot]==nullptr) {
		inventory[destSlot]=inventory[srcSlot];
		inventory[srcSlot]=nullptr;
		return 1;
	} else { //TODO: add item combination

		return 0;//return 2;
	}
}

void Player::selectItem(unsigned char index,bool temporary) {
    prevSelectedItem = selectedItem;
    selectedItem = index;
    temporarySelection = temporary;
}

void Player::selectPrevItem() {
    if (temporarySelection) {
        selectedItem = prevSelectedItem;
        prevSelectedItem = inventory_size;
        temporarySelection = false;
    }
}

void Player::selectGasMask(Item* it) {
    for (unsigned char i=0;i<inventory_size;i++) {
        if (inventory[i]==it) {
            if (i==wearingGasMask) {
                wearingGasMask = inventory_size;
            } else {
                wearingGasMask = i;
            }
        }
    }
}

void Player::select714(Item* it) {
    for (unsigned char i=0;i<inventory_size;i++) {
        if (inventory[i]==it) {
            if (i==wearing714) {
                wearing714 = inventory_size;
            } else {
                wearing714 = i;
            }
        }
    }
}

void Player::setVisible(bool isVisible) {
    camera->setVisible(isVisible);
}

bool Player::drawSelectedItem() {

    if (wearingGasMask<inventory_size) {
        if (inventory[wearingGasMask]!=nullptr) {
            inventory[wearingGasMask]->drawItem();
        } else {
            wearingGasMask = inventory_size;
        }
    }

    if (selectedItem<inventory_size) {
        if (inventory[selectedItem]!=nullptr) {
            inventory[selectedItem]->drawItem();
            return true;
        } else {
            selectedItem = inventory_size;
            return false;
        }
    } else {
        return false;
    }
}

unsigned char Player::getSelectedItem() {
    return selectedItem;
}

void Player::forceWalk(irr::core::vector3df inForcedDir,float inPlayerControl) {
    forceDir = inForcedDir; playerControl = inPlayerControl;
}
