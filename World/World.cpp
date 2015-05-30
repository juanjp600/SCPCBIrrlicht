#include "World.h"
#include "Player.h"

#include "NPCs/NPC096.h"
#include "NPCs/NPC173.h"

#include "Rooms/Room.h"
#include "Rooms/RMesh.h"

#include "Rooms/Door.h"

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <sstream>

void World::renderLights() {
    //smCamera->setVisible(true);
    //mainPlayer->setVisible(false);
    irr::scene::ICameraSceneNode* oldCam = irrSmgr->getActiveCamera();
    oldCam->render();

    irrDriver->setRenderTarget(lightPass[0],true,true,irr::video::SColor(255,0,0,0));

    plainLightCallback->pass = 0;

    irrSmgr->fastDrawAll_init(oldCam->getPosition());

    irr::core::matrix4 projectionMatrix;
    projectionMatrix.buildProjectionMatrixPerspectiveFovLH(90.f*irr::core::DEGTORAD,1.f,1.f,150.f);
    for (int i=0;i<plainLightCallback->lightList.size();i++) {
        irr::core::vector3df lightPos = irr::core::vector3df(plainLightCallback->lightList[i].pos[0],plainLightCallback->lightList[i].pos[1],plainLightCallback->lightList[i].pos[2]);
        //smCamera->setPosition(irr::core::vector3df(PlainLightCallback->lightList[i].pos[0],PlainLightCallback->lightList[i].pos[1],PlainLightCallback->lightList[i].pos[2]));
        if (lightPos.getDistanceFromSQ(mainPlayer->getPosition())<300.f*300.f&&mainPlayer->seesBoundingBox(irr::core::aabbox3df(lightPos-irr::core::vector3df(130.f,130.f,130.0),lightPos+irr::core::vector3df(130.f,130.f,130.0)),*mainPlayer->getViewFrustum())) {
            for (int j=0;j<6;j++) {
                irrDriver->setRenderTarget(lightDepth,true,true,irr::video::SColor(255,255,0,0));
                //irrSmgr->setActiveCamera(smCamera);
                irrDriver->getOverrideMaterial().EnableFlags = irr::video::EMF_MATERIAL_TYPE;
                irrDriver->getOverrideMaterial().EnablePasses = irr::scene::ESNRP_SOLID;
                irrDriver->getOverrideMaterial().Material.MaterialType = (irr::video::E_MATERIAL_TYPE)zBufferShader;
                //irrDriver->setViewPort(irr::core::recti(0,0,128,128));
                /*switch (j) {
                    case 0:
                        //smCamera->setRotation(irr::core::vector3df(0.f,0.f,0.f));
                        smCamera->setUpVector(irr::core::vector3df(0.f,1.f,0.f));
                        smCamera->setTarget(smCamera->getPosition()+irr::core::vector3df(0.f,0.f,1.f));
                    break;
                    case 1:
                        smCamera->setUpVector(irr::core::vector3df(0.f,1.f,0.f));
                        smCamera->setTarget(smCamera->getPosition()+irr::core::vector3df(1.f,0.f,0.f));
                        //smCamera->setRotation(irr::core::vector3df(0.f,90.f,0.f));
                    break;
                    case 2:
                        smCamera->setUpVector(irr::core::vector3df(0.f,1.f,0.f));
                        smCamera->setTarget(smCamera->getPosition()+irr::core::vector3df(0.f,0.f,-1.f));
                        //smCamera->setRotation(irr::core::vector3df(0.f,180.f,0.f));
                    break;
                    case 3:
                        smCamera->setUpVector(irr::core::vector3df(0.f,1.f,0.f));
                        smCamera->setTarget(smCamera->getPosition()+irr::core::vector3df(-1.f,0.f,0.f));
                        //smCamera->setRotation(irr::core::vector3df(0.f,270.f,0.f));
                    break;
                    case 4:
                        smCamera->setUpVector(irr::core::vector3df(0.f,0.f,1.f));
                        smCamera->setTarget(smCamera->getPosition()+irr::core::vector3df(0.f,1.f,0.f));
                        //smCamera->setRotation(irr::core::vector3df(90.f,0.f,0.f));
                    break;
                    case 5:
                        smCamera->setUpVector(irr::core::vector3df(0.f,0.f,1.f));
                        smCamera->setTarget(smCamera->getPosition()+irr::core::vector3df(0.f,-1.f,0.f));
                        //smCamera->setRotation(irr::core::vector3df(-90.f,0.f,0.f));
                    break;
                }*/
                //smCamera->OnRegisterSceneNode();
                irrSmgr->fastDrawAll(projectionMatrix,plainLightCallback->lightList[i].viewMatrix[j]);

                plainLightCallback->lightMatrix = projectionMatrix;//smCamera->getProjectionMatrix();
                plainLightCallback->lightMatrix *= plainLightCallback->lightList[i].viewMatrix[j];//smCamera->getViewMatrix();

                irrDriver->setRenderTarget(lightPass[1],true,true,irr::video::SColor(255,0,0,0));

                //irrDriver->setViewPort(irr::core::recti(0,0,mainWidth,mainHeight));
                irrDriver->getOverrideMaterial().EnablePasses = 0;
                //irrSmgr->setActiveCamera(oldCam);
                irrSmgr->fastDrawAll(oldCam->getProjectionMatrix(),oldCam->getViewMatrix());

                irrDriver->setRenderTarget(lightPass[0],false,false,irr::video::SColor(255,0,0,0));
                irrDriver->draw2DImage(lightPass[1],irr::core::vector2di(0,0));
            }
        }
        plainLightCallback->pass++;
    }
    //mainPlayer->setVisible(true);
    //smCamera->setVisible(false);
    irrDriver->getOverrideMaterial().EnablePasses = 0;

    irrDriver->setRenderTarget(0);
    //irrSmgr->getActiveCamera()->render();
    irrDriver->draw2DImage(lightPass[0],irr::core::vector2di(0,0));

    irrSmgr->fastDrawAll_end();
}

World::World(unsigned int width,unsigned int height,bool fullscreen) {
    mainWidth = width; mainHeight = height;

	scale2D = height/1024.f;
	scale2Db = height/720.f;

    irrDriverType = irr::video::EDT_OPENGL;
    irrReceiver = new MainEventReceiver;
    irrDevice = irr::createDevice(irrDriverType,irr::core::dimension2d<irr::u32>(width,height),32,fullscreen,false,true,irrReceiver);
    irrDriver = irrDevice->getVideoDriver();
	irrSmgr = irrDevice->getSceneManager();
	irrColl = irrSmgr->getSceneCollisionManager();

	Room::setSmgr(irrSmgr);

	Sound::initSounds();

	menusOpen = menus::NONE;

    irrDriver->setTextureCreationFlag(irr::video::ETCF_ALWAYS_32_BIT, true);

    irrEnv = irrDevice->getGUIEnvironment();

    irrFileSystem = irrDevice->getFileSystem();

    irrTimer = irrDevice->getTimer();
    time = irrTimer->getRealTime();
    irrReceiver->setTime(time);

	font1 = irr::gui::CGUITTFont::createTTFont(irrEnv, "GFX/cour.ttf", 16*scale2D, true, true);
	font2 = irr::gui::CGUITTFont::createTTFont(irrEnv, "GFX/cour.ttf", 64*scale2D, true, true);

	smCamera = irrSmgr->addCameraSceneNode(0,irr::core::vector3df(0,0,0),irr::core::vector3df(0,0,-1));
	smCamera->setFOV(90.0*irr::core::DEGTORAD64);
	smCamera->setAspectRatio(1.f);
	smCamera->setNearValue(1.f);
    smCamera->setFarValue(200.0*RoomScale);
	//smCamera->setVisible(false);

	int seed = time;
	std::cout<<"Seed: "<<seed<<"\n";

    srand(seed);

    irr::video::IGPUProgrammingServices* irrGpu = irrDriver->getGPUProgrammingServices();
    roomShader = irr::video::EMT_LIGHTMAP; // Fallback material type
    roomCallback = new RoomShaderCallBack;
    roomShader = (irr::video::E_MATERIAL_TYPE)irrGpu->addHighLevelShaderMaterialFromFiles("GFX/shaders/RoomVertShader.vert", "main", irr::video::EVST_VS_1_1,"GFX/shaders/RoomFragShader.frag", "main", irr::video::EPST_PS_1_1,roomCallback, irr::video::EMT_LIGHTMAP);

    roomShader_noNormals = irr::video::EMT_LIGHTMAP;
    roomCallback_noNormals = new RoomShaderCallBack_noNormals;
    roomShader_noNormals = (irr::video::E_MATERIAL_TYPE)irrGpu->addHighLevelShaderMaterialFromFiles("GFX/shaders/RoomVertShader.vert", "main", irr::video::EVST_VS_1_1,"GFX/shaders/RoomFragShader_noNormal.frag", "main", irr::video::EPST_PS_1_1,roomCallback_noNormals, irr::video::EMT_LIGHTMAP);

    vertLightShader = irr::video::EMT_SOLID;
    vertLightCallback = new VertLightShaderCallBack;
    vertLightShader = (irr::video::E_MATERIAL_TYPE)irrGpu->addHighLevelShaderMaterialFromFiles("GFX/shaders/VertLightingVert.vert", "main", irr::video::EVST_VS_1_1,"GFX/shaders/VertLightingFrag.frag", "main", irr::video::EPST_PS_1_1,vertLightCallback, irr::video::EMT_SOLID);

    vertLightShader_alpha = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;
    vertLightShader_alpha = (irr::video::E_MATERIAL_TYPE)irrGpu->addHighLevelShaderMaterialFromFiles("GFX/shaders/VertLightingVert.vert", "main", irr::video::EVST_VS_1_1,"GFX/shaders/VertLightingFrag.frag", "main", irr::video::EPST_PS_1_1,vertLightCallback,irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);

    fogBillboardShader = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;
    fogBillboardShader = (irr::video::E_MATERIAL_TYPE)irrGpu->addHighLevelShaderMaterialFromFiles("GFX/shaders/FogBillboardVert.vert", "main", irr::video::EVST_VS_1_1,"GFX/shaders/FogBillboardFrag.frag", "main", irr::video::EPST_PS_1_1,vertLightCallback,irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);

    normalsShader = irr::video::EMT_SOLID; // Fallback material type
    normalsCallback= new NormalsShaderCallBack;
    normalsShader = (irr::video::E_MATERIAL_TYPE)irrGpu->addHighLevelShaderMaterialFromFiles("GFX/shaders/NewNormalVert.vert", "main", irr::video::EVST_VS_1_1,"GFX/shaders/NewNormalFrag.frag", "main", irr::video::EPST_PS_1_1,normalsCallback, irr::video::EMT_SOLID);
    normalsCallback->ambient = irr::video::SColor(255,20,20,20);

	plainLightShader = irr::video::EMT_SOLID; // Fallback material type
    plainLightCallback= new PlainLightShaderCallBack;
    plainLightShader = (irr::video::E_MATERIAL_TYPE)irrGpu->addHighLevelShaderMaterialFromFiles("GFX/shaders/LightingVert.vert", "main", irr::video::EVST_VS_1_1,"GFX/shaders/LightingFrag.frag", "main", irr::video::EPST_PS_1_1,plainLightCallback, irr::video::EMT_SOLID);
    plainLightCallback->ambient = irr::video::SColor(255,20,20,20);

	postProcShader = irr::video::EMT_SOLID; // Fallback material type
    postProcCallback= new PostProcShaderCallBack;
    postProcShader = (irr::video::E_MATERIAL_TYPE)irrGpu->addHighLevelShaderMaterialFromFiles("GFX/shaders/PostProcessVert.vert", "main", irr::video::EVST_VS_1_1,"GFX/shaders/PostProcessFrag.frag", "main", irr::video::EPST_PS_1_1,postProcCallback, irr::video::EMT_SOLID);

	zBufferShader = irr::video::EMT_SOLID; // Fallback material type
    zBufferCallback= new ZBufferShaderCallBack;
    zBufferShader = (irr::video::E_MATERIAL_TYPE)irrGpu->addHighLevelShaderMaterialFromFiles("GFX/shaders/ZBufferVert.vert", "main", irr::video::EVST_VS_1_1,"GFX/shaders/ZBufferFrag.frag", "main", irr::video::EPST_PS_1_1,zBufferCallback, irr::video::EMT_SOLID);

    blurImage = irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(width,height),"blurImage",irr::video::ECF_R8G8B8);
    blurImage2 = irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(width,height),"blurImage2",irr::video::ECF_R8G8B8);
    zBuffer = irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(width,height),"ZBuffer",irr::video::ECF_R16F);
    finalImage = irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(width,height),"finalImage",irr::video::ECF_R8G8B8);

    lightPass[0] = irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(1024,1024),"lightPass0",irr::video::ECF_R8G8B8);
    lightPass[1] = irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(1024,1024),"lightPass1",irr::video::ECF_R8G8B8);
    lightDepth = irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(1024,1024),"lightDepth",irr::video::ECF_R16F);

    reflection = irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(64,64),"relection",irr::video::ECF_R8G8B8);

    fogTexture = irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(width,height),"fogTexture",irr::video::ECF_A8R8G8B8);

    irr::video::ITexture* fogBillTex = irrDriver->getTexture("GFX/fogBillboard.png");

    irr::scene::SMesh* quadMesh = new irr::scene::SMesh();
	irr::scene::SMeshBuffer* buf = new irr::scene::SMeshBuffer();

	quadMesh->addMeshBuffer(buf);

	irr::video::S3DVertex verts[4];
	verts[0]=irr::video::S3DVertex(irr::core::vector3df(0.f,0.f,0.f),irr::core::vector3df(0.f,0.f,1.f),irr::video::SColor(255,255,0,0),irr::core::vector2df(0.f,0.f));
	verts[1]=irr::video::S3DVertex(irr::core::vector3df(1.f,0.f,0.f),irr::core::vector3df(0.f,0.f,1.f),irr::video::SColor(255,0,0,255),irr::core::vector2df(1.f,0.f));
	verts[2]=irr::video::S3DVertex(irr::core::vector3df(0.f,1.f,0.f),irr::core::vector3df(0.f,0.f,1.f),irr::video::SColor(255,255,255,0),irr::core::vector2df(0.f,1.f));
	verts[3]=irr::video::S3DVertex(irr::core::vector3df(1.f,1.f,0.f),irr::core::vector3df(0.f,0.f,1.f),irr::video::SColor(255,0,255,0),irr::core::vector2df(1.f,1.f));
	irr::u16 inds[] {
		1,0,2,
		2,3,1
	};
	buf->Vertices.reallocate(4);
	buf->Vertices.set_used(4);
	for (unsigned int j=0;j<4;j++) {
		buf->Vertices[j]=verts[j];
	}
	buf->Indices.reallocate(6);
	buf->Indices.set_used(6);
	for (unsigned int j=0;j<6;j++) {
		buf->Indices[j]=inds[j];
	}
	buf->getMaterial().setTexture(0,blurImage);
	buf->getMaterial().setTexture(1,zBuffer);
	buf->getMaterial().setTexture(2,fogTexture);

	buf->getMaterial().MaterialType = (irr::video::E_MATERIAL_TYPE)postProcShader;

	buf->recalculateBoundingBox();

	buf->drop();
	screenQuad = irrSmgr->addMeshSceneNode(quadMesh);
	screenQuad->setVisible(false);

    blinkMeterIMG = irrDriver->getTexture("GFX/BlinkMeter.jpg");
    staminaMeterIMG = irrDriver->getTexture("GFX/staminaMeter.jpg");

	//Add test model

    dynamics = new irrDynamics();
    //gContactAddedCallback = CustomMaterialCombinerCallback;
    dynamics->setGravity(-100*RoomScale);

	irr::scene::IMeshSceneNode* node = nullptr;
    //dynRegister* ItemDyn = new dynRegister(dynamics);
	Item::setDynamics(dynamics);
	Item::setDriver(irrDriver);
	Item::setDimensions(mainWidth,mainHeight);

    Item420::setMeshNode(genItemNode(std::string("GFX/Items/420.x"),std::string(""),0.015f*RoomScale));
	ItemKey1::setMeshNode(genItemNode(std::string("GFX/Items/keycard.x"),std::string("GFX/Items/keycard1.jpg"),0.012f*RoomScale));
	ItemKey2::setMeshNode(genItemNode(std::string("GFX/Items/keycard.x"),std::string("GFX/Items/keycard2.jpg"),0.012f*RoomScale));
	ItemKey3::setMeshNode(genItemNode(std::string("GFX/Items/keycard.x"),std::string("GFX/Items/keycard3.jpg"),0.012f*RoomScale));
	ItemKey4::setMeshNode(genItemNode(std::string("GFX/Items/keycard.x"),std::string("GFX/Items/keycard4.jpg"),0.012f*RoomScale));
	ItemKey5::setMeshNode(genItemNode(std::string("GFX/Items/keycard.x"),std::string("GFX/Items/keycard5.jpg"),0.012f*RoomScale));
	ItemMisc::setMeshNode(genItemNode(std::string("GFX/Items/keycard.x"),std::string("GFX/Items/card.jpg"),0.012f*RoomScale));
	ItemKey6::setMeshNode(genItemNode(std::string("GFX/Items/keycard.x"),std::string("GFX/Items/keycardomni.jpg"),0.012f*RoomScale));
	ItemScp860::setMeshNode(genItemNode(std::string("GFX/Items/key.b3d"),std::string(""),0.03f*RoomScale));
	ItemPaper::setMeshNode(genItemNode(std::string("GFX/Items/paper.x"),std::string(""),0.09f*RoomScale));
	ItemVest::setMeshNode(genItemNode(std::string("GFX/Items/vest.x"),std::string("GFX/NPCs/MTFbody.jpg"),0.6f*RoomScale));
	ItemFinevest::setMeshNode(genItemNode(std::string("GFX/Items/vest.x"),std::string("GFX/NPCs/MTFbody.jpg"),0.66f*RoomScale));
	ItemVeryfinevest::setMeshNode(genItemNode(std::string("GFX/Items/vest.x"),std::string("GFX/NPCs/MTFbody.jpg"),0.75f*RoomScale));
	ItemHazmatsuit::setMeshNode(genItemNode(std::string("GFX/Items/hazmat.b3d"),std::string(""),0.39f*RoomScale));
	ItemHazmatsuit2::setMeshNode(genItemNode(std::string("GFX/Items/hazmat.b3d"),std::string(""),0.39f*RoomScale));
	ItemHazmatsuit3::setMeshNode(genItemNode(std::string("GFX/Items/hazmat.b3d"),std::string(""),0.39f*RoomScale));
	ItemCup::setMeshNode(genItemNode(std::string("GFX/Items/cup.x"),std::string(""),1.2f*RoomScale));
	ItemEmptycup::setMeshNode(genItemNode(std::string("GFX/Items/cup.x"),std::string(""),1.2f*RoomScale));
	ItemScp500::setMeshNode(genItemNode(std::string("GFX/Items/pill.b3d"),std::string(""),0.03f*RoomScale));
	ItemFirstaid::setMeshNode(genItemNode(std::string("GFX/Items/firstaid.x"),std::string(""),1.5f*RoomScale));
	ItemFinefirstaid::setMeshNode(genItemNode(std::string("GFX/Items/firstaid.x"),std::string(""),0.9f*RoomScale));
	ItemFirstaid2::setMeshNode(genItemNode(std::string("GFX/Items/firstaid.x"),std::string("GFX/Items/firstaidkit2.jpg"),0.9f*RoomScale));
	ItemVeryfinefirstaid::setMeshNode(genItemNode(std::string("GFX/Items/eyedrops.b3d"),std::string("GFX/Items/bottle.jpg"),0.06f*RoomScale));
	ItemGasmask::setMeshNode(genItemNode(std::string("GFX/Items/gasmask.b3d"),std::string(""),0.6f*RoomScale));
	ItemSupergasmask::setMeshNode(genItemNode(std::string("GFX/Items/gasmask.b3d"),std::string(""),0.63f*RoomScale));
	ItemGasmask3::setMeshNode(genItemNode(std::string("GFX/Items/gasmask.b3d"),std::string(""),0.63f*RoomScale));
	ItemBat::setMeshNode(genItemNode(std::string("GFX/Items/battery.x"),std::string(""),0.006f*RoomScale));
	ItemScp148::setMeshNode(genItemNode(std::string("GFX/Items/metalpanel.x"),std::string(""),0.117187f*RoomScale));
	ItemScp148ingot::setMeshNode(genItemNode(std::string("GFX/Items/scp148.x"),std::string(""),0.117187f*RoomScale));
	ItemNav::setMeshNode(genItemNode(std::string("GFX/Items/navigator.x"),std::string(""),0.024f*RoomScale));
	ItemRadio::setMeshNode(genItemNode(std::string("GFX/Items/radio.x"),std::string(""),30.0f*RoomScale));
	ItemFineradio::setMeshNode(genItemNode(std::string("GFX/Items/radio.x"),std::string(""),0.03f*RoomScale));
	ItemVeryfineradio::setMeshNode(genItemNode(std::string("GFX/Items/radio.x"),std::string(""),0.03f*RoomScale));
	Item18vradio::setMeshNode(genItemNode(std::string("GFX/Items/radio.x"),std::string(""),0.036f*RoomScale));
	ItemCigarette::setMeshNode(genItemNode(std::string("GFX/Items/420.x"),std::string(""),0.012f*RoomScale));
	Item420s::setMeshNode(genItemNode(std::string("GFX/Items/420.x"),std::string(""),0.012f*RoomScale));
	ItemHand::setMeshNode(genItemNode(std::string("GFX/Items/severedhand.b3d"),std::string(""),1.2f*RoomScale));
	ItemHand2::setMeshNode(genItemNode(std::string("GFX/Items/severedhand.b3d"),std::string("GFX/Items/shand2.png"),1.2f*RoomScale));
	Item18vbat::setMeshNode(genItemNode(std::string("GFX/Items/battery.x"),std::string(""),0.009f*RoomScale));
	ItemKillbat::setMeshNode(genItemNode(std::string("GFX/Items/battery.x"),std::string(""),0.009f*RoomScale));
	ItemFineeyedrops::setMeshNode(genItemNode(std::string("GFX/Items/eyedrops.b3d"),std::string(""),0.036f*RoomScale));
	ItemSupereyedrops::setMeshNode(genItemNode(std::string("GFX/Items/eyedrops.b3d"),std::string(""),0.036f*RoomScale));
	ItemEyedrops::setMeshNode(genItemNode(std::string("GFX/Items/eyedrops.b3d"),std::string(""),0.036f*RoomScale));
	ItemScp714::setMeshNode(genItemNode(std::string("GFX/Items/scp714.b3d"),std::string(""),9.0f*RoomScale));
	ItemScp1025::setMeshNode(genItemNode(std::string("GFX/Items/scp1025.b3d"),std::string(""),3.0f*RoomScale));
	ItemScp513::setMeshNode(genItemNode(std::string("GFX/Items/513.x"),std::string(""),3.0f*RoomScale));

	for (irr::u32 ui=0;ui<2;ui++) {
        Item* it = ItemEyedrops::createItemEyedrops();
        itemList.push_back(it);

		it = ItemSupergasmask::createItemSupergasmask();
        itemList.push_back(it);

        it = ItemKey2::createItemKey2();
        itemList.push_back(it);

        it = ItemNav::createItemNav();
        itemList.push_back(it);
        it = ItemMisc::createItemMisc();
        itemList.push_back(it);
        it = ItemScp714::createItemScp714();
        itemList.push_back(it);
        it = ItemPaper::createItemPaper();
        itemList.push_back(it);

        it = ItemKey3::createItemKey3();
        itemList.push_back(it);

        it = ItemKey4::createItemKey4();
        itemList.push_back(it);

        it = ItemKey5::createItemKey5();
        itemList.push_back(it);

        it = ItemKey6::createItemKey6();
        itemList.push_back(it);
    }

	btRigidBody* rbody;

	Room::setDynamics(dynamics);

	ambient[0] = Sound::getSound(std::string("SFX/Music/The Dread.ogg"),false);
	if (ambient[0]!=nullptr) {
		ambient[0]->playSound(true);
	}
	RMesh* rme;

	irr::video::E_MATERIAL_TYPE roomShaders[5];
	roomShaders[0] = roomShader;
	roomShaders[1] = roomShader_noNormals;
	roomShaders[2] = vertLightShader;
	roomShaders[3] = vertLightShader_alpha;
	roomShaders[4] = zBufferShader;

    irr::video::ITexture* textures[7];
    textures[0] = reflection;//lightPass[0];
    textures[1] = lightDepth;
    textures[2] = lightPass[1];

    textures[3] = irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(1024,1024),"lightPass0_t",irr::video::ECF_R8G8B8);
    textures[4] = irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(1024,1024),"lightPass1_t",irr::video::ECF_R8G8B8);
    textures[5] = irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(1024,1024),"lightPass0_b",irr::video::ECF_R8G8B8);
    textures[6] = irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(1024,1024),"lightPass1_b",irr::video::ECF_R8G8B8);

#if 1
	//LCZ
	/*RoomStart*/rme = loadRMesh(std::string("GFX/map/173_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); RoomStart::setBase(rme);
	/*Room2storage*/rme = loadRMesh(std::string("GFX/map/Room2storage_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2storage::setBase(rme);
	/*Room3storage*/rme = loadRMesh(std::string("GFX/map/Room3storage_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room3storage::setBase(rme);
	/*Room012*/rme = loadRMesh(std::string("GFX/map/Room012_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room012::setBase(rme);
	/*Room2*/rme = loadRMesh(std::string("GFX/map/Room2_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2::setBase(rme);
	/*Room2_2*/rme = loadRMesh(std::string("GFX/map/Room2_2_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2_2::setBase(rme);
	/*Room2c*/rme = loadRMesh(std::string("GFX/map/Room2C_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2c::setBase(rme);
	/*Room2closets*/rme = loadRMesh(std::string("GFX/map/Room2closets_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2closets::setBase(rme);
	/*Room2elevator*/rme = loadRMesh(std::string("GFX/map/Room2elevator_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2elevator::setBase(rme);
	/*Room2doors*/rme = loadRMesh(std::string("GFX/map/Room2doors_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2doors::setBase(rme);
	/*Room2scps*/rme = loadRMesh(std::string("GFX/map/Room2scps_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2scps::setBase(rme);
	/*Room3storage*/rme = loadRMesh(std::string("GFX/map/Room3storage_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room3storage::setBase(rme);
	/*Room2testRoom2*/rme = loadRMesh(std::string("GFX/map/Room2testRoom2_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2testRoom2::setBase(rme);
	/*Room3*/rme = loadRMesh(std::string("GFX/map/Room3_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room3::setBase(rme);
	/*Room3_2*/rme = loadRMesh(std::string("GFX/map/Room3_2_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room3_2::setBase(rme);
	/*Room4*/rme = loadRMesh(std::string("GFX/map/Room4_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room4::setBase(rme);
	/*Roompj*/rme = loadRMesh(std::string("GFX/map/Roompj_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Roompj::setBase(rme);
	/*Room914*/rme = loadRMesh(std::string("GFX/map/machineRoom_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room914::setBase(rme);
#endif
#if 0
	//HCZ
	/*Room008*/rme = loadRMesh(std::string("GFX/map/008_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room008::setBase(rme);
	/*RoomCoffin*/rme = loadRMesh(std::string("GFX/map/coffin_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); RoomCoffin::setBase(rme);
	/*RoomEndroom2*/rme = loadRMesh(std::string("GFX/map/endroom2_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); RoomEndroom2::setBase(rme);
	/*RoomTestroom*/rme = loadRMesh(std::string("GFX/map/testRoom_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); RoomTestroom::setBase(rme);
	/*RoomTunnel*/rme = loadRMesh(std::string("GFX/map/tunnel_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); RoomTunnel::setBase(rme);
	/*RoomTunnel2*/rme = loadRMesh(std::string("GFX/map/tunnel2_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); RoomTunnel2::setBase(rme);
	/*Room035*/rme = loadRMesh(std::string("GFX/map/Room035_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room035::setBase(rme);
	/*Room049*/rme = loadRMesh(std::string("GFX/map/Room049_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room049::setBase(rme);
	/*Room106*/rme = loadRMesh(std::string("GFX/map/Room106_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room106::setBase(rme);
	/*Room2ctunnel*/rme = loadRMesh(std::string("GFX/map/Room2Ctunnel_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2ctunnel::setBase(rme);
	/*Room2nuke*/rme = loadRMesh(std::string("GFX/map/Room2nuke_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2nuke::setBase(rme);
	/*Room2pipes*/rme = loadRMesh(std::string("GFX/map/Room2pipes_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2pipes::setBase(rme);
	/*Room2pit*/rme = loadRMesh(std::string("GFX/map/Room2pit_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2pit::setBase(rme);
	/*Room3pit*/rme = loadRMesh(std::string("GFX/map/Room3pit_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room3pit::setBase(rme);
	/*Room2servers*/rme = loadRMesh(std::string("GFX/map/Room2servers_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2servers::setBase(rme);
	/*Room2tunnel*/rme = loadRMesh(std::string("GFX/map/Room2tunnel_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2tunnel::setBase(rme);
	/*Room3tunnel*/rme = loadRMesh(std::string("GFX/map/Room3tunnel_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room3tunnel::setBase(rme);
	/*Room4tunnels*/rme = loadRMesh(std::string("GFX/map/4tunnels_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room4tunnels::setBase(rme);
	/*Room513*/rme = loadRMesh(std::string("GFX/map/Room513_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room513::setBase(rme);
#endif
#if 0
	//EZ
	/*Room860*/rme = loadRMesh(std::string("GFX/map/Room860_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room860::setBase(rme);
	/*RoomExit1*/rme = loadRMesh(std::string("GFX/map/exit1_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); RoomExit1::setBase(rme);
	/*RoomGateaentrance*/rme = loadRMesh(std::string("GFX/map/gateaentrance_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); RoomGateaentrance::setBase(rme);
	/*RoomLockroom2*/rme = loadRMesh(std::string("GFX/map/lockroom2_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); RoomLockroom2::setBase(rme);

	/*Room2z3*/rme = loadRMesh(std::string("GFX/map/Room2z3_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2z3::setBase(rme);
	/*Room2cafeteria*/rme = loadRMesh(std::string("GFX/map/Room2cafeteria_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2cafeteria::setBase(rme);
	/*Room2cz3*/rme = loadRMesh(std::string("GFX/map/Room2Cz3_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2cz3::setBase(rme);
	/*Room2ccont*/rme = loadRMesh(std::string("GFX/map/Room2ccont_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2ccont::setBase(rme);
	/*Room2offices*/rme = loadRMesh(std::string("GFX/map/Room2offices_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2offices::setBase(rme);
	/*Room2offices2*/rme = loadRMesh(std::string("GFX/map/Room2offices2_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2offices2::setBase(rme);
	/*Room2offices3*/rme = loadRMesh(std::string("GFX/map/Room2offices3_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2offices3::setBase(rme);
	/*Room2poffices*/rme = loadRMesh(std::string("GFX/map/Room2poffices_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2poffices::setBase(rme);
	/*Room2poffices2*/rme = loadRMesh(std::string("GFX/map/Room2poffices2_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2poffices2::setBase(rme);
	/*Room2sroom*/rme = loadRMesh(std::string("GFX/map/Room2sroom_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2sroom::setBase(rme);
	/*Room2toilets*/rme = loadRMesh(std::string("GFX/map/Room2toilets_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2toilets::setBase(rme);
	/*Room2tesla*/rme = loadRMesh(std::string("GFX/map/Room2tesla_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room2tesla::setBase(rme);
	/*Room3servers*/rme = loadRMesh(std::string("GFX/map/Room3servers_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room3servers::setBase(rme);
	/*Room3servers2*/rme = loadRMesh(std::string("GFX/map/Room3servers2_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room3servers2::setBase(rme);
	/*Room3z3*/rme = loadRMesh(std::string("GFX/map/Room3z3_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room3z3::setBase(rme);
	/*Room4z3*/rme = loadRMesh(std::string("GFX/map/Room4z3_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room4z3::setBase(rme);
#endif
#if 1
	//Misc
	/*Room173*/rme = loadRMesh(std::string("GFX/map/173bright_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room173::setBase(rme);
	/*RoomCheckpoint1*/rme = loadRMesh(std::string("GFX/map/checkpoint1_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); RoomCheckpoint1::setBase(rme);
	/*RoomCheckpoint2*/rme = loadRMesh(std::string("GFX/map/checkpoint2_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); RoomCheckpoint2::setBase(rme);
	/*RoomGatea*/rme = loadRMesh(std::string("GFX/map/gatea_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); RoomGatea::setBase(rme);
	/*RoomPocketdimension*/rme = loadRMesh(std::string("GFX/map/pocketdimension1_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); RoomPocketdimension::setBase(rme);
#endif
    /*Room079*/rme = loadRMesh(std::string("GFX/map/room079_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); Room079::setBase(rme);
	/*RoomLockroom*/rme = loadRMesh(std::string("GFX/map/lockroom_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); RoomLockroom::setBase(rme);
	/*RoomEndroom*/rme = loadRMesh(std::string("GFX/map/endroom_opt.rm2"),irrFileSystem,irrSmgr,textures,roomShaders,plainLightCallback); RoomEndroom::setBase(rme);

    NPC::owner = this;
    NPC::dynamics = dynamics;
    NPC096::baseNode = irrSmgr->addAnimatedMeshSceneNode(irrSmgr->getMesh("GFX/NPCs/scp096.b3d"));
    NPC096::baseNode->getMaterial(0).MaterialType = normalsShader;
    NPC096::baseNode->getMaterial(0).setTexture(1,irrDriver->getTexture("GFX/NPCs/normal_flat.png"));
    NPC096::baseNode->getMaterial(0).setTexture(2,irrDriver->getTexture("GFX/NPCs/SCP096_specular.png"));
    NPC096::baseNode->getMaterial(1).MaterialType = normalsShader;
    NPC096::baseNode->getMaterial(1).setTexture(1,irrDriver->getTexture("GFX/NPCs/normal_flat.png"));
    NPC096::baseNode->getMaterial(1).setTexture(2,irrDriver->getTexture("GFX/NPCs/SCP096_specular.png"));
    NPC096::baseNode->getMaterial(2).MaterialType = plainLightShader;
    NPC096::baseNode->setScale(irr::core::vector3df(4.f,4.f,4.f));
    NPC096::baseNode->setFrameLoop(1059,1074);
    NPC096::baseNode->setAnimationSpeed(64.0f);

    NPC173::baseNode = irrSmgr->addMeshSceneNode(irrSmgr->getMesh("GFX/NPCs/173_2.b3d"));

    NPC173::baseNode->setScale(irr::core::vector3df(1.3*RoomScale));

    NPC173::baseOcclusionNode = irrSmgr->addCubeSceneNode(10.0f,nullptr,-1,irr::core::vector3df(0,0,0),irr::core::vector3df(0,0,0),irr::core::vector3df(0.7f, 2.2f, 0.7f));
    NPC173::baseOcclusionNode->getMaterial(0).MaterialType = irr::video::EMT_TRANSPARENT_ADD_COLOR;
    NPC173::driver = irrDriver;
    //node->setPosition(irr::core::vector3df(x*204.8f*RoomScale,10.f*RoomScale,y*204.8f*RoomScale));
    //node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
    //node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);

    //node->getMaterial(0).Lighting = true;
    NPC173::baseNode->getMaterial(0).MaterialType = (irr::video::E_MATERIAL_TYPE)normalsShader;

    //rbody = dynamics->addTriMesh_moving(node,16000.f,20,1,1);
    //rbody->setAngularFactor(btVector3(0,0,0));

    NPC173::baseNode->getMaterial(0).setTexture(1, irrDriver->getTexture("GFX/NPCs/173_norm.jpg"));
    NPC173::baseNode->getMaterial(0).setTexture(2, irrDriver->getTexture("GFX/NPCs/173_Spec.jpg"));

    //testNPC = NPC173::createNPC173();
    //static_cast<NPC173*>(testNPC)->boxNode = irrSmgr->addCubeSceneNode();

	mainPlayer = new Player(this,irrSmgr,dynamics,irrReceiver);

	//mainPlayer->update();

	Item::setPlayer(mainPlayer);
	NPC::player = mainPlayer; //add setplayer?

	mainPlayer->testNode = NPC173::baseNode;

    Door::baseDoorNode[0] = irrSmgr->addMeshSceneNode(irrSmgr->getMesh("GFX/map/Door01.x"));
    Door::baseDoorNode[0]->getMaterial(0).MaterialType = (irr::video::E_MATERIAL_TYPE)plainLightShader;
    Door::baseDoorNode[0]->setScale(irr::core::vector3df(RoomScale*1.95f,RoomScale*1.35f,RoomScale*1.95f));
    Door::baseFrameNode = irrSmgr->addMeshSceneNode(irrSmgr->getMesh("GFX/map/DoorFrame.x"));
    Door::baseFrameNode->getMaterial(0).MaterialType = (irr::video::E_MATERIAL_TYPE)plainLightShader;
    Door::baseFrameNode->getMaterial(1).MaterialType = (irr::video::E_MATERIAL_TYPE)plainLightShader;
    Door::baseFrameNode->setScale(irr::core::vector3df(RoomScale*0.1f,RoomScale*0.1f,RoomScale*0.12f));
    Door::baseButtonNode[0] = irrSmgr->addMeshSceneNode(irrSmgr->getMesh("GFX/map/Button.x"));
    Door::baseButtonNode[0]->getMaterial(0).MaterialType = (irr::video::E_MATERIAL_TYPE)plainLightShader;
    Door::baseButtonNode[0]->setScale(irr::core::vector3df(RoomScale*1.f,RoomScale*1.f,RoomScale*1.f));
    Door::dynamics = dynamics;
    //rbody = dynamics->addBoxObject(Door::baseFrameNode,1000.f);
    //rbody->setLinearFactor(btVector3(0.f,0.f,0.f));

	createMap(0);
	//testNPC->update();
	//mainPlayer->teleport(testNPC->getPosition()+irr::core::vector3df(0.f,30.f,0.f));
	/*Door::baseDoorNode[0]->setPosition(mainPlayer->getPosition()-irr::core::vector3df(0.f,mainPlayer->getPosition().Y,1018.f*0.1f*RoomScale));
	Door::baseFrameNode->setPosition(mainPlayer->getPosition()-irr::core::vector3df(0.f,mainPlayer->getPosition().Y,1024.f*0.1f*RoomScale));
	Door::baseButtonNode[0]->setPosition(mainPlayer->getPosition()-irr::core::vector3df(150.f*0.1f*RoomScale,mainPlayer->getPosition().Y-165.f*0.1f*RoomScale,1000.f*0.1f*RoomScale));
	Door::baseButtonNode[0]->setRotation(irr::core::vector3df(0.f,180.f,0.f));
	btTransform rTransform = rbody->getCenterOfMassTransform();
	rTransform.setOrigin(irrToBtVec(Door::baseFrameNode->getPosition()));
	rbody->setCenterOfMassTransform(rTransform);*/

	for (int i=0;i<50;i++) {
        fogBillboards[i]=irrSmgr->addBillboardSceneNode(nullptr,irr::core::dimension2df(500.f*0.1f*RoomScale,500.f*0.1f*RoomScale));
        fogBillboards[i]->setMaterialTexture(0,fogBillTex);
        fogBillboards[i]->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        fogBillboards[i]->setMaterialType(fogBillboardShader);
        fogBillboards[i]->getMaterial(0).ZBuffer = 0;
        fogBillboards[i]->setPosition(mainPlayer->getPosition()+irr::core::vector3df((rand()%1024-512)*0.1f*RoomScale,-(rand()%100)*0.1f*RoomScale,(rand()%1024-512)*0.1f*RoomScale));
        //fogBillAlpha[i]=(rand()%256)/255.f;
        fogBillTargetPos[i]=fogBillboards[i]->getPosition();
        fogBillboards[i]->setVisible(false);
    }

	irrDevice->getCursorControl()->setVisible(false);
	irrDevice->getCursorControl()->setPosition((irr::s32)mainWidth/2,(irr::s32)mainHeight/2);

	for (int y=19;y>=0;y--) {
		for (int x=19;x>=0;x--) {
			if (roomArray[x][y]!=nullptr) {
				//test node

				//------------

				/*mesh1 = irrSmgr->getMesh("GFX/NPCs/scp-066.b3d");

				node = irrSmgr->addMeshSceneNode(mesh1);


				node->setScale(irr::core::vector3df(2.1*RoomScale));
				node->setPosition(irr::core::vector3df(x*204.8f*RoomScale,10*RoomScale,y*204.8f*RoomScale));

				rbody = dynamics->addTriMesh_moving(node,5.0f,5,1,1);
				rbody->setFriction(10.0f);
				rbody->setDamping(0,0);
				node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);

				node->getMaterial(0).setTexture(1, irrDriver->getTexture("GFX/NPCs/scp-066_normal.png"));
				node->getMaterial(0).setTexture(2, irrDriver->getTexture("GFX/NPCs/scp-066_specular.png"));
				node->getMaterial(1).MaterialType = (irr::video::E_MATERIAL_TYPE)NormalsShader;
				node->getMaterial(1).setTexture(1, irrDriver->getTexture("GFX/NPCs/scp-066_normal.png"));
				node->getMaterial(1).setTexture(2, irrDriver->getTexture("GFX/NPCs/scp-066_specular.png"));
				node->getMaterial(0).MaterialType = (irr::video::E_MATERIAL_TYPE)NormalsShader;

				node->getMaterial(0).SpecularColor.set(0,0,0,0);
				node->getMaterial(0).Shininess = 0.f;
				node->getMaterial(1).SpecularColor.set(0,0,0,0);
				node->getMaterial(1).Shininess = 0.0f;*/

				y=-1;

				break;
			}
		}
	}

	for (unsigned char i=0;i<3;i++) {
		pauseImgs[i]=nullptr;
	}
	for (unsigned char i=0;i<inventory_size;i++) {
		invImgs[i]=nullptr;
	}
}

World::~World() {


    if (mainPlayer!=nullptr) { delete mainPlayer; mainPlayer=nullptr; }
    for (unsigned char i=0;i<inventory_size;i++) {
		if (invImgs[i]!=nullptr) { irrDriver->removeTexture(invImgs[i]); invImgs[i]=nullptr; }
    }
    for (unsigned char i=0;i<3;i++) {
		if (pauseImgs[i]!=nullptr) { irrDriver->removeTexture(pauseImgs[i]); pauseImgs[i]=nullptr; }
    }
    Sound::killSounds();
    irrDevice->drop();

    delete irrReceiver;

	auto a = irr::LeakHunter::getReferenceCountedObjects();
	std::cout<<"Leaked IReferenceCount: "<<a.size()<<"\n";

	for (unsigned int i=0;i<a.size();i++) {
		if (a[i]->getDebugName()) {
			std::cout<<a[i]->getDebugName();
			if (std::string(a[i]->getDebugName())==std::string("COpenGLTexture")) {
				std::cout<<" -> "<<dynamic_cast<const irr::video::ITexture*>(a[i])->getName().getPath().c_str();
			}
			std::cout<<"\n";
		}
	}

	gContactAddedCallback = CustomMaterialCombinerCallback;
}

bool World::run() {
    time = irrTimer->getRealTime();
    irrReceiver->setTime(time);

	/*if (prevTime==0) { fpsFactor = 1.0; } else {
		fpsFactor = (time-prevTime)/(1000.0/70.0);
	}*/
	prevTime = time;

	if (menusOpen==menus::NONE) {

		float prec = 0.65f;

        std::cout<<mainPlayer->getPosition().X<<"\n";
		mainPlayer->update();
		dynamics->simStep(1.f/60.f,60.f * prec);
		mainPlayer->resetSpeeds();
		//testNPC->update();
		//testNPC->updateModel();

		if (irrReceiver->IsMouseDown(0) != mainPlayer->getLastMouseDown(0) && irrReceiver->IsMouseDown(0)==false) {
			for (unsigned int i=0;i<itemList.size();i++) {
				irr::scene::SViewFrustum frust = *mainPlayer->getViewFrustum();
				//transform the frustum to the node's current absolute transformation
				irr::core::matrix4 invTrans(itemList[i]->getTransform(), irr::core::matrix4::EM4CONST_INVERSE);
				//invTrans.makeInverse();
				frust.transform(invTrans);
				if (!itemList[i]->getPicked()) {
					if (mainPlayer->seesBoundingBox(itemList[i]->getBBox(),frust)) {
						mainPlayer->addToInventory(itemList[i]);
						break;
					}
				}
			}
		}
		if (irrReceiver->IsMouseDown(1) != mainPlayer->getLastMouseDown(1) && irrReceiver->IsMouseDown(1)==false) {
			for (unsigned int i=0;i<inventory_size;i++) {
				if (mainPlayer->takeFromInventory(i)) break;
			}
		}

		mainPlayer->updateHead();

		int px,py;
		px = coordToRoomGrid(mainPlayer->getPosition().X);
		py = coordToRoomGrid(mainPlayer->getPosition().Z);
		if (px>=0 && px<20 && py>=0 && py<20) {
			if (roomArray[px][py]!=nullptr) {
				if (ppx!=px || ppy!=py) {
					for (int y=0;y<20;y++) {
						for (int x=0;x<20;x++) {
							if (roomArray[x][y]!=nullptr) {
								if (std::max(std::abs(x-px),std::abs(y-py))<3) {
									roomArray[x][y]->setActivation(true);
								} else {
									roomArray[x][y]->setActivation(false);
								}
							}
						}
					}
					std::vector<pointLight> nLights = roomArray[px][py]->getPointLights();
					if (roomArray[px][py+1]!=nullptr && py+1<20) {
                        const std::vector<pointLight> &addLights = roomArray[px][py+1]->getPointLights();
                        for (unsigned int i=0;i<addLights.size();i++) {
                            nLights.push_back(addLights[i]);
                        }
					}
					if (roomArray[px][py-1]!=nullptr && py-1>0) {
                        const std::vector<pointLight> &addLights = roomArray[px][py-1]->getPointLights();
                        for (unsigned int i=0;i<addLights.size();i++) {
                            nLights.push_back(addLights[i]);
                        }
					}
					if (roomArray[px+1][py]!=nullptr && px+1<20) {
                        const std::vector<pointLight> &addLights = roomArray[px+1][py]->getPointLights();
                        for (unsigned int i=0;i<addLights.size();i++) {
                            nLights.push_back(addLights[i]);
                        }
					}
					if (roomArray[px-1][py]!=nullptr && px-1>0) {
                        const std::vector<pointLight> &addLights = roomArray[px-1][py]->getPointLights();
                        for (unsigned int i=0;i<addLights.size();i++) {
                            nLights.push_back(addLights[i]);
                        }
					}

					/*if (roomArray[px][py+2]!=nullptr && py+2<20) {
                        const std::vector<pointLight> &addLights = roomArray[px][py+2]->getPointLights();
                        for (unsigned int i=0;i<addLights.size();i++) {
                            nLights.push_back(addLights[i]);
                        }
					}
					if (roomArray[px][py-2]!=nullptr && py-2>0) {
                        const std::vector<pointLight> &addLights = roomArray[px][py-2]->getPointLights();
                        for (unsigned int i=0;i<addLights.size();i++) {
                            nLights.push_back(addLights[i]);
                        }
					}
					if (roomArray[px+2][py]!=nullptr && px+2<20) {
                        const std::vector<pointLight> &addLights = roomArray[px+2][py]->getPointLights();
                        for (unsigned int i=0;i<addLights.size();i++) {
                            nLights.push_back(addLights[i]);
                        }
					}
					if (roomArray[px-2][py]!=nullptr && px-2>0) {
                        const std::vector<pointLight> &addLights = roomArray[px-2][py]->getPointLights();
                        for (unsigned int i=0;i<addLights.size();i++) {
                            nLights.push_back(addLights[i]);
                        }
					}*/
					plainLightCallback->setLights(nLights);
					normalsCallback->setLights(nLights);
					ppx = px;
					ppy = py;
				}
			}
		}

		irr::core::position2di mousePos = irrReceiver->getMousePos();
		if (mousePos != irr::core::position2di(mainWidth/2,mainHeight/2)) {
			mainPlayer->yaw += ((int)mousePos.X-(int)(mainWidth/2))*0.1f;
			mainPlayer->pitch += ((int)mousePos.Y-(int)(mainHeight/2))*0.1f;
			irrDevice->getCursorControl()->setPosition((irr::s32)mainWidth/2,(irr::s32)mainHeight/2);
		}

		if (irrReceiver->IsKeyDown(irr::KEY_TAB)==false && irrReceiver->IsPrevKeyDown(irr::KEY_TAB)==true) {
			menusOpen = menus::INVOPEN;
		}
		if (irrReceiver->IsPrevKeyDown(irr::KEY_ESCAPE)==true && irrReceiver->IsKeyDown(irr::KEY_ESCAPE)==false) {
			menusOpen = menus::PAUSEOPEN;
		}
	} else {
		//dynamics->resetTimer(time);
	}

    irrDriver->beginScene(true, true, irr::video::SColor(255, 255, 0, 255));

	draw3D();

	mainPlayer->booststamina(10.f,-1.f);

	drawHUD();

    irrDriver->endScene();

    time = irrTimer->getRealTime();
	if (time-prevTime<17) irrDevice->sleep(17-(time-prevTime));

	Sound::processDrops();
	irrReceiver->perLoopUpdate();
	prevMenusOpen = menusOpen;
    return irrDevice->run();
}

void World::draw3D() {
	//postProcCallback->fpsFactor = fpsFactor;

    /*irrDriver->setRenderTarget(fogTexture);

    //draw a triangle with a different material to make alpha blending for the billboards work properly
    irr::video::SMaterial mat;
    mat.MaterialType = VertLightShader;
    irrDriver->setMaterial(mat);
    irr::core::vector3df vert1(0.f,0.f,0.f);
    irr::core::vector3df vert2(0.f,-1.f,0.f);
    irr::core::vector3df vert3(0.f,-1.f,-1.f);
    irrDriver->draw3DTriangle(irr::core::triangle3df(vert1,vert2,vert3),irr::video::SColor(255,0,0,0));

    for (int i=0;i<50;i++) {
        irr::core::matrix4 rotMatrix;
        float dist = mainPlayer->getPosition().getDistanceFrom(fogBillTargetPos[i]);
        if (dist<200.f*0.1f*RoomScale) {
            fogBillTargetPos[i]=(fogBillTargetPos[i]+(fogBillTargetPos[i]-mainPlayer->getPosition())/dist*20.f);
        } else if (dist>1000.f*0.1f*RoomScale) {
            fogBillTargetPos[i]=(mainPlayer->getPosition()-(fogBillTargetPos[i]-mainPlayer->getPosition())*((990.f*0.1f*RoomScale)/dist));
            fogBillTargetPos[i].Y = mainPlayer->getPosition().Y-(rand()%100);
            fogBillboards[i]->setPosition(fogBillTargetPos[i]);
        } else {
            rotMatrix.setRotationDegrees(irr::core::vector3df((rand()%100-50)/25.f,(rand()%100-50)/12.f,(rand()%100-50)/150.f)*0.2f);
            fogBillTargetPos[i]+=irr::core::vector3df(rand()%3-1,rand()%3-1,rand()%3-1)*0.1f;
            irr::core::vector3df diff = fogBillTargetPos[i]-mainPlayer->getPosition();
            rotMatrix.transformVect(diff);
            fogBillTargetPos[i]=mainPlayer->getPosition()+diff;
        }

        if ((fogBillTargetPos[i].Y-mainPlayer->getPosition().Y)>100.f) {
            fogBillTargetPos[i].Y-=100.f;
        }
        if ((mainPlayer->getPosition().Y-fogBillTargetPos[i].Y)>100.f) {
            fogBillTargetPos[i].Y+=100.f;
        }

        for (int j=0;j<50;j++) {
            if (j!=i) {
                float dist3 = fogBillTargetPos[i].getDistanceFrom(fogBillTargetPos[j]);
                if (dist3<50.f*0.1f*RoomScale) {
                    fogBillTargetPos[i]=(fogBillTargetPos[i]+(fogBillTargetPos[i]-fogBillTargetPos[j])/dist3*40.f*0.1f*RoomScale);
                }
            }
        }

        fogBillboards[i]->updateAbsolutePosition();
        fogBillboards[i]->setPosition(fogBillboards[i]->getAbsolutePosition()*(1.f-(0.05f))+fogBillTargetPos[i]*0.05f);
        fogBillboards[i]->updateAbsolutePosition();

        /*float dist2 = mainPlayer->getPosition().getDistanceFrom(fogBillboards[i]->getAbsolutePosition());
        if (dist2<400.f*0.1f*RoomScale) {
            unsigned char bcolor = std::max((dist2-(100.f*0.1f*RoomScale))/(300.f*0.1f*RoomScale),0.f)*1;
            fogBillboards[i]->setColor(irr::video::SColor(255,bcolor,bcolor,bcolor));
        } else if (dist2>800.f*0.1*RoomScale) {
            unsigned char bcolor = std::max(((1010.f*0.1f*RoomScale)-dist2)/(210.f*0.1f*RoomScale),0.f)*1;
            fogBillboards[i]->setColor(irr::video::SColor(255,bcolor,bcolor,bcolor));
        } else {
            fogBillboards[i]->setColor(irr::video::SColor(255,1,1,1));
        }*\/

        fogBillboards[i]->setVisible(true);
        fogBillboards[i]->render();
        fogBillboards[i]->setVisible(false);
    }

    RoomCallback->reflectFactor=0.f;
    irrDriver->setRenderTarget(reflection,true,true);
    mainPlayer->reflectNY();
    irrSmgr->fastDrawAll();
    mainPlayer->resetCam();
    RoomCallback->reflectFactor=1.f;
	irrDriver->setRenderTarget(blurImage2); //copy the old render
    irrDriver->draw2DImage(finalImage,irr::core::position2d<irr::s32>(0,0),irr::core::rect<irr::s32>(0,0,mainWidth,mainHeight), 0,irr::video::SColor(255,255,255,255), false);
    irrDriver->setRenderTarget(blurImage); //create a new render, using the old one to add a blur effect

    irrSmgr->drawAll();

    float blinkTimer = mainPlayer->blinkTimer;
    if (blinkTimer<0) {
        float darkA = 0.f;
        if (mainPlayer->blinkTimer>=-0.5f) {
			darkA = std::max(0.f,std::min(-mainPlayer->blinkTimer*2.f,1.f));
        } else if (mainPlayer->blinkTimer>=-1.0f) {
            darkA = std::max(0.f,std::min(1.f+(mainPlayer->blinkTimer+0.5f)*2.f,1.f));
        }
        darkA = 1.f-darkA;
        darkA *= darkA;
        darkA = 1.f-darkA;
        irrDriver->draw2DRectangle(irr::video::SColor(std::min(255.f,darkA*255.f),0,0,0),irr::core::rect<irr::s32>(0,0,mainWidth,mainHeight));
        postProcCallback->minBlur = darkA*6.f;
    } else {
		if (ItemSelected==true) {
            postProcCallback->minBlur=2.f;
        } else {
            postProcCallback->minBlur=0.f;
        }
		if (menusOpen!=menus::NONE) postProcCallback->minBlur = 2.f;
    }

    irrDriver->setRenderTarget(ZBuffer,true,true,irr::video::SColor(255,255,255,255)); //white = far

    irrDriver->getOverrideMaterial().EnableFlags = irr::video::EMF_MATERIAL_TYPE;
	irrDriver->getOverrideMaterial().EnablePasses = irr::scene::ESNRP_SOLID;
	irrDriver->getOverrideMaterial().Material.MaterialType = (irr::video::E_MATERIAL_TYPE)ZBufferShader;
	irrSmgr->fastDrawAll();
	irrDriver->getOverrideMaterial().EnablePasses = 0;

    irrDriver->setRenderTarget(finalImage); //draw to screen
    //irrDriver->clearZBuffer();

	//screenQuad->render();
	//irrDriver->draw2DImage(blurImage2,irr::core::position2d<irr::s32>(0,0),irr::core::rect<irr::s32>(0,0,mainWidth,mainHeight), 0,irr::video::SColor(std::min(blurAlpha,200.0f),255,255,255), false);

    renderLights();
    irrDriver->draw2DImage(lightPass[0],irr::core::position2d<irr::s32>(0,0),irr::core::rect<irr::s32>(0,0,mainWidth,mainHeight),0);

	irrDriver->setRenderTarget(0);
	irrDriver->draw2DImage(lightPass[0],irr::core::position2d<irr::s32>(0,0),irr::core::rect<irr::s32>(0,0,mainWidth,mainHeight), 0,irr::video::SColor(255,255,255,255), false);

    */
    //renderLights();
    irrDriver->setRenderTarget(0);
    irrSmgr->drawAll();
    irrDriver->runAllOcclusionQueries(true);
    irrDriver->updateAllOcclusionQueries();
}

void World::drawHUD() {

    itemSelected = mainPlayer->drawSelectedItem();

	if (hudMsgTimer>0.f) {
		hudMsgTimer-=1.f;

		float alpha = std::min(std::max(hudMsgTimer*0.01f,0.f),1.f);
		font1->draw(hudMsg.c_str(),irr::core::rect<irr::s32>(mainWidth/2+2,mainHeight+2,mainWidth/2+2,mainHeight/2+102),irr::video::SColor(150*alpha,0,0,0),true,true);
		font1->draw(hudMsg.c_str(),irr::core::rect<irr::s32>(mainWidth/2,mainHeight,mainWidth/2,mainHeight/2+100),irr::video::SColor(255*alpha,255,255,255),true,true);
	}

    //Blink meter
    irrDriver->draw2DLine(irr::core::position2d<irr::s32>(80*scale2Db,mainHeight-95*scale2Db),irr::core::position2d<irr::s32>(284*scale2Db,mainHeight-95*scale2Db),irr::video::SColor(255,255,255,255));
    irrDriver->draw2DLine(irr::core::position2d<irr::s32>(80*scale2Db,mainHeight-75*scale2Db),irr::core::position2d<irr::s32>(284*scale2Db,mainHeight-75*scale2Db),irr::video::SColor(255,255,255,255));
    irrDriver->draw2DLine(irr::core::position2d<irr::s32>(80*scale2Db,mainHeight-95*scale2Db),irr::core::position2d<irr::s32>(80*scale2Db,mainHeight-75*scale2Db),irr::video::SColor(255,255,255,255));
    irrDriver->draw2DLine(irr::core::position2d<irr::s32>(284*scale2Db,mainHeight-95*scale2Db),irr::core::position2d<irr::s32>(284*scale2Db,mainHeight-75*scale2Db),irr::video::SColor(255,255,255,255));

    for (int i=0;i<mainPlayer->blinkTimer/100*20;i++) {
        irrDriver->draw2DImage(blinkMeterIMG,irr::core::recti((83+i*10)*scale2Db,mainHeight-92*scale2Db,(91+i*10)*scale2Db,mainHeight-78*scale2Db),irr::core::recti(0,0,8,14));
    }

    //stamina meter
    irrDriver->draw2DLine(irr::core::position2d<irr::s32>(80*scale2Db,mainHeight-55*scale2Db),irr::core::position2d<irr::s32>(284*scale2Db,mainHeight-55*scale2Db),irr::video::SColor(255,255,255,255));
    irrDriver->draw2DLine(irr::core::position2d<irr::s32>(80*scale2Db,mainHeight-35*scale2Db),irr::core::position2d<irr::s32>(284*scale2Db,mainHeight-35*scale2Db),irr::video::SColor(255,255,255,255));
    irrDriver->draw2DLine(irr::core::position2d<irr::s32>(80*scale2Db,mainHeight-55*scale2Db),irr::core::position2d<irr::s32>(80*scale2Db,mainHeight-35*scale2Db),irr::video::SColor(255,255,255,255));
    irrDriver->draw2DLine(irr::core::position2d<irr::s32>(284*scale2Db,mainHeight-55*scale2Db),irr::core::position2d<irr::s32>(284*scale2Db,mainHeight-35*scale2Db),irr::video::SColor(255,255,255,255));

    for (int i=0;i<mainPlayer->stamina/100*20;i++) {
        irrDriver->draw2DImage(staminaMeterIMG,irr::core::recti((83+i*10)*scale2Db,mainHeight-52*scale2Db,(91+i*10)*scale2Db,mainHeight-38*scale2Db),irr::core::recti(0,0,8,14));
    }

    blurAlpha = 100;

    for (int y=19;y>=0;y--) {
        for (int x=19;x>=0;x--) {
            if (roomArray[x][y]!=nullptr) {
                irrDriver->draw2DRectangle(irr::video::SColor(255,100,100,100),irr::core::recti((19-x)*10,y*10,(19-x)*10+8,y*10+8));
            }
        }
    }
    //irrDriver->draw2DRectangle(irr::video::SColor(255,255,0,0),irr::core::recti((19-coordToRoomGrid(testNPC->getPosition().X))*10,coordToRoomGrid(testNPC->getPosition().Z)*10,(19-coordToRoomGrid(testNPC->getPosition().X))*10+8,coordToRoomGrid(testNPC->getPosition().Z)*10+8));
    irrDriver->draw2DRectangle(irr::video::SColor(255,0,255,0),irr::core::recti((19-coordToRoomGrid(mainPlayer->getPosition().X))*10,coordToRoomGrid(mainPlayer->getPosition().Z)*10,(19-coordToRoomGrid(mainPlayer->getPosition().X))*10+8,coordToRoomGrid(mainPlayer->getPosition().Z)*10+8));

#if 0
    //{ startPathCode
    irr::core::vector2di startPos;
    irr::core::vector2di endPos;

    /*for (int y=19;y>=0;y--) {
        for (int x=19;x>=0;x--) {
            if (roomArray[x][y]!=nullptr) {
                startPos.X = x; startPos.Y = y;
                y=-1;
                break;
            }
        }
    }*/

    for (int y=0;y<20;y++) {
        for (int x=0;x<20;x++) {
            if (roomArray[x][y]!=nullptr) {
                startPos.X = x; startPos.Y = y;
                y=20;
                break;
            }
        }
    }
    endPos.X = coordToRoomGrid(mainPlayer->getPosition().X); endPos.Y = coordToRoomGrid(mainPlayer->getPosition().Z);

    std::vector<irr::core::vector2di> rPath;

    getRoomList(startPos,endPos,rPath);

    for (int y=19;y>=0;y--) {
        for (int x=19;x>=0;x--) {
            if (roomArray[x][y]!=nullptr) {
                irrDriver->draw2DRectangle(irr::video::SColor(255,100,100,100),irr::core::recti((19-x)*10,y*10,(19-x)*10+8,y*10+8));
            }
        }
    }

    if (rPath.size()>0) {
        if (rPath[0].X==startPos.X) {
            if (rPath[0].Y<startPos.Y) {
                for (int y=rPath[0].Y;y<=startPos.Y;y++) {
                    irrDriver->draw2DRectangle(irr::video::SColor(255,255,255,0),irr::core::recti((19-startPos.X)*10,y*10,(19-startPos.X)*10+8,y*10+8));
                }
            } else {
                for (int y=startPos.Y;y<=rPath[0].Y;y++) {
                    irrDriver->draw2DRectangle(irr::video::SColor(255,255,255,0),irr::core::recti((19-startPos.X)*10,y*10,(19-startPos.X)*10+8,y*10+8));
                }
            }
        } else {
            if (rPath[0].X<startPos.X) {
                for (int x=rPath[0].X;x<=startPos.X;x++) {
                    irrDriver->draw2DRectangle(irr::video::SColor(255,255,255,0),irr::core::recti((19-x)*10,startPos.Y*10,(19-x)*10+8,startPos.Y*10+8));
                }
            } else {
                for (int x=startPos.X;x<=rPath[0].X;x++) {
                    irrDriver->draw2DRectangle(irr::video::SColor(255,255,255,0),irr::core::recti((19-x)*10,startPos.Y*10,(19-x)*10+8,startPos.Y*10+8));
                }
            }
        }

        for (unsigned int i=1;i<rPath.size();i++) {
            if (rPath[i-1].X==rPath[i].X) {
                if (rPath[i-1].Y<rPath[i].Y) {
                    for (int y=rPath[i-1].Y;y<rPath[i].Y;y++) {
                        irrDriver->draw2DRectangle(irr::video::SColor(255,255,255,0),irr::core::recti((19-rPath[i].X)*10,y*10,(19-rPath[i].X)*10+8,y*10+8));
                    }
                } else {
                    for (int y=rPath[i].Y;y<rPath[i-1].Y;y++) {
                        irrDriver->draw2DRectangle(irr::video::SColor(255,255,255,0),irr::core::recti((19-rPath[i].X)*10,y*10,(19-rPath[i].X)*10+8,y*10+8));
                    }
                }
            } else {
                if (rPath[i-1].X<rPath[i].X) {
                    for (int x=rPath[i-1].X;x<rPath[i].X;x++) {
                        irrDriver->draw2DRectangle(irr::video::SColor(255,255,255,0),irr::core::recti((19-x)*10,rPath[i].Y*10,(19-x)*10+8,rPath[i].Y*10+8));
                    }
                } else {
                    for (int x=rPath[i].X;x<rPath[i-1].X;x++) {
                        irrDriver->draw2DRectangle(irr::video::SColor(255,255,255,0),irr::core::recti((19-x)*10,rPath[i].Y*10,(19-x)*10+8,rPath[i].Y*10+8));
                    }
                }
            }
            irrDriver->draw2DRectangle(irr::video::SColor(255,255,255,0),irr::core::recti((19-rPath[i].X)*10,rPath[i].Y*10,(19-rPath[i].X)*10+8,rPath[i].Y*10+8));
            //std::cout<<"drawingline"<<i<<": "<<rPath[i].X<<" "<<rPath[i].Y<<"\n";
            //irrDriver->draw3DBox(irr::core::aabbox3df(irr::core::vector3df(rPath[i].X-0.5f,2.f*RoomScale,rPath[i].Y-0.5f),irr::core::vector3df(rPath[i].X+0.5f,22.f*RoomScale,rPath[i].Y+0.5f)));//Line(irr::core::vector3df(rPath[i-1].X,2.f*RoomScale,rPath[i-1].Y),irr::core::vector3df(rPath[i].X,2.f*RoomScale,rPath[i].Y),irr::video::SColor(255,255,0,0));
        }

        if (rPath[rPath.size()-1].X==endPos.X) {
            if (rPath[rPath.size()-1].Y<endPos.Y) {
                for (int y=rPath[rPath.size()-1].Y;y<=endPos.Y;y++) {
                    irrDriver->draw2DRectangle(irr::video::SColor(255,255,255,0),irr::core::recti((19-endPos.X)*10,y*10,(19-endPos.X)*10+8,y*10+8));
                }
            } else {
                for (int y=endPos.Y;y<=rPath[rPath.size()-1].Y;y++) {
                    irrDriver->draw2DRectangle(irr::video::SColor(255,255,255,0),irr::core::recti((19-endPos.X)*10,y*10,(19-endPos.X)*10+8,y*10+8));
                }
            }
        } else {
            if (rPath[rPath.size()-1].X<endPos.X) {
                for (int x=rPath[rPath.size()-1].X;x<=endPos.X;x++) {
                    irrDriver->draw2DRectangle(irr::video::SColor(255,255,255,0),irr::core::recti((19-x)*10,endPos.Y*10,(19-x)*10+8,endPos.Y*10+8));
                }
            } else {
                for (int x=endPos.X;x<=rPath[rPath.size()-1].X;x++) {
                    irrDriver->draw2DRectangle(irr::video::SColor(255,255,255,0),irr::core::recti((19-x)*10,endPos.Y*10,(19-x)*10+8,endPos.Y*10+8));
                }
            }
        }
    }

    irrDriver->draw2DRectangle(irr::video::SColor(255,255,0,255),irr::core::recti((19-startPos.X)*10,startPos.Y*10,(19-startPos.X)*10+8,startPos.Y*10+8));
    irrDriver->draw2DRectangle(irr::video::SColor(255,0,255,0),irr::core::recti((19-endPos.X)*10,endPos.Y*10,(19-endPos.X)*10+8,endPos.Y*10+8));
    //} endPathCode
#endif

    if (menusOpen==menus::INVOPEN) {
        mainPlayer->selectItem(inventory_size);
		if (prevMenusOpen!=menusOpen) {
			Sound::freezeCategory(1);
			irrReceiver->perLoopUpdate();
			for (unsigned char i=0;i<inventory_size;i++) {
				std::string imgpath = mainPlayer->getItemInvImg(i);
				if (imgpath!="") {
					invImgs[i]=irrDriver->getTexture(imgpath.c_str());
				}
			}
			irrDevice->getCursorControl()->setVisible(true);
		}
		for (unsigned char i=0;i<inventory_size;i++) {
			int x,y,w,h;
			x = mainWidth/2-(90*scale2Db*inventory_size/4)+(i%5*90*scale2Db);
			y = mainHeight/2-(90*scale2Db*inventory_size/10)+(i/5*90*scale2Db);
			w = 64;
			h = 64;
			irrDriver->draw2DRectangle(irr::video::SColor(255,255,255,255),irr::core::recti(irr::core::position2di(x-2*scale2Db,y-2*scale2Db),irr::core::position2di(x+(w+2)*scale2Db,y+(h+2)*scale2Db)));
			if (invImgs[i]!=nullptr && i!=dragItem) {
				irrDriver->draw2DImage(invImgs[i],irr::core::recti(x,y,x+w*scale2Db,y+h*scale2Db),irr::core::rect<irr::s32>(0,0,w,h));
				if (irrReceiver->getMousePos().X>x && irrReceiver->getMousePos().X<x+w*scale2Db && irrReceiver->getMousePos().Y>y && irrReceiver->getMousePos().Y<y+h*scale2Db) {
					font1->draw(mainPlayer->getItemName(i).c_str(),irr::core::recti(irr::core::position2di(x+2,y+(h+8)*scale2Db),irr::core::position2di(x+(w+2)*scale2Db,y+(h+24)*scale2Db)),irr::video::SColor(100,0,0,0),true,true);
					font1->draw(mainPlayer->getItemName(i).c_str(),irr::core::recti(irr::core::position2di(x,y+(h+6)*scale2Db),irr::core::position2di(x+w*scale2Db,y+(h+22)*scale2Db)),irr::video::SColor(255,255,255,255),true,true);
					if (irrReceiver->IsDoubleClick(0)) {
                        mainPlayer->selectItem(i);
						menusOpen=menus::NONE;
					} else if (irrReceiver->IsMouseDown(0) && dragItem>=inventory_size) {
						dragItem = i;
					}
				}
			} else {
				irrDriver->draw2DRectangle(irr::video::SColor(255,0,0,0),irr::core::recti(irr::core::position2di(x,y),irr::core::position2di(x+w*scale2Db,y+h*scale2Db)));
			}
		}
		if (dragItem<inventory_size) {
			if (invImgs[dragItem]==nullptr) {
				dragItem = inventory_size;
			} else {
				int x,y,w,h;
				unsigned int dist = 100*scale2Db;
				unsigned char targetSlot = inventory_size;
				for (unsigned char i=0;i<inventory_size;i++) {
					x = mainWidth/2-(90*scale2Db*inventory_size/4)+(i%5*90*scale2Db);
					y = mainHeight/2-(90*scale2Db*inventory_size/10)+(i/5*90*scale2Db);
					w = 64;
					h = 64;

					int xd,yd;
					xd = x+w*scale2Db/2-irrReceiver->getMousePos().X;
					yd = y+h*scale2Db/2-irrReceiver->getMousePos().Y;

					if ((irr::u32)irr::core::squareroot(xd*xd+yd*yd)<dist) {
						dist = irr::core::squareroot(xd*xd+yd*yd);
						targetSlot = i;
					}
				}
				if (targetSlot<inventory_size) {
					x = mainWidth/2-(90*scale2Db*inventory_size/4)+(targetSlot%5*90*scale2Db);
					y = mainHeight/2-(90*scale2Db*inventory_size/10)+(targetSlot/5*90*scale2Db);
					w = 64;
					h = 64;

					irr::video::SColor colors[4];
					colors[0]=irr::video::SColor(100,255,255,255);
					colors[1]=colors[0];
					colors[2]=colors[0];
					colors[3]=colors[0];

					irrDriver->draw2DImage(invImgs[dragItem],irr::core::recti(x,y,x+w*scale2Db,y+h*scale2Db),irr::core::recti(0,0,w,h),nullptr,colors);
				}
				x = irrReceiver->getMousePos().X-32;
				y = irrReceiver->getMousePos().Y-32;
				irrDriver->draw2DImage(invImgs[dragItem],irr::core::recti(x,y,x+w*scale2Db,y+h*scale2Db),irr::core::rect<irr::s32>(0,0,w,h));
				if (!irrReceiver->IsMouseDown(0)) {
					if (targetSlot!=dragItem) {
						if (targetSlot<inventory_size) {
							if (mainPlayer->moveToSlot(dragItem,targetSlot)==1) {
								invImgs[targetSlot] = invImgs[dragItem];
								invImgs[dragItem] = nullptr;
							} else {
								hudMsg = "This item can't be used this way";
								hudMsgTimer = 70.f*2.f;
							}
						} else {
							mainPlayer->takeFromInventory(dragItem);
							menusOpen=menus::NONE;
						}
					}
					dragItem = inventory_size;
				}
			}
		}
		if (irrReceiver->IsKeyDown(irr::KEY_TAB)==false && irrReceiver->IsPrevKeyDown(irr::KEY_TAB)==true) menusOpen = menus::NONE;
		if (menusOpen!=menus::INVOPEN) {
			Sound::unfreezeCategory(1);
			for (unsigned char i=0;i<inventory_size;i++) {
				if (invImgs[i]!=nullptr) {
					irrDriver->removeTexture(invImgs[i]);
					for (unsigned char j=0;j<inventory_size;j++) {
						if (invImgs[j]==invImgs[i] && i!=j) invImgs[j]=nullptr;
					}
					invImgs[i]=nullptr;
				}
			}
			irrDevice->getCursorControl()->setVisible(false);
			irrDevice->getCursorControl()->setPosition((irr::s32)mainWidth/2,(irr::s32)mainHeight/2);
		}
	} else if (menusOpen==menus::PAUSEOPEN || menusOpen==menus::OPTIONSOPEN) {
        mainPlayer->selectItem(inventory_size);
		if (pauseImgs[0]==nullptr) {
			pauseImgs[0] = irrDriver->getTexture("GFX/menu/pausemenu.jpg");
			pauseImgs[1] = irrDriver->getTexture("GFX/menu/menublack.jpg");
			pauseImgs[2] = irrDriver->getTexture("GFX/menu/menuwhite.jpg");
			irrReceiver->perLoopUpdate();
			irrDevice->getCursorControl()->setVisible(true);
			Sound::freezeCategory(1);
		}
		irr::video::SColor corners[] {
			irr::video::SColor(255,255,255,255),
			irr::video::SColor(255,255,255,255),
			irr::video::SColor(255,255,255,255),
			irr::video::SColor(255,255,255,255)
		};

		irrDriver->draw2DImage(pauseImgs[0],irr::core::recti(mainWidth/2-300*scale2D,mainHeight/2-300*scale2D,mainWidth/2+300*scale2D,mainHeight/2+300*scale2D),irr::core::recti(0,0,600,600),nullptr,corners);

		if (menusOpen==menus::PAUSEOPEN) {
			font2->draw("PAUSED",irr::core::recti(mainWidth/2-180*scale2D,mainHeight/2-300*scale2D,mainWidth/2+300*scale2D,mainHeight/2-205*scale2D),irr::video::SColor(255,255,255,255),true,true);

			if (button(std::string("Resume"),mainWidth/2-180*scale2D,mainHeight/2,455*scale2D,70*scale2D)) { menusOpen = menus::NONE; }
			if (button(std::string("Options"),mainWidth/2-180*scale2D,mainHeight/2+80*scale2D,455*scale2D,70*scale2D)) { menusOpen = menus::OPTIONSOPEN; }
			if (button(std::string("Quit"),mainWidth/2-180*scale2D,mainHeight/2+160*scale2D,455*scale2D,70*scale2D)) { irrDevice->closeDevice(); }//TODO: add main menu

			if (irrReceiver->IsPrevKeyDown(irr::KEY_ESCAPE)==true && irrReceiver->IsKeyDown(irr::KEY_ESCAPE)==false) menusOpen = menus::NONE;
		} else {
			font2->draw("OPTIONS",irr::core::recti(mainWidth/2-180*scale2D,mainHeight/2-300*scale2D,mainWidth/2+300*scale2D,mainHeight/2-205*scale2D),irr::video::SColor(255,255,255,255),true,true);

			if (button(std::string("Graphics"),mainWidth/2-175*scale2D,mainHeight/2-190*scale2D,140*scale2D,24*scale2D)) { subMenusOpen=0; }
			if (button(std::string("Audio"),mainWidth/2-20*scale2D,mainHeight/2-190*scale2D,140*scale2D,24*scale2D)) { subMenusOpen=1; }
			if (button(std::string("Controls"),mainWidth/2+135*scale2D,mainHeight/2-190*scale2D,140*scale2D,24*scale2D)) { subMenusOpen=2; }

			irrDriver->draw2DImage(pauseImgs[2],irr::core::recti(mainWidth/2-180*scale2D,mainHeight/2-150*scale2D,mainWidth/2+280*scale2D,mainHeight/2+200*scale2D),irr::core::recti(mainWidth/2-180*scale2D,mainHeight/2-150*scale2D,mainWidth/2+280*scale2D,mainHeight/2+200*scale2D));
			irrDriver->draw2DImage(pauseImgs[1],irr::core::recti(mainWidth/2-177*scale2D,mainHeight/2-147*scale2D,mainWidth/2+277*scale2D,mainHeight/2+197*scale2D),irr::core::recti(mainWidth/2-177*scale2D,mainHeight/2-147*scale2D,mainWidth/2+277*scale2D,mainHeight/2+197*scale2D));

			irr::core::recti outline;
			std::stringstream ss;
			std::string s;
			unsigned char color;
			switch (subMenusOpen) {
				case 0:
					irrDriver->draw2DImage(pauseImgs[2],irr::core::recti(mainWidth/2-175*scale2D,mainHeight/2-167*scale2D,mainWidth/2-36*scale2D,mainHeight/2-150*scale2D),irr::core::recti(mainWidth/2-175*scale2D,mainHeight/2-167*scale2D,mainWidth/2-36*scale2D,mainHeight/2-150*scale2D));
					irrDriver->draw2DImage(pauseImgs[1],irr::core::recti(mainWidth/2-173*scale2D,mainHeight/2-170*scale2D,mainWidth/2-39*scale2D,mainHeight/2-147*scale2D),irr::core::recti(mainWidth/2-172*scale2D,mainHeight/2-170*scale2D,mainWidth/2-33*scale2D,mainHeight/2-147*scale2D));

					ss << "Gamma: ";
					ss << postProcCallback->gammaFactor;
					s = ss.str();

					font1->draw(s.c_str(),irr::core::recti(mainWidth/2-150*scale2D,mainHeight/2-125*scale2D,mainWidth/2+100*scale2D,mainHeight/2-110*scale2D),irr::video::SColor(255,255,255,255),false,false);

					outline = irr::core::recti(mainWidth/2-150*scale2D,mainHeight/2-100*scale2D,mainWidth/2+260*scale2D,mainHeight/2-86*scale2D);
					irrDriver->draw2DRectangleOutline(irr::core::recti(outline.UpperLeftCorner.X-3*scale2D,outline.UpperLeftCorner.Y-3*scale2D,outline.LowerRightCorner.X+3*scale2D,outline.LowerRightCorner.Y+3*scale2D));

					if (irrReceiver->IsMouseDown(0)) {
						if (irrReceiver->getMousePos().X>outline.UpperLeftCorner.X && irrReceiver->getMousePos().Y>outline.UpperLeftCorner.Y &&
							irrReceiver->getMousePos().X<outline.LowerRightCorner.X && irrReceiver->getMousePos().Y<outline.LowerRightCorner.Y) {
							postProcCallback->gammaFactor = ((irrReceiver->getMousePos().X-outline.UpperLeftCorner.X)/(float)(outline.LowerRightCorner.X-outline.UpperLeftCorner.X))*1.5f+0.5f;
							postProcCallback->gammaFactor = ((int)(postProcCallback->gammaFactor*100.f))*0.01f;
							postProcCallback->invGammaFactor = 1.f/postProcCallback->gammaFactor;
						}
					}

					irrDriver->draw2DImage(blinkMeterIMG,irr::core::recti(
						mainWidth/2-150*scale2D+(postProcCallback->gammaFactor-0.5f)/1.5f*406*scale2D,
						mainHeight/2-100*scale2D,
						mainWidth/2-142*scale2D+(postProcCallback->gammaFactor-0.5f)/1.5f*406*scale2D,mainHeight/2-86*scale2D),
						irr::core::recti(0,0,8,14));

					irrDriver->draw2DRectangle(irr::video::SColor(255,0,0,0),irr::core::recti(mainWidth/2-150*scale2D,mainHeight/2-65*scale2D,mainWidth/2+250*scale2D,mainHeight/2+100*scale2D));

					color = std::pow(2.5f/255.f,postProcCallback->invGammaFactor)*255;
					irrDriver->draw2DRectangle(irr::video::SColor(255,color,color,color),irr::core::recti(mainWidth/2-100*scale2D,mainHeight/2-20*scale2D,mainWidth/2-32*scale2D,mainHeight/2+48*scale2D));
					color = std::pow(1.25f/255.f,postProcCallback->invGammaFactor)*255;
					irrDriver->draw2DRectangle(irr::video::SColor(255,color,color,color),irr::core::recti(mainWidth/2+132*scale2D,mainHeight/2-20*scale2D,mainWidth/2+200*scale2D,mainHeight/2+48*scale2D));
					font1->draw("Drag the slider so that the left square",irr::core::recti(mainWidth/2-150*scale2D,mainHeight/2+105*scale2D,mainWidth/2+100*scale2D,mainHeight/2+120*scale2D),irr::video::SColor(255,255,255,255),false,false);
					font1->draw("is visible but the right square isn't",irr::core::recti(mainWidth/2-150*scale2D,mainHeight/2+120*scale2D,mainWidth/2+100*scale2D,mainHeight/2+135*scale2D),irr::video::SColor(255,255,255,255),false,false);
				break;
				case 1:
					irrDriver->draw2DImage(pauseImgs[2],irr::core::recti(mainWidth/2-20*scale2D,mainHeight/2-167*scale2D,mainWidth/2+119*scale2D,mainHeight/2-150*scale2D),irr::core::recti(mainWidth/2-20*scale2D,mainHeight/2-167*scale2D,mainWidth/2+119*scale2D,mainHeight/2-150*scale2D));
					irrDriver->draw2DImage(pauseImgs[1],irr::core::recti(mainWidth/2-18*scale2D,mainHeight/2-170*scale2D,mainWidth/2+116*scale2D,mainHeight/2-147*scale2D),irr::core::recti(mainWidth/2-18*scale2D,mainHeight/2-170*scale2D,mainWidth/2+116*scale2D,mainHeight/2-147*scale2D));
				break;
				case 2:
					irrDriver->draw2DImage(pauseImgs[2],irr::core::recti(mainWidth/2+135*scale2D,mainHeight/2-167*scale2D,mainWidth/2+274*scale2D,mainHeight/2-150*scale2D),irr::core::recti(mainWidth/2+135*scale2D,mainHeight/2-167*scale2D,mainWidth/2+274*scale2D,mainHeight/2-150*scale2D));
					irrDriver->draw2DImage(pauseImgs[1],irr::core::recti(mainWidth/2+137*scale2D,mainHeight/2-170*scale2D,mainWidth/2+271*scale2D,mainHeight/2-147*scale2D),irr::core::recti(mainWidth/2+137*scale2D,mainHeight/2-170*scale2D,mainWidth/2+271*scale2D,mainHeight/2-147*scale2D));
				break;
			};

			if (irrReceiver->IsPrevKeyDown(irr::KEY_ESCAPE)==true && irrReceiver->IsKeyDown(irr::KEY_ESCAPE)==false) menusOpen = menus::PAUSEOPEN;
		}

		if (menusOpen!=menus::PAUSEOPEN && menusOpen!=menus::OPTIONSOPEN) {
			irrDriver->removeTexture(pauseImgs[0]); pauseImgs[0]=nullptr;
			irrDriver->removeTexture(pauseImgs[1]); pauseImgs[1]=nullptr;
			irrDriver->removeTexture(pauseImgs[2]); pauseImgs[2]=nullptr;
			irrDevice->getCursorControl()->setVisible(false);
			irrDevice->getCursorControl()->setPosition((irr::s32)mainWidth/2,(irr::s32)mainHeight/2);
			Sound::unfreezeCategory(1);
		}
	}
}

void World::drawFog() {

}

bool World::button(const std::string &text,int x,int y,int w,int h) {
	irr::gui::CGUITTFont* buttonFont = font1;
	if (h>68*scale2D) {
		buttonFont = font2;
	}
	bool clicking = false;
	irrDriver->draw2DImage(pauseImgs[2],irr::core::recti(x,y,x+w,y+h),irr::core::recti(x,y,x+w,y+h));
	if (irrReceiver->getMousePos().X>x && irrReceiver->getMousePos().X<x+w && irrReceiver->getMousePos().Y>y && irrReceiver->getMousePos().Y<y+h) {
		irrDriver->draw2DRectangle(irr::video::SColor(255,50,50,50),irr::core::recti(x+3*scale2D,y+3*scale2D,x+w-2*scale2D,y+h-2*scale2D));
		if (irrReceiver->IsMouseDown(0)==false && irrReceiver->IsPrevMouseDown(0)==true) clicking = true;
	} else {
		irrDriver->draw2DImage(pauseImgs[1],irr::core::recti(x+3*scale2D,y+3*scale2D,x+w-2*scale2D,y+h-2*scale2D),irr::core::recti(x+2*scale2D,y+2*scale2D,x+w-2*scale2D,y+h-2*scale2D));
	}
	buttonFont->draw(text.c_str(),irr::core::recti(x,y,x+w,y+h),irr::video::SColor(255,255,255,255),true,true,nullptr);
	return clicking;
}

/*float World::getFPSfactor() {
    return fpsFactor;
}*/

void trimFileName(std::string &inStr) {
	for (unsigned int i=inStr.size();i>0;i--) {
		if (inStr[i]=='/' || inStr[i]=='\\') {
			std::string newStr;
			for (unsigned int j=i+1;j<inStr.size();j++) {
				newStr.push_back(inStr[j]);
			}
			inStr = newStr;
			break;
		}
	}
}

bool getNodeTriangleTextureName(irr::scene::ISceneNode* node,const irr::core::triangle3df& tri,std::string& texname) {
	//this function requires modified Irrlicht
	if (!node) return false;

	irr::scene::IMesh* mesh = nullptr;

	irr::scene::ESCENE_NODE_TYPE type = node->getType();
	if ((type == irr::scene::ESNT_MESH) || (type == irr::scene::ESNT_OCTREE)){
		mesh = static_cast<irr::scene::IMeshSceneNode*>(node)->getMesh();
	} else if (type == irr::scene::ESNT_ANIMATED_MESH) {
		mesh = static_cast<irr::scene::IAnimatedMeshSceneNode*>(node)->getMesh()->getMesh(0);
	} else {
		return false;
	}

	if (!mesh)
		return false;

	irr::scene::IMeshBuffer* buf = mesh->getMeshBuffer(tri.index);
	irr::video::ITexture* tex = buf->getMaterial().getTexture(0);
	if (!tex)
		return false;

	texname = std::string(tex->getName().getPath().c_str());
	return true;
}

int coordToRoomGrid(float coord) {
	return (int)((coord+102.4f*RoomScale)/(204.8f*RoomScale));
}
