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
		breathSound[i][0] = sound::getSound(std::string("SFX/9341/breath")+std::to_string(i)+std::string(".ogg"),false,1);
		breathSound[i][1] = sound::getSound(std::string("SFX/9341/breath")+std::to_string(i)+std::string("gas.ogg"),false,1);
	}

	for (unsigned char i=0;i<4;i++) {
		stepSound[0][0][i] = sound::getSound(std::string("SFX/Step")+std::to_string(i+1)+std::string(".ogg"),false,1);
		stepSound[0][1][i] = sound::getSound(std::string("SFX/Run")+std::to_string(i+1)+std::string(".ogg"),false,1);
		stepSound[1][0][i] = sound::getSound(std::string("SFX/StepMetal")+std::to_string(i+1)+std::string(".ogg"),false,1);
		stepSound[1][1][i] = sound::getSound(std::string("SFX/RunMetal")+std::to_string(i+1)+std::string(".ogg"),false,1);
		if (i<3) {
			stepSound[2][0][i] = sound::getSound(std::string("SFX/StepPD")+std::to_string(i+1)+std::string(".ogg"),false,1);
			stepSound[3][0][i] = sound::getSound(std::string("SFX/StepForest")+std::to_string(i+1)+std::string(".ogg"),false,1);
		}
	}

    // Add camera
    Camera = irrSmgr->addCameraSceneNode(0,irr::core::vector3df(0,0,0),irr::core::vector3df(0,0,-1));
    Camera->setPosition(irr::core::vector3df(15*204.8f*RoomScale, 10*RoomScale, 15*204.8f*RoomScale));
    Camera->setTarget(irr::core::vector3df(0, 0, 0));
    Camera->setNearValue(3.0*RoomScale);
    Camera->setFarValue(200.0*RoomScale);
    Camera->setFOV(70.f*irr::core::DEGTORAD);

    irrSmgr->setActiveCamera(Camera);

    //Add capsule
    Capsule = dynamics->addPlayerColliderObject(Camera,height,radius,mass,1,1);
    Capsule->setAngularFactor(btVector3(0,0,0)); //don't let the capsule rotate until the player dies
    Capsule->setSleepingThresholds (0.0, 0.0);
    Capsule->setGravity(btVector3(0.f,-450.f*RoomScale,0.f));
	Capsule->setActivationState(DISABLE_DEACTIVATION);

#ifdef PLAYER_PENETRATION_RECOVER
	ghostObject = new btPairCachingGhostObject();
	ghostObject->setWorldTransform(Capsule->getCenterOfMassTransform());
	ghostObject->setCollisionShape(Capsule->getCollisionShape());
	ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	dynamics->getCollisionWorld()->addCollisionObject(ghostObject,btBroadphaseProxy::CharacterFilter,btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
#endif

    //clear inventory
    for (irr::u32 i=0;i<inventory_size;i++) {
        inventory[i]=nullptr;
    }
}

#ifdef PLAYER_PENETRATION_RECOVER
bool player::recoverFromPenetration (){
	// Here we must refresh the overlapping paircache as the penetrating movement itself or the
	// previous recovery iteration might have used setWorldTransform and pushed us into an object
	// that is not in the previous cache contents from the last timestep, as will happen if we
	// are pushed into a new AABB overlap. Unhandled this means the next convex sweep gets stuck.
	//
	// Do this by calling the broadphase's setAabb with the moved AABB, this will update the broadphase
	// paircache and the ghostobject's internal paircache at the same time.    /BW

	btCollisionWorld* collisionWorld = dynamics->getCollisionWorld();

	btVector3 minAabb, maxAabb;
	Capsule->getCollisionShape()->getAabb(ghostObject->getWorldTransform(), minAabb,maxAabb);
	collisionWorld->getBroadphase()->setAabb(ghostObject->getBroadphaseHandle(),
						 minAabb,
						 maxAabb,
						 collisionWorld->getDispatcher());

	bool penetration = false;

	//collisionWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(ghostObject->getBroadphaseHandle(),collisionWorld->getDispatcher());

	collisionWorld->getDispatcher()->dispatchAllCollisionPairs(ghostObject->getOverlappingPairCache(), collisionWorld->getDispatchInfo(), collisionWorld->getDispatcher());

	btVector3 m_currentPosition = ghostObject->getWorldTransform().getOrigin();
	btManifoldArray	m_manifoldArray;
	btVector3 m_touchingNormal;

	btScalar maxPen = btScalar(0.0);
	for (int i = 0; i < ghostObject->getOverlappingPairCache()->getNumOverlappingPairs(); i++)
	{
		m_manifoldArray.resize(0);

		btBroadphasePair* collisionPair = &ghostObject->getOverlappingPairCache()->getOverlappingPairArray()[i];

		btCollisionObject* obj0 = static_cast<btCollisionObject*>(collisionPair->m_pProxy0->m_clientObject);
		btCollisionObject* obj1 = static_cast<btCollisionObject*>(collisionPair->m_pProxy1->m_clientObject);

		btCollisionObject* compare = static_cast<btCollisionObject*>(Capsule);

		if ((obj0 && !obj0->hasContactResponse()) || (obj1 && !obj1->hasContactResponse()) || (obj0==compare) || (obj1==compare))
			continue;

		if (collisionPair->m_algorithm)
			collisionPair->m_algorithm->getAllContactManifolds(m_manifoldArray);


		for (int j=0;j<m_manifoldArray.size();j++)
		{
			btPersistentManifold* manifold = m_manifoldArray[j];
			if (manifold->getBody0()==compare || manifold->getBody1()==compare) continue;
			btScalar directionSign = manifold->getBody0() == ghostObject ? btScalar(-1.0) : btScalar(1.0);
			for (int p=0;p<manifold->getNumContacts();p++)
			{
				const btManifoldPoint&pt = manifold->getContactPoint(p);

				btScalar dist = pt.getDistance();

				if (dist < 0.0)
				{
					if (dist < maxPen)
					{
						maxPen = dist;
						m_touchingNormal = pt.m_normalWorldOnB * directionSign;//??

					}
					m_currentPosition += pt.m_normalWorldOnB * directionSign * dist * btScalar(0.2);
					penetration = true;
				} else {
					//printf("touching %f\n", dist);
				}
			}

			//manifold->clearManifold();
		}
	}
	btTransform newTrans = ghostObject->getWorldTransform();
	newTrans.setOrigin(m_currentPosition);
	ghostObject->setWorldTransform(newTrans);
//	printf("m_touchingNormal = %f,%f,%f\n",m_touchingNormal[0],m_touchingNormal[1],m_touchingNormal[2]);
	return penetration;
}
#endif

player::~player() {
	dynamics->unregisterRBody(Capsule);
	if (irrSmgr->getActiveCamera()==Camera) irrSmgr->setActiveCamera(nullptr);
	Camera->remove();
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

void player::teleport(irr::core::vector3df position) {
	btTransform oTrans = Capsule->getCenterOfMassTransform();
	oTrans.setOrigin(btVector3(position.X,position.Y,position.Z));
	Capsule->setCenterOfMassTransform(oTrans);
	Capsule->setLinearVelocity(btVector3(0,0,0));
}

void player::resetSpeeds() {
	Capsule->setLinearVelocity(btVector3(dynSpeed[0],Capsule->getLinearVelocity()[1],dynSpeed[2]));
}

void player::boostStamina(float add,float clamp) {
    Stamina+=add*owner->getFPSfactor();
    Stamina = std::min(Stamina,100.f);
    if (clamp>0.f) {
        if (Stamina<clamp) {
            Stamina = (clamp*owner->getFPSfactor()*0.05f+Stamina*(1.f-(owner->getFPSfactor()*0.05f)));
        }
    } else {
        if (Stamina>-clamp) {
            Stamina = (-clamp*owner->getFPSfactor()*0.05f+Stamina*(1.f-(owner->getFPSfactor()*0.05f)));
        }
    }
}

void player::update() {

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

    if (dynamics->rayTest(Capsule->getCenterOfMassPosition(),Capsule->getCenterOfMassPosition()-btVector3(0,height/2.f+3.f+std::max(0.f,Capsule->getLinearVelocity()[1]),0))) {
        //make the player slightly float above the ground to prevent twitching
        Capsule->setLinearVelocity(btVector3(Capsule->getLinearVelocity()[0],std::max(0.f,Capsule->getLinearVelocity()[1]),Capsule->getLinearVelocity()[2]));
    }

	dynSpeed = Capsule->getLinearVelocity();

	float fpsFactor = owner->getFPSfactor();

	BlinkTimer-=fpsFactor*0.09f;

    if (BlinkTimer<=-1.f) {
        BlinkTimer = 100.f;
    }

    if (irrReceiver->IsKeyDown(irr::KEY_SPACE)) {
        if (BlinkTimer>0.f) { BlinkTimer = 0.f; }
        if (BlinkTimer<-0.5f) { BlinkTimer = -0.5f; }
    }

    while (yaw>180.0) { yaw-=360.0; }
    while (yaw<-180.0) { yaw+=360.0; }

    while (pitch>180.0) { pitch-=360.0; }
    while (pitch<-180.0) { pitch+=360.0; }

    if (pitch>70.0) pitch = 70.0;
    if (pitch<-70.0) pitch = -70.0;

	if (!noclip) {
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
				stepSound[owner->pickPlayerTriangle()][1][chosen]->playSound(false,3.f);
			}
		}

		shakeFactor = 0.f;

		Capsule->setGravity(btVector3(0.f,-450.f*RoomScale/std::max(fpsFactor,1.f),0.f));
		Capsule->setFriction(speed.distance(btVector3(0,speed[1],0))*50.f/std::max(fpsFactor,1.f));

		if (!dead) {
			//Capsule->setFriction(0.01f/fpsFactor);
			sprintTimer=std::min(std::max(0.f,sprintTimer-0.1f*fpsFactor),10.f);
			resetSpeeds();
			if ((irrReceiver->IsKeyDown(irr::KEY_KEY_W)
				|| irrReceiver->IsKeyDown(irr::KEY_KEY_S)
				|| irrReceiver->IsKeyDown(irr::KEY_KEY_A)
				|| irrReceiver->IsKeyDown(irr::KEY_KEY_D)
				) && std::abs(speed[1]) < 20.f) {
				Capsule->setLinearVelocity(btVector3(speed[0],speed[1]-(speed[1]*0.5f*fpsFactor),speed[2]));
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
								breathSound[currBreathSound][0]->playSound(false,(50.f-Stamina)/50.f);
							}
							//Stamina = 100.f;
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
					stepSound[owner->pickPlayerTriangle()][walkSpeed>40.f][chosen]->playSound(false,std::min(walkSpeed/40.f,1.f));
				}

				dir-=90;
				dir *= irr::core::DEGTORAD;

				if (walkingSpeed>walkSpeed) {
					walkingSpeed=std::max(walkSpeed,walkingSpeed+(walkSpeed-walkingSpeed)*0.25f*fpsFactor);
				} else {
					walkingSpeed=std::min(walkingSpeed+(walkSpeed-walkingSpeed)*0.05f*fpsFactor,walkSpeed);
				}

				shakeFactor = walkSpeed;
			} else {
				Stamina=std::min(Stamina+0.15*fpsFactor,100.0);
				if (std::abs(speed[1])<15.f) {
					walkingSpeed = walkingSpeed-(walkingSpeed*0.5f*fpsFactor);
					if (walkingSpeed<=0.02f) walkingSpeed = 0.f;
				} else {
					if (walkingSpeed>=70.f) walkingSpeed=std::max(70.f,walkingSpeed-(walkingSpeed*0.1f*fpsFactor));
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
			//Capsule->setFriction(3.0f);
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

		prevLinearVelocity = Capsule->getLinearVelocity();

		if (irrReceiver->IsKeyDown(irr::KEY_KEY_N) && !irrReceiver->IsPrevKeyDown(irr::KEY_KEY_N)) {
			noclip = true;
			Capsule->setLinearVelocity(btVector3(0.f,0.f,0.f));
			dynamics->unregisterRBody(Capsule);
			irrReceiver->perLoopUpdate();
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
		Camera->setPosition(Camera->getAbsolutePosition()+irr::core::vector3df(std::cos(dir)*std::cos(pitch*irr::core::DEGTORAD),-std::sin(pitch*irr::core::DEGTORAD),-std::sin(dir)*std::cos(pitch*irr::core::DEGTORAD)));

		if (irrReceiver->IsKeyDown(irr::KEY_KEY_N) && !irrReceiver->IsPrevKeyDown(irr::KEY_KEY_N)) {
			noclip = false;
			dynamics->registerNewRBody(Camera,Capsule,-1,1,1,irr::core::vector3df(0,-(height/2.f)+(radius/1.5f),0));
			btTransform newTrans = Capsule->getCenterOfMassTransform();
			newTrans.setOrigin(btVector3(Camera->getAbsolutePosition().X,Camera->getAbsolutePosition().Y,Camera->getAbsolutePosition().Z));
			Capsule->setCenterOfMassTransform(newTrans);
			irrReceiver->perLoopUpdate();
		}
	}
}

void player::resetCam() {
    Camera->setPosition(tPos); Camera->updateAbsolutePosition();
	Camera->setTarget(tTarget);
	Camera->setUpVector(tUpVec);
}

void player::reflectNX() {
    resetCam();

    irr::core::vector3df prevTarget(Camera->getTarget()-Camera->getAbsolutePosition());
    irr::core::vector3df prevUpVector(Camera->getUpVector());

    irr::core::vector3df refPos;
    owner->pickPlayerTriangle(&refPos,irr::core::vector3df(-500.f,0.f,0.f));
    Camera->setPosition(refPos-irr::core::vector3df(Camera->getAbsolutePosition().X-refPos.X,0.f,0.f));

    Camera->updateAbsolutePosition();

    prevTarget.X=-prevTarget.X;
    Camera->setTarget(Camera->getAbsolutePosition()+prevTarget);
    prevUpVector.X=-prevUpVector.X;
    Camera->setUpVector(prevUpVector);
}

void player::reflectPX() {
    resetCam();

    irr::core::vector3df prevTarget(Camera->getTarget()-Camera->getAbsolutePosition());
    irr::core::vector3df prevUpVector(Camera->getUpVector());

    irr::core::vector3df refPos;
    owner->pickPlayerTriangle(&refPos,irr::core::vector3df(500.f,0.f,0.f));
    Camera->setPosition(refPos+irr::core::vector3df(refPos.X-Camera->getAbsolutePosition().X,0.f,0.f));

    Camera->updateAbsolutePosition();

    prevTarget.X=-prevTarget.X;
    Camera->setTarget(Camera->getAbsolutePosition()+prevTarget);
    prevUpVector.X=-prevUpVector.X;
    Camera->setUpVector(prevUpVector);
}

void player::reflectNY() {
    resetCam();

    irr::core::vector3df prevTarget(Camera->getTarget()-Camera->getAbsolutePosition());
    irr::core::vector3df prevUpVector(Camera->getUpVector());

    irr::core::vector3df refPos;
    owner->pickPlayerTriangle(&refPos,irr::core::vector3df(0.f,-500.f,0.f));
    Camera->setPosition(refPos-irr::core::vector3df(0.f,Camera->getAbsolutePosition().Y-refPos.Y,0.f));

    Camera->updateAbsolutePosition();

    prevTarget.Y=-prevTarget.Y;
    Camera->setTarget(Camera->getAbsolutePosition()+prevTarget);
    prevUpVector.Y=-prevUpVector.Y;
    Camera->setUpVector(prevUpVector);
}

void player::reflectPY() {
    resetCam();

    irr::core::vector3df prevTarget(Camera->getTarget()-Camera->getAbsolutePosition());
    irr::core::vector3df prevUpVector(Camera->getUpVector());

    irr::core::vector3df refPos;
    owner->pickPlayerTriangle(&refPos,irr::core::vector3df(0.f,500.f,0.f));
    Camera->setPosition(refPos+irr::core::vector3df(0.f,refPos.Y-Camera->getAbsolutePosition().Y,0.f));

    Camera->updateAbsolutePosition();

    prevTarget.Y=-prevTarget.Y;
    Camera->setTarget(Camera->getAbsolutePosition()+prevTarget);
    prevUpVector.Y=-prevUpVector.Y;
    Camera->setUpVector(prevUpVector);
}

void player::reflectNZ() {
    resetCam();

    irr::core::vector3df prevTarget(Camera->getTarget()-Camera->getAbsolutePosition());
    irr::core::vector3df prevUpVector(Camera->getUpVector());

    irr::core::vector3df refPos;
    owner->pickPlayerTriangle(&refPos,irr::core::vector3df(0.f,0.f,-500.f));
    Camera->setPosition(refPos-irr::core::vector3df(0.f,0.f,Camera->getAbsolutePosition().Z-refPos.Z));

    Camera->updateAbsolutePosition();

    prevTarget.Z=-prevTarget.Z;
    Camera->setTarget(Camera->getAbsolutePosition()+prevTarget);
    prevUpVector.Z=-prevUpVector.Z;
    Camera->setUpVector(prevUpVector);
}

void player::reflectPZ() {
    resetCam();

    irr::core::vector3df prevTarget(Camera->getTarget()-Camera->getAbsolutePosition());
    irr::core::vector3df prevUpVector(Camera->getUpVector());

    irr::core::vector3df refPos;
    owner->pickPlayerTriangle(&refPos,irr::core::vector3df(0.f,0.f,500.f));
    Camera->setPosition(refPos+irr::core::vector3df(0.f,0.f,refPos.Z-Camera->getAbsolutePosition().Z));

    Camera->updateAbsolutePosition();

    prevTarget.Z=-prevTarget.Z;
    Camera->setTarget(Camera->getAbsolutePosition()+prevTarget);
    prevUpVector.Z=-prevUpVector.Z;
    Camera->setUpVector(prevUpVector);
}

void player::updateHead() {

	if (!dead) {
#ifdef PLAYER_PENETRATION_RECOVER
		if (crouchState<=0.f) {
			ghostObject->setWorldTransform(Capsule->getCenterOfMassTransform());
			unsigned char rfp = 0;
			while (rfp++<8 && recoverFromPenetration()) {}
			Capsule->setCenterOfMassTransform(ghostObject->getWorldTransform());
			btTransform Transform;
			Capsule->getMotionState()->getWorldTransform(Transform);
			btVector3 Point = Transform.getOrigin();
			Camera->setPosition(irr::core::vector3df(Point[0],Point[1]+(height/2.f)-(radius/1.5f),Point[2]));
		}
#endif
		Camera->updateAbsolutePosition();
		Camera->setPosition(Camera->getAbsolutePosition()+irr::core::vector3df(0.f,std::sin(shake*irr::core::DEGTORAD)*(0.4f-(crouchState*0.4f)),0.f));
	}
	Camera->updateAbsolutePosition();

	//std::cout<<Camera->getAbsolutePosition().X<<" "<<Camera->getAbsolutePosition().Y<<" "<<Camera->getAbsolutePosition().Z<<"\n";

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

	tPos = Camera->getAbsolutePosition();
	tTarget = Camera->getTarget();
	tUpVec = Camera->getUpVector();
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

unsigned char world::pickPlayerTriangle(irr::core::vector3df* intersec,const irr::core::vector3df customEnd) {
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

unsigned char player::moveToSlot(unsigned char srcSlot,unsigned char destSlot) {
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

void player::selectItem(unsigned char index) {
    selectedItem = index;
}

void player::selectGasMask(item* it) {
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

void player::select714(item* it) {
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

void player::setVisible(bool isVisible) {
    Camera->setVisible(isVisible);
}

bool player::drawSelectedItem() {

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
