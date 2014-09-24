#include "3dworld.h"
//#include "rooms/rmesh.h"
#include "rooms/room.h"

#include <iostream>
#include <cmath>

world::world(unsigned int width,unsigned int height,bool fullscreen) {
    mainWidth = width; mainHeight = height;

    irrDriverType = irr::video::EDT_OPENGL;
    irrReceiver = new MainEventReceiver;
    irrDevice = irr::createDevice(irrDriverType,irr::core::dimension2d<irr::u32>(width,height),32,fullscreen,false,true,irrReceiver);
    irrDriver = irrDevice->getVideoDriver();
	irrSmgr = irrDevice->getSceneManager();

	//irrSmgr->setAmbientLight(irr::video::SColor(255,20,20,20));

    irrDriver->setTextureCreationFlag(irr::video::ETCF_ALWAYS_32_BIT, true);

    irrEnv = irrDevice->getGUIEnvironment();

    irrFileSystem = irrDevice->getFileSystem();

    irrTimer = irrDevice->getTimer();

    irr::video::IGPUProgrammingServices* irrGpu = irrDriver->getGPUProgrammingServices();
    RoomShader = irr::video::EMT_LIGHTMAP; // Fallback material type
    RoomCallback = new RoomShaderCallBack;
    RoomShader = irrGpu->addHighLevelShaderMaterialFromFiles("RoomVertShader.txt", "vertMain", irr::video::EVST_VS_1_1,"RoomFragShader.txt", "fragMain", irr::video::EPST_PS_1_1,RoomCallback, irr::video::EMT_LIGHTMAP);

    NormalsShader = irr::video::EMT_SOLID; // Fallback material type
    NormalsShaderCallBack* NormalsCallback= new NormalsShaderCallBack;
    NormalsShader = irrGpu->addHighLevelShaderMaterialFromFiles("NewNormalVert.txt", "main", irr::video::EVST_VS_1_1,"NewNormalFrag.txt", "main", irr::video::EPST_PS_1_1,NormalsCallback, irr::video::EMT_SOLID);
    NormalsCallback->fvAmbient = irr::video::SColor(255,20,20,20);

    blurImage = irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(width,height),"",irr::video::ECF_A8R8G8B8);
    blurImage2 = irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(width,height),"",irr::video::ECF_A8R8G8B8);
    BlinkMeterIMG = irrDriver->getTexture("test/BlinkMeter.jpg");
    StaminaMeterIMG = irrDriver->getTexture("test/StaminaMeter.jpg");

    /*defItemParams.getNode = &irr::scene::ISceneManager::addMeshSceneNode;
    defItemParams.registerRBody = &irrDynamics::registerNewRBody;
    defItemParams.unregisterRBody = &irrDynamics::unregisterRBody;*/

    // Add camera
	/*irr::scene::ICameraSceneNode *Camera = irrSmgr->addCameraSceneNodeFPS(0, 50, 0.02);
	Camera->setPosition(irr::core::vector3df(0, 5, -5));
	Camera->setTarget(irr::core::vector3df(0, 0, 0));*/

	//Add test model

    dynamics = new irrDynamics();//irrDynamics::getInstance();
    dynamics->setGravity(-100*RoomScale);

	irr::scene::IMeshSceneNode* node = nullptr;
    dynRegister* itemDyn = new dynRegister(dynamics);

    node = irrSmgr->addMeshSceneNode(irrSmgr->getMesh("test/eyedrops.b3d"));
    node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);

	item::setDynamics(itemDyn);

    node->setScale(irr::core::vector3df(0.06*RoomScale,0.06*RoomScale,0.06*RoomScale));
    itemEyedrops::setMeshNode(node);

    node = irrSmgr->addMeshSceneNode(irrSmgr->getMesh("test/gasmask.b3d"));
    node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);

    node->setScale(irr::core::vector3df(0.6*RoomScale,0.6*RoomScale,0.6*RoomScale));
    itemGasMask::setMeshNode(node);

    for (irr::u32 ui=0;ui<10000;ui++) {
        item* it = itemEyedrops::createItemEyedrops();
        itemList.push_back(it);

        it = itemGasMask::createItemGasMask();
        itemList.push_back(it);
    }

    RMesh* rme = loadRMesh(std::string("test/testroom_opt.rmesh"),irrFileSystem,irrDriver);
	node = irrSmgr->addMeshSceneNode(rme->mesh);

    std::vector<irr::video::SLight> lightList;
    for (unsigned int i=0;i<rme->pointlights.size();i++) {
        irr::video::SLight newLight;
        newLight.Position = rme->pointlights[i].Position*0.1f*RoomScale;
        newLight.DiffuseColor = rme->pointlights[i].DiffuseColor;
        newLight.Radius = rme->pointlights[i].Radius*0.05f*RoomScale;
        lightList.push_back(newLight);

        irrSmgr->addCubeSceneNode(1.0f,nullptr,-1,rme->pointlights[i].Position*0.1f*RoomScale)->getMaterial(0).DiffuseColor = irr::video::SColor(255,255,0,0);
    }
    NormalsCallback->setLights(lightList);

	btTransform Transform;
	Transform.setIdentity();
	btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);
	btVector3 localInertia(0,0,0);
	btRigidBody* rbody = new btRigidBody(0.f, MotionState, rme->shape,localInertia);
	//rbody->setActivationState(DISABLE_DEACTIVATION);
	dynamics->registerNewRBody(node,rbody,0);
	//all rigid bodies must have a friction value for friction to work
	rbody->setFriction(1.f);
	rbody->setRollingFriction(1.f);

	//node->setScale(irr::core::vector3df(0.1f*RoomScale));

    for (irr::u32 ui=0; ui<node->getMaterialCount(); ++ui) {
        if (node->getMaterial(ui).MaterialType == irr::video::EMT_LIGHTMAP) { //lightmapped surfaces are suitable for the RoomShader
            node->getMaterial(ui).setTexture(2,irrDriver->getTexture("test/tilebump.jpg"));
            node->getMaterial(ui).MaterialType = (irr::video::E_MATERIAL_TYPE)RoomShader;
            node->getMaterial(ui).TextureLayer[0].AnisotropicFilter = 16;
            node->getMaterial(ui).TextureLayer[1].AnisotropicFilter = 16;
            node->getMaterial(ui).TextureLayer[2].AnisotropicFilter = 16;
        } else if (node->getMaterial(ui).MaterialType == irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL) {
            std::cout<<"ALPHA_CHANNEL\n";
        } else if (node->getMaterial(ui).MaterialType == irr::video::EMT_SOLID) {
            std::cout<<"SOLID\n";
        }
    }

    node->setPosition(irr::core::vector3df(0,0*RoomScale,0));

    dynamics->addTriMesh_static(node);

	for (int i = 0;i<1;i++) {
        irr::scene::IMesh* mesh1 = irrSmgr->getMesh("test/scp-066.b3d");

        node = irrSmgr->addMeshSceneNode(mesh1);


        node->setScale(irr::core::vector3df(2.1*RoomScale));
        node->setPosition(irr::core::vector3df(-0*RoomScale,10*RoomScale,0*RoomScale));

        rbody = dynamics->addTriMesh_moving(node,5.0f,5,1,1);
        rbody->setFriction(10.0f);
        rbody->setDamping(0,0);
        node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);

        node->getMaterial(0).setTexture(1, irrDriver->getTexture("test/scp-066_normal.png"));
        node->getMaterial(0).setTexture(2, irrDriver->getTexture("test/scp-066_specular.png"));
        node->getMaterial(1).MaterialType = (irr::video::E_MATERIAL_TYPE)NormalsShader;
        node->getMaterial(1).setTexture(1, irrDriver->getTexture("test/scp-066_normal.png"));
        node->getMaterial(1).setTexture(2, irrDriver->getTexture("test/scp-066_specular.png"));
        node->getMaterial(0).MaterialType = (irr::video::E_MATERIAL_TYPE)NormalsShader;

        node->getMaterial(0).SpecularColor.set(0,0,0,0);
        node->getMaterial(0).Shininess = 0.f;
        node->getMaterial(1).SpecularColor.set(0,0,0,0);
        node->getMaterial(1).Shininess = 0.0f;
	}

	mainPlayer = new player(this,irrSmgr,dynamics,irrReceiver);

    //test node
    irr::scene::IMesh* mesh1 = irrSmgr->getMesh("test/173_2.b3d");

    node = irrSmgr->addMeshSceneNode(mesh1);

    node->setScale(irr::core::vector3df(1.3*RoomScale));
    node->setPosition(irr::core::vector3df(0*RoomScale,10*RoomScale,-10*RoomScale));
    node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
    node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);

    node->getMaterial(0).Lighting = true;
    node->getMaterial(0).MaterialType = (irr::video::E_MATERIAL_TYPE)NormalsShader;

    rbody = dynamics->addTriMesh_moving(node,16000.f,20,1,1);
    rbody->setAngularFactor(btVector3(0,1.0,0));
    //rbody->setLinearFactor(btVector3(0.1,0.1,0.1));

    node->getMaterial(0).setTexture(1, irrDriver->getTexture("test/173_norm.jpg"));
    node->getMaterial(0).setTexture(2, irrDriver->getTexture("test/173_Spec.jpg"));

    node->getMaterial(0).EmissiveColor = irr::video::SColor(100,100,100,100);

    mainPlayer->testNode = node;
    //------------

	irrDevice->getCursorControl()->setVisible(false);
}

world::~world() {
    //fileSystem->drop();
    //env->drop();
    //smgr->drop();
    //driver->drop();
    irrDevice->drop();
}

bool world::run() {

    if (prevTime==0) { FPSfactor = 1.0; } else {
        FPSfactor = (irrTimer->getRealTime()-prevTime)/(1000.0/70.0);
    }
    prevTime = irrTimer->getRealTime();

    float prec = 0.75f;

    dynamics->simStep(irrTimer->getRealTime(),60.f * prec);

    mainPlayer->update();
    irrDriver->beginScene(true, true, irr::video::SColor(255, 255, 0, 255));
    irrDriver->setRenderTarget(blurImage2); //copy the old render
    irrDriver->draw2DImage(blurImage,irr::core::position2d<irr::s32>(0,0),irr::core::rect<irr::s32>(0,0,mainWidth,mainHeight), 0,irr::video::SColor(255,255,255,255), false);
    irrDriver->setRenderTarget(blurImage); //create a new render, using the old one to add a blur effect
    irrSmgr->drawAll();
    float BlinkTimer = mainPlayer->BlinkTimer;
    if (BlinkTimer<0) {
        double darkA;
        if (mainPlayer->BlinkTimer>-0.44625) {
            darkA = std::max(darkA,std::sin(-BlinkTimer*201.68));
        } else if (mainPlayer->BlinkTimer>-1.33875) {
            darkA = std::max(darkA,std::abs(std::sin(BlinkTimer*201.68)));
        }
        irrDriver->draw2DRectangle(irr::video::SColor(std::min(255.0,darkA*255.0),0,0,0),irr::core::rect<irr::s32>(0,0,mainWidth,mainHeight));
    }
    irrDriver->draw2DImage(blurImage2,irr::core::position2d<irr::s32>(0,0),irr::core::rect<irr::s32>(0,0,mainWidth,mainHeight), 0,irr::video::SColor(std::min(blurAlpha/FPSfactor,200.0f),255,255,255), false);
    irrDriver->setRenderTarget(0); //draw to screen
    irrDriver->clearZBuffer();
    irrDriver->draw2DImage(blurImage,irr::core::position2d<irr::s32>(0,0),irr::core::rect<irr::s32>(0,0,mainWidth,mainHeight), 0,irr::video::SColor(255,255,255,255), false);

    //Blink meter
    irrDriver->draw2DLine(irr::core::position2d<irr::s32>(80,mainHeight-95),irr::core::position2d<irr::s32>(284,mainHeight-95),irr::video::SColor(255,255,255,255));
    irrDriver->draw2DLine(irr::core::position2d<irr::s32>(80,mainHeight-75),irr::core::position2d<irr::s32>(284,mainHeight-75),irr::video::SColor(255,255,255,255));
    irrDriver->draw2DLine(irr::core::position2d<irr::s32>(80,mainHeight-95),irr::core::position2d<irr::s32>(80,mainHeight-75),irr::video::SColor(255,255,255,255));
    irrDriver->draw2DLine(irr::core::position2d<irr::s32>(284,mainHeight-95),irr::core::position2d<irr::s32>(284,mainHeight-75),irr::video::SColor(255,255,255,255));

    for (int i=0;i<mainPlayer->BlinkTimer/100*20;i++) {
        irrDriver->draw2DImage(BlinkMeterIMG,irr::core::position2d<irr::s32>(83+i*10,mainHeight-92),irr::core::rect<irr::s32>(0,0,8,14), 0,irr::video::SColor(255,255,255,255), false);
    }

    //Stamina meter
    irrDriver->draw2DLine(irr::core::position2d<irr::s32>(80,mainHeight-55),irr::core::position2d<irr::s32>(284,mainHeight-55),irr::video::SColor(255,255,255,255));
    irrDriver->draw2DLine(irr::core::position2d<irr::s32>(80,mainHeight-35),irr::core::position2d<irr::s32>(284,mainHeight-35),irr::video::SColor(255,255,255,255));
    irrDriver->draw2DLine(irr::core::position2d<irr::s32>(80,mainHeight-55),irr::core::position2d<irr::s32>(80,mainHeight-35),irr::video::SColor(255,255,255,255));
    irrDriver->draw2DLine(irr::core::position2d<irr::s32>(284,mainHeight-55),irr::core::position2d<irr::s32>(284,mainHeight-35),irr::video::SColor(255,255,255,255));

    for (int i=0;i<mainPlayer->Stamina/100*20;i++) {
        irrDriver->draw2DImage(StaminaMeterIMG,irr::core::position2d<irr::s32>(83+i*10,mainHeight-52),irr::core::rect<irr::s32>(0,0,8,14), 0,irr::video::SColor(255,255,255,255), false);
    }

    if (irrReceiver->IsMouseDown(0)) {
        for (unsigned int i=0;i<itemList.size();i++) {
            if (itemList[i]->getPicked()) {
                itemList[i]->Unpick(mainPlayer->getPosition());
                break;
            }
        }
    }

    if (irrReceiver->IsMouseDown(1)) {
        for (unsigned int i=0;i<itemList.size();i++) {
            if (!itemList[i]->getPicked()) {
                itemList[i]->Pick();
                break;
            }
        }
    }

    blurAlpha = 200;

    irrDriver->endScene();

    irr::core::position2di mousePos = irrReceiver->getMousePos();
    if (mousePos != irr::core::position2di(mainWidth/2,mainHeight/2)) {
        mainPlayer->yaw += ((int)mousePos.X-(int)(mainWidth/2))*0.1;
        mainPlayer->pitch += ((int)mousePos.Y-(int)(mainHeight/2))*0.1;
        irrDevice->getCursorControl()->setPosition((irr::s32)mainWidth/2,(irr::s32)mainHeight/2);
    }

    return irrDevice->run();
}

float world::getFPSfactor() {
    return FPSfactor;
}

player::player(world* own,irr::scene::ISceneManager* smgr,irrDynamics* dyn,MainEventReceiver* receiver,float iheight,float iradius,float mass) {
    owner = own;
    irrSmgr = smgr;
    dynamics = dyn;
    irrReceiver = receiver;

	height = iheight*RoomScale;
	radius = iradius*RoomScale;

    selfRotation.X = selfRotation.Y = selfRotation.Z = 0;

    // Add camera
    Camera = irrSmgr->addCameraSceneNode(0,irr::core::vector3df(0,0,0),irr::core::vector3df(0,0,-1));
    Camera->setPosition(irr::core::vector3df(-0*RoomScale, 10*RoomScale, 0*RoomScale));
    Camera->setTarget(irr::core::vector3df(0, 0, 0));
    Camera->setNearValue(5.0*RoomScale);
    Camera->setFarValue(256.0*RoomScale);

    irrSmgr->setActiveCamera(Camera);

    //Add capsule
    Capsule = dynamics->addPlayerColliderObject(Camera,height,radius,mass,1,1);
    Capsule->setAngularFactor(btVector3(0,0,0)); //don't let the capsule rotate until the player dies
    Capsule->setSleepingThresholds (0.0, 0.0);
    Capsule->setGravity(btVector3(0.f,-300.0f*RoomScale,0.f));
	Capsule->setActivationState(DISABLE_DEACTIVATION);

    //clear inventory
    for (irr::u32 i=0;i<inventory_size;i++) {
        inventory[i]=nullptr;
    }
}

void player::update() {

    Camera->updateAbsolutePosition();

    if (irrReceiver->IsKeyDown(irr::KEY_SPACE)) {
        if (BlinkTimer>0) { BlinkTimer = 0.0; }
        if (BlinkTimer<-0.8925) { BlinkTimer = -0.8925; }
    }

    irr::core::vector3df dir(0,1,0);
    irr::core::matrix4 rotMatrix;

    BlinkTimer-=owner->getFPSfactor()*0.09;

    if (BlinkTimer<=-1.785) {
        BlinkTimer = 100;
    }

    while (yaw>180.0) { yaw-=360.0; }
    while (yaw<-180.0) { yaw+=360.0; }

    while (pitch>180.0) { pitch-=360.0; }
    while (pitch<-180.0) { pitch+=360.0; }

    if (pitch>70.0) pitch = 70.0;
    if (pitch<-70.0) pitch = -70.0;

    selfRotation.X = pitch;
    selfRotation.Y = yaw;

    rotMatrix.setRotationDegrees(irr::core::vector3df(0,0,5));
    rotMatrix.transformVect(dir);

    rotMatrix.setRotationDegrees(selfRotation);
    rotMatrix.transformVect(dir);

    rotMatrix.setRotationDegrees(Camera->getRotation());
    rotMatrix.transformVect(dir);
    Camera->setUpVector(dir);
    dir.X = 0;
    dir.Y = 0;
    dir.Z = 1;

    rotMatrix.setRotationDegrees(selfRotation);
    rotMatrix.transformVect(dir);

    rotMatrix.setRotationDegrees(Camera->getRotation());
    rotMatrix.transformVect(dir);
    Camera->updateAbsolutePosition();
    Camera->setTarget(Camera->getAbsolutePosition()+dir);

    btVector3 speed = Capsule->getLinearVelocity();

    /*float addVSpeed = 0.0f;//(speed.y()>=0.0)*0.75;
    if (irrReceiver->IsKeyDown(irr::KEY_KEY_W)) {
        btVector3 newSpeed(sin(irr::core::degToRad(yaw))*walkSpeed*RoomScale,speed.y()-addVSpeed,cos(irr::core::degToRad(yaw))*walkSpeed*RoomScale);
        Capsule->setLinearVelocity(newSpeed);
    } else if (irrReceiver->IsKeyDown(irr::KEY_KEY_S)) {
        btVector3 newSpeed(sin(irr::core::degToRad(yaw))*-walkSpeed*RoomScale,speed.y()-addVSpeed,cos(irr::core::degToRad(yaw))*-walkSpeed*RoomScale);
        Capsule->setLinearVelocity(newSpeed);
    } else if (irrReceiver->IsKeyDown(irr::KEY_KEY_A)) {
        btVector3 newSpeed(sin(irr::core::degToRad(yaw-90.0))*walkSpeed*RoomScale,speed.y()-addVSpeed,cos(irr::core::degToRad(yaw-90.0))*walkSpeed*RoomScale);
        Capsule->setLinearVelocity(newSpeed);
    } else if (irrReceiver->IsKeyDown(irr::KEY_KEY_D)) {
        btVector3 newSpeed(sin(irr::core::degToRad(yaw+90.0))*walkSpeed*RoomScale,speed.y()-addVSpeed,cos(irr::core::degToRad(yaw+90.0))*walkSpeed*RoomScale);
        Capsule->setLinearVelocity(newSpeed);
    } else {
        Capsule->setLinearVelocity(btVector3(0,speed.y()-addVSpeed,0));
    }*/
    if (!dead) {
		if ((irrReceiver->IsKeyDown(irr::KEY_KEY_W)
			|| irrReceiver->IsKeyDown(irr::KEY_KEY_S)
			|| irrReceiver->IsKeyDown(irr::KEY_KEY_A)
			|| irrReceiver->IsKeyDown(irr::KEY_KEY_D)
			) && std::abs(speed[1]) < 20.f) {

			float walkSpeed = 40.0;
			if (crouchState<0.015f) {
				if (irrReceiver->IsKeyDown(irr::KEY_LSHIFT)) {
					if (Stamina>0) walkSpeed=80.0;
					Stamina-=0.2*owner->getFPSfactor();
					if (Stamina<0) Stamina = -10.0;
				} else {
					Stamina=std::min(Stamina+0.15*owner->getFPSfactor(),100.0);
				}
			} else {
				if (crouchState>0.5f) walkSpeed = 10.f; else walkSpeed = 20.f;
				Stamina=std::min(Stamina+0.15*owner->getFPSfactor(),100.0);
			}
			float dir = 0;
			if (irrReceiver->IsKeyDown(irr::KEY_KEY_W)) {
				dir = 0;
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
			dir-=90;
			dir *= irr::core::DEGTORAD;
			Capsule->setFriction(0.15f);
			float xs,zs,nxs,nzs,d,dd;
			xs = speed[0];
			zs = speed[2];
			if (irr::core::squareroot(xs*xs+zs*zs)<walkSpeed*0.75f*RoomScale) {
				Capsule->applyCentralImpulse(btVector3(std::cos(dir+yaw*irr::core::DEGTORAD)*walkSpeed*2.f*RoomScale,0.f,-std::sin(dir+yaw*irr::core::DEGTORAD)*walkSpeed*2.f*RoomScale));
			} else {
				nxs = std::cos(dir+yaw*irr::core::DEGTORAD);
				nzs = -std::sin(dir+yaw*irr::core::DEGTORAD);
				d = irr::core::squareroot(xs*xs+zs*zs);
				nxs = std::atan2(nxs,nzs);
				nzs = std::atan2(xs,zs);
				dd = nxs - nzs;
				while (dd<-irr::core::PI/2.f) dd+=irr::core::PI;
				while (dd>irr::core::PI/2.f) dd-=irr::core::PI;
				nzs = nzs+dd * 0.5f;
				nzs -= irr::core::PI/2.f;

				Capsule->setLinearVelocity(btVector3(std::cos(nzs)*d,speed[1],-std::sin(nzs)*d));
			}
		} else {
			if (std::abs(speed[1])<=4.f) Capsule->setFriction(3.0f); else Capsule->setFriction(1.0f);
			Stamina=std::min(Stamina+0.15*owner->getFPSfactor(),100.0);
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
	} else {
		Capsule->setFriction(3.0f);
		Capsule->setRollingFriction(25.0f);
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

		crouchState=std::min(0.5f,crouchState+(0.5f-crouchState)*owner->getFPSfactor()*0.2f);
		if (crouchState>0.4998f) crouchState = 0.5f;

		if (changed) { //must unregister body to perform changes to its shape
			dynamics->unregisterRBody(Capsule);
			Capsule->getCollisionShape()->setLocalScaling(btVector3(1.f,1.f-crouchState,1.f));
			dynamics->registerNewRBody(Camera,Capsule,-1,1,1,irr::core::vector3df(0,(-(height/2.f)*(1.f-crouchState))+(radius/1.5f),0));
			Capsule->setGravity(btVector3(0.f,-300.0f*RoomScale,0.f));
		}
	} else {
		if (crouchState>0.0f) changed = true;

		crouchState=std::max(0.f,crouchState+(-crouchState)*owner->getFPSfactor()*0.1f);
		if (crouchState<0.002f) crouchState = 0.f;

		if (changed) { //must unregister body to perform changes to its shape
			dynamics->unregisterRBody(Capsule);
			Capsule->getCollisionShape()->setLocalScaling(btVector3(1.f,1.f-crouchState,1.f));
			dynamics->registerNewRBody(Camera,Capsule,-1,1,1,irr::core::vector3df(0,(-(height/2.f)*(1.f-crouchState))+(radius/1.5f),0));
			Capsule->setGravity(btVector3(0.f,-300.0f*RoomScale,0.f));
		}
	}
	//std::cout<<Capsule->getCollisionShape()->getLocalScaling()[1]<<" "<<crouchState<<"\n";

    //std::cout<<"seesNode: "<<seesMeshNode(testNode)<<"\n";
}

void player::addToInventory(item* it) {
    for (irr::u32 i=0;i<inventory_size;i++) {
        if (inventory[i]==nullptr) {
            inventory[i]=it;
            it->Pick();
            break;
        }
    }
}
void player::takeFromInventory(unsigned char slot) {

}

bool player::seesMeshNode(irr::scene::IMeshSceneNode* node) {
    irr::scene::SViewFrustum frust = *Camera->getViewFrustum();

    //transform the frustum to the node's current absolute transformation
    irr::core::matrix4 invTrans(node->getAbsoluteTransformation(), irr::core::matrix4::EM4CONST_INVERSE);
    //invTrans.makeInverse();
    frust.transform(invTrans);

    irr::core::vector3df edges[8];
    node->getBoundingBox().getEdges(edges);

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

bool getNodeTriangleTextureName //taken from here: http://irrlicht.sourceforge.net/forum/viewtopic.php?f=9&t=45212
(
		irr::scene::ISceneNode* node,
		const irr::core::triangle3df& tri,
		std::string& texname
) //this function can be slow, use it carefully
{
		irr::scene::IMesh* mesh = 0;

		irr::scene::ESCENE_NODE_TYPE type = node->getType();
		if ((type == irr::scene::ESNT_MESH) || (type == irr::scene::ESNT_OCTREE))
		{
				mesh = static_cast<irr::scene::IMeshSceneNode*>(node)->getMesh();
		}
		else if (type == irr::scene::ESNT_ANIMATED_MESH)
		{
				mesh = static_cast<irr::scene::IAnimatedMeshSceneNode*>(node)->getMesh()->getMesh(0);
		}
		else
		{
				return false;
		}

		if (!mesh)
				return false;


		irr::core::vector3df ptA = tri.pointA;
		irr::core::vector3df ptB = tri.pointB;
		irr::core::vector3df ptC = tri.pointC;

		irr::core::matrix4 matrix = node->getAbsoluteTransformation();
		irr::core::matrix4 inverse;
		irr::core::vector3df p0, p1, p2;

		if (matrix.getInverse(inverse))
		{
				inverse.transformVect(p0, ptA);
				inverse.transformVect(p1, ptB);
				inverse.transformVect(p2, ptC);
		}
		else { p0 = ptA; p1 = ptB; p2 = ptC; }

		for (irr::u32 i=0; i<mesh->getMeshBufferCount(); ++i)
		{
				bool p0Found = false;
				bool p1Found = false;
				bool p2Found = false;

				//printf("mesh->getMeshBuffer(%i): vertexCount=%i\n", i, mesh->getMeshBuffer(i)->getVertexCount());

				irr::scene::IMeshBuffer* buf = mesh->getMeshBuffer(i);
				for (irr::u32 j=0; j<buf->getVertexCount(); ++j)
				{
						irr::core::vector3df pos = buf->getPosition(j);

						//if (pos.equals(p0) || pos.equals(p1) || pos.equals(p2))
								//printf("vertice found!\n");

						if ((!p0Found) && (pos.equals(p0)))
						//if (pos.equals(p0))
						{
								p0Found = true;
						}

						if ((!p1Found) && (pos.equals(p1)))
						//if (pos.equals(p1))
						{
								p1Found = true;
						}

						if ((!p2Found) && (pos.equals(p2)))
						//if (pos.equals(p2))
						{
								p2Found = true;
						}
				}

				if (p0Found && p1Found && p2Found)
				{
						irr::video::ITexture* tex = buf->getMaterial().getTexture(0);
						if (!tex)
								return false;

						texname = std::string(tex->getName().getPath().c_str());
						return true;
				}
		}

		return false;
}
