#include "3dworld.h"
#include "rooms/rmesh.h"

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

    node->setScale(irr::core::vector3df(0.06*RoomScale,0.06*RoomScale,0.06*RoomScale));
    itemEyedrops::setMeshNode(node); itemEyedrops::setDynamics(itemDyn);

    node = irrSmgr->addMeshSceneNode(irrSmgr->getMesh("test/gasmask.b3d"));
    node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);

    node->setScale(irr::core::vector3df(0.6*RoomScale,0.6*RoomScale,0.6*RoomScale));
    itemGasMask::setMeshNode(node); itemGasMask::setDynamics(itemDyn);

    for (irr::u32 ui=0;ui<10000;ui++) {
        item* it = itemEyedrops::createItemEyedrops();
        itemList.push_back(it);

        it = itemGasMask::createItemGasMask();
        itemList.push_back(it);
    }

    RMesh* rme = loadRMesh(std::string("test/173bright_opt.rmesh"),irrFileSystem,irrDriver);
	node = irrSmgr->addMeshSceneNode(rme->mesh);//irrSmgr->getMesh("test/room939.b3d"));

    std::vector<irr::video::SLight> lightList;
    for (unsigned int i=0;i<rme->pointlights.size();i++) {
        /*irr::scene::ILightSceneNode* light1 =
            irrSmgr->addLightSceneNode(0, irr::core::vector3df(0,0,0),
            rme->pointlights[i].color,rme->pointlights[i].radius*0.05f*RoomScale);

        light1->setPosition(rme->pointlights[i].position*0.1f*RoomScale);
        std::cout<<"Light "<<i<<": "<<rme->pointlights[i].position.X<<" "<<rme->pointlights[i].position.Y<<" "<<rme->pointlights[i].position.Z<<"\n";

        */
        irr::video::SLight newLight;
        newLight.Position = rme->pointlights[i].position*0.1f*RoomScale;
        newLight.DiffuseColor = rme->pointlights[i].color;
        newLight.Radius = rme->pointlights[i].radius*0.05f*RoomScale;
        lightList.push_back(newLight);

        irrSmgr->addCubeSceneNode(1.0f,nullptr,-1,rme->pointlights[i].position*0.1f*RoomScale)->getMaterial(0).DiffuseColor = irr::video::SColor(255,255,0,0);
    }
    NormalsCallback->setLights(lightList);

    //PerNodeLightManager* lightMgr = new PerNodeLightManager(irrSmgr);
    //irrSmgr->setLightManager(lightMgr);

	node->setScale(irr::core::vector3df(0.1f*RoomScale));

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

    //itemTemplates.push_back(new itemtemplate(this,"ReVision Eyedrops",ITEM_EYEDROPS,"test/eyedrops.b3d"));

    /*for (int i=0;i<3;i++) {
        irr::video::SColorf lightColor=irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f);
        switch (i) {
            case 0:
                lightColor=irr::video::SColorf(1.0f, 0.0f, 0.0f, 1.0f);
            break;
            case 1:
                lightColor=irr::video::SColorf(0.0f, 0.0f, 1.0f, 1.0f);
            break;
            case 2:
                lightColor=irr::video::SColorf(0.0f, 1.0f, 0.0f, 1.0f);
            break;
            case 3:
                lightColor=irr::video::SColorf(0.0f, 0.8f, 0.0f, 1.0f);
            break;
        }
        irr::scene::ILightSceneNode* light1 =
            irrSmgr->addLightSceneNode(0, irr::core::vector3df(0,0,0),
            lightColor,40.0f*RoomScale);

        light1->setPosition(irr::core::vector3df(-0*RoomScale+cos(i*90.0)*50.0,0*RoomScale,10*RoomScale+sin(i*90.0)*50.0-20.0*RoomScale));

        //dynamics->addSphericalObject(light1,10.f,10.f);
    }*/

    //createItemTemplate("Eyedrops",itemTempIDs::ITEM_EYEDROPS,"test/eyedrops.b3d","",0.06*RoomScale);

	for (int i = 0;i<1;i++) {
        irr::scene::IMesh* mesh1 = irrSmgr->getMesh("test/173_2.b3d");
        //irr::scene::IMesh* mesh2 = irrSmgr->getMeshManipulator()->createMeshWithTangents(mesh1);
        node = irrSmgr->addMeshSceneNode(mesh1);

        node->setScale(irr::core::vector3df(1.3*RoomScale));
        node->setPosition(irr::core::vector3df(0*RoomScale,10*RoomScale,-10*RoomScale));
        node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
        node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
        /*node->getMaterial(0).AmbientColor = irr::video::SColor(0,0,0,0);
        node->getMaterial(0).DiffuseColor = irr::video::SColor(0,0,0,0);*/
        node->getMaterial(0).Lighting = true;
        node->getMaterial(0).MaterialType = (irr::video::E_MATERIAL_TYPE)NormalsShader;
        /*node->getMaterial(1).Lighting = false;
        node->getMaterial(2).Lighting = false;
        node->getMaterial(3).Lighting = false;
        node->getMaterial(4).Lighting = false;
        node->getMaterial(5).Lighting = false;*/
        btRigidBody* rbody = dynamics->addTriMesh_moving(node,16.f/*000*/,20,1,1);
        rbody->setAngularFactor(btVector3(0,0.1,0));
        rbody->setLinearFactor(btVector3(0.1,0.1,0.1));

        //node->getMaterial(0).DiffuseColor = irr::video::SColor(0, 0, 0, 0);

        node->getMaterial(0).setTexture(1, irrDriver->getTexture("test/173_norm.jpg"));
        node->getMaterial(0).setTexture(2, irrDriver->getTexture("test/173_Spec.jpg"));
        //node->getMaterial(0).MaterialType = irr::video::EMT_NORMAL_MAP_SOLID;

        /*node->getMaterial(0).SpecularColor.set(100,100,100,100);
        node->getMaterial(0).Shininess = 0.3f;

        node->getMaterial(0).AmbientColor = irr::video::SColor(255, 0, 0, 100);*/

        /*node->getMaterial(1).setTexture(1, irrDriver->getTexture("test/173_norm.jpg"));
        node->getMaterial(1).MaterialType = (irr::video::E_MATERIAL_TYPE)NormalsShader;//irr::video::EMT_NORMAL_MAP_SOLID;*/

        node->getMaterial(0).EmissiveColor = irr::video::SColor(100,100,100,100);

        //node->setDebugDataVisible(irr::scene::EDS_NORMALS);
	}

	for (int i = 0;i<10;i++) {
        irr::scene::IMesh* mesh1 = irrSmgr->getMesh("test/scp-066.b3d");
        irr::scene::IMesh* mesh2 = irrSmgr->getMeshManipulator()->createMeshWithTangents(mesh1);
        node = irrSmgr->addMeshSceneNode(mesh2);


        node->setScale(irr::core::vector3df(2.1*RoomScale));
        node->setPosition(irr::core::vector3df(-0*RoomScale,10*RoomScale,0*RoomScale));
        /*node->getMaterial(0).Lighting = false;
        node->getMaterial(1).Lighting = false;*/
        dynamics->addTriMesh_moving(node,5.0f,5,1,1);
        node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);

        node->getMaterial(0).setTexture(1, irrDriver->getTexture("test/scp-066_normal.png"));
        node->getMaterial(0).setTexture(2, irrDriver->getTexture("test/scp-066_specular.png"));
        node->getMaterial(1).MaterialType = (irr::video::E_MATERIAL_TYPE)NormalsShader;//irr::video::EMT_NORMAL_MAP_SOLID;
        node->getMaterial(1).setTexture(1, irrDriver->getTexture("test/scp-066_normal.png"));
        node->getMaterial(1).setTexture(2, irrDriver->getTexture("test/scp-066_specular.png"));
        node->getMaterial(0).MaterialType = (irr::video::E_MATERIAL_TYPE)NormalsShader;

        //irr::video::EMT_TRANSPARENT_ADD_COLOR
        //node->setMaterialType(irr::video::EMT_NORMAL_MAP_SOLID);

        //node->setMaterialTexture(2, irrDriver->getTexture("test/scp-066_normal.jpg"));
        /*node->getMaterial(0).setTexture(1, irrDriver->getTexture("test/scp-066_normal.jpg"));
        node->getMaterial(0).MaterialType = irr::video::EMT_NORMAL_MAP_SOLID;*/

        node->getMaterial(0).SpecularColor.set(0,0,0,0);
        node->getMaterial(0).Shininess = 0.f;
        node->getMaterial(1).SpecularColor.set(0,0,0,0);
        node->getMaterial(1).Shininess = 0.0f;
	}

	mainPlayer = new player(this,irrSmgr,dynamics,irrReceiver);



	for (int i = 0;i<10000;i++) {
        //item* it = createItem(itemTempIDs::ITEM_EYEDROPS);
        //disableItem(it);//it->Pick();

        //it->Unpick(irr::core::vector3df(20*RoomScale,10*RoomScale,0));
        /*node = irrSmgr->addMeshSceneNode(irrSmgr->getMesh("test/eyedrops.b3d"));

        node->setScale(irr::core::vector3df(0.06*RoomScale));
        node->setPosition(irr::core::vector3df(-0*RoomScale,10*RoomScale,0*RoomScale));
        /\*node->getMaterial(0).Lighting = false;
        node->getMaterial(1).Lighting = false;*\/
        dynamics->addTriMesh_moving(node,10.0f,10,2,2);

        node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);*/

	}

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

    float prec = 2.0f;

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

    blurAlpha = 0;

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

player::player(world* own,irr::scene::ISceneManager* smgr,irrDynamics* dyn,MainEventReceiver* receiver,float height,float radius,float mass) {
    owner = own;
    irrSmgr = smgr;
    dynamics = dyn;
    irrReceiver = receiver;

    selfRotation.X = selfRotation.Y = selfRotation.Z = 0;

    // Add camera
    Camera = irrSmgr->addCameraSceneNode(0,irr::core::vector3df(0,0,0),irr::core::vector3df(0,0,-1));
    Camera->setPosition(irr::core::vector3df(-0*RoomScale, 10*RoomScale, 0*RoomScale));
    Camera->setTarget(irr::core::vector3df(0, 0, 0));
    Camera->setNearValue(5.0*RoomScale);
    Camera->setFarValue(256.0*RoomScale);

    irrSmgr->setActiveCamera(Camera);

    //Add capsule
    Capsule = dynamics->addPlayerColliderObject(Camera,height*RoomScale,radius*RoomScale,mass,1,1);
    Capsule->setAngularFactor(btVector3(0,0,0)); //don't let the capsule rotate until the player dies
    Capsule->setSleepingThresholds (0.0, 0.0);

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

    float walkSpeed = 40.0;
    if (irrReceiver->IsKeyDown(irr::KEY_LSHIFT)) {
        if (Stamina>0) walkSpeed=80.0;
        Stamina-=0.2*owner->getFPSfactor();
        if (Stamina<0) Stamina = -10.0;
    } else {
        Stamina=std::min(Stamina+0.15*owner->getFPSfactor(),100.0);
    }

    float addVSpeed = (speed.y()>=0.0)*0.75;
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
    }
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
    float xzdist;
    float deltapitch;//,deltayaw;

    float tx,ty,tz;
    Camera->updateAbsolutePosition();
    node->updateAbsolutePosition();
    tx = Camera->getAbsolutePosition().X-node->getAbsolutePosition().X;
    ty = Camera->getAbsolutePosition().Y-node->getAbsolutePosition().Y;
    tz = Camera->getAbsolutePosition().Z-node->getAbsolutePosition().Z;
    xzdist = std::sqrt((tx*tx)+(tz*tz));

    deltapitch = std::atan2(xzdist,ty);
    deltapitch*=irr::core::RADTODEG;

    return false;
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
