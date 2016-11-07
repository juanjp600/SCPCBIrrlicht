#include "World.h"
#include "Player.h"

#include "NPCs/NPC096.h"
#include "NPCs/NPC106.h"
#include "NPCs/NPC173.h"
#include "NPCs/NPC178.h"
#include "NPCs/NPC513.h"

#include "Rooms/Room.h"
#include "Rooms/RMesh.h"

#include "Rooms/LCZIncludes.h"
#include "Rooms/HCZIncludes.h"
#include "Rooms/EZIncludes.h"
#include "Rooms/MiscIncludes.h"

#include "Rooms/Door.h"

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <sstream>

World* GameObject::owner = nullptr;
irrDynamics* GameObject::dynamics = nullptr;
irr::scene::ISceneManager* GameObject::irrSmgr = nullptr;
irr::video::IVideoDriver* GameObject::irrDriver = nullptr;
MainEventReceiver* GameObject::irrReceiver = nullptr;
Player* GameObject::mainPlayer = nullptr;
unsigned short GameObject::screenWidth = 0;
unsigned short GameObject::screenHeight = 0;

void World::renderLights() {
#if 0
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
#endif
}

void World::initBasics() {
    scale2D = mainHeight/1024.f;
	scale2Db = mainHeight/720.f;

    irrDriverType = irr::video::EDT_OPENGL;
    GameObject::irrReceiver = new MainEventReceiver;
    irrDevice = irr::createDevice(irrDriverType,irr::core::dimension2d<irr::u32>(mainWidth,mainHeight),32,isFullscreen,false,false,irrReceiver);
    GameObject::screenWidth = mainWidth; GameObject::screenHeight = mainHeight;
    GameObject::irrDriver = irrDevice->getVideoDriver();
	GameObject::irrSmgr = irrDevice->getSceneManager();
	irrColl = irrSmgr->getSceneCollisionManager();

	Sound::initSounds();

	menusOpen = menus::NONE;

    irrDriver->setTextureCreationFlag(irr::video::ETCF_ALWAYS_32_BIT, true);

    irrEnv = irrDevice->getGUIEnvironment();

    irrFileSystem = irrDevice->getFileSystem();

    irrTimer = irrDevice->getTimer();
    time = irrTimer->getRealTime();
    irrReceiver->setTime(time);

    int seed = time;
	std::cout<<"Seed: "<<seed<<"\n";

    srand(seed);

    GameObject::dynamics = new irrDynamics();
    dynamics->setGravity(-100*RoomScale);
}

World::World(unsigned int width,unsigned int height,bool fullscreen) {
    mainWidth = width; mainHeight = height; isFullscreen = fullscreen;
    initBasics();

	font1 = irr::gui::CGUITTFont::createTTFont(irrEnv, "GFX/cour.ttf", 16*scale2D, true, true);
	font2 = irr::gui::CGUITTFont::createTTFont(irrEnv, "GFX/cour.ttf", 64*scale2D, true, true);

	/*smCamera = irrSmgr->addCameraSceneNode(0,irr::core::vector3df(0,0,0),irr::core::vector3df(0,0,-1));
	smCamera->setFOV(90.0*irr::core::DEGTORAD64);
	smCamera->setAspectRatio(1.f);
	smCamera->setNearValue(1.f);
    smCamera->setFarValue(200.0*RoomScale);*/

    shadersSetup();

    blurImage = irr::video::IRenderTarget(irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(width,height),"blurImage",irr::video::ECF_R8G8B8));
    blurImage2 = irr::video::IRenderTarget(irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(width,height),"blurImage2",irr::video::ECF_R8G8B8));
    zBuffer = irr::video::IRenderTarget(irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(width,height),"ZBuffer",irr::video::ECF_R16F));
    finalImage = irr::video::IRenderTarget(irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(width,height),"finalImage",irr::video::ECF_R8G8B8));

    deferredLightData.reallocate(3);
    deferredLightData.set_used(3);
    deferredLightData[0] = irr::video::IRenderTarget(irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(width,height),"deferredLightData0",irr::video::ECF_R8G8B8));
    deferredLightData[1] = irr::video::IRenderTarget(irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(width,height),"deferredLightData1",irr::video::ECF_A16B16G16R16F));
    deferredLightData[2] = irr::video::IRenderTarget(irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(width,height),"deferredLightData2",irr::video::ECF_A32B32G32R32F)); //suprisingly enough, 16bit precision isn't acceptable
    renderedLights.reallocate(2);
    renderedLights.set_used(2);
    renderedLights[0] = irr::video::IRenderTarget(irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(width,height),"renderedDiffuseLights",irr::video::ECF_R8G8B8));
    renderedLights[1] = irr::video::IRenderTarget(irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(width,height),"renderedSpecularLights",irr::video::ECF_R8G8B8));
    irrDriver->setRenderTarget(renderedLights[0].RenderTexture,true,true); irrDriver->setRenderTarget(renderedLights[1].RenderTexture,true,true); irrDriver->setRenderTarget(nullptr);

    //lightPass[0] = irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(1024,1024),"lightPass0",irr::video::ECF_R8G8B8);
    //lightPass[1] = irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(1024,1024),"lightPass1",irr::video::ECF_R8G8B8);
    //lightDepth = irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(1024,1024),"lightDepth",irr::video::ECF_R16F);

    reflection = irr::video::IRenderTarget(irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(128,128),"relection",irr::video::ECF_R8G8B8));

    fogTexture = irr::video::IRenderTarget(irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(width,height),"fogTexture",irr::video::ECF_A8R8G8B8));

    ingameRTT.reallocate(2);
    ingameRTT.set_used(2);
    ingameRTT[0] = irr::video::IRenderTarget(irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(512,512),"ingameRTT0",irr::video::ECF_R8G8B8));
    ingameRTT[1] = irr::video::IRenderTarget(irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(512,512),"ingameRTT1",irr::video::ECF_R8G8B8));

    irr::video::ITexture* fogBillTex = irrDriver->getTexture("GFX/fogBillboard.png");

    irr::scene::SMesh* quadMesh = new irr::scene::SMesh();
	irr::scene::CMeshBuffer<irr::video::S3DVertex>* buf = new irr::scene::CMeshBuffer<irr::video::S3DVertex>(irrDriver->getVertexDescriptor(0));

	quadMesh->addMeshBuffer(buf);

	irr::video::S3DVertex verts[4];
	verts[0]=irr::video::S3DVertex(irr::core::vector3df(0.f,0.f,0.f),irr::core::vector3df(0.f,0.f,1.f),irr::video::SColor(255,255,0,0),irr::core::vector2df(0.f,0.f));
	verts[1]=irr::video::S3DVertex(irr::core::vector3df(1.f,0.f,0.f),irr::core::vector3df(0.f,0.f,1.f),irr::video::SColor(255,0,0,255),irr::core::vector2df(1.f,0.f));
	verts[2]=irr::video::S3DVertex(irr::core::vector3df(0.f,1.f,0.f),irr::core::vector3df(0.f,0.f,1.f),irr::video::SColor(255,255,255,0),irr::core::vector2df(0.f,1.f));
	verts[3]=irr::video::S3DVertex(irr::core::vector3df(1.f,1.f,0.f),irr::core::vector3df(0.f,0.f,1.f),irr::video::SColor(255,0,255,0),irr::core::vector2df(1.f,1.f));

	irr::scene::CVertexBuffer<irr::video::S3DVertex>* vertexBuffer = new irr::scene::CVertexBuffer<irr::video::S3DVertex>();
    irr::scene::CIndexBuffer* indexBuffer = new irr::scene::CIndexBuffer(irr::video::EIT_16BIT);

    for (unsigned int j = 0; j<4; ++j) {
        vertexBuffer->addVertex(verts[j]);
    }

    indexBuffer->addIndex(1);
    indexBuffer->addIndex(0);
    indexBuffer->addIndex(2);
    indexBuffer->addIndex(2);
    indexBuffer->addIndex(3);
    indexBuffer->addIndex(1);

    buf->setVertexBuffer(vertexBuffer, 0);
    buf->setIndexBuffer(indexBuffer);

	buf->getMaterial().setTexture(0,blurImage.RenderTexture);
	buf->getMaterial().setTexture(1,zBuffer.RenderTexture);

	buf->getMaterial().MaterialType = (irr::video::E_MATERIAL_TYPE)postProcShader;

	buf->recalculateBoundingBox();

	buf->drop();
	screenQuad = irrSmgr->addMeshSceneNode(quadMesh);
	screenQuad->setVisible(false);
	deferredLightSphere = irrSmgr->addSphereSceneNode(1.f);
	deferredLightSphere->getMaterial(0).setTexture(0,deferredLightData[0].RenderTexture);
	deferredLightSphere->getMaterial(0).setTexture(1,deferredLightData[1].RenderTexture);
	deferredLightSphere->getMaterial(0).setTexture(2,deferredLightData[2].RenderTexture);
	deferredLightSphere->getMaterial(0).MaterialType = renderDeferredLightShader;
	deferredLightSphere->getMaterial(0).BackfaceCulling = false;
	deferredLightSphere->getMaterial(0).FrontfaceCulling = true;
	deferredLightSphere->setVisible(false);

    blinkMeterIMG = irrDriver->getTexture("GFX/BlinkMeter.jpg");
    staminaMeterIMG = irrDriver->getTexture("GFX/staminaMeter.jpg");

	//Add test model

	irr::scene::IMeshSceneNode* node = nullptr;

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

	for (irr::u32 ui=0;ui<1;ui++) {

		Item* it = ItemGasmask::createItemGasmask();
        itemList.push_back(it);

        it = ItemKey1::createItemKey1();
        itemList.push_back(it);

        it = ItemKey2::createItemKey2();
        itemList.push_back(it);

        it = ItemScp513::createItemScp513();
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

	ambient[0] = Sound::getSound(std::string("SFX/Music/The Dread.ogg"),false);
	if (ambient[0]!=nullptr) {
		ambient[0]->playSound(true);
	}
	RMesh* rme;

	irr::video::E_MATERIAL_TYPE roomShaders[4];
	roomShaders[0] = roomShader;
	roomShaders[1] = roomShader_noNormals;
	roomShaders[2] = vertLightShader;
	roomShaders[3] = vertLightShader_alpha;

    irr::video::ITexture* roomTextures[2];
    roomTextures[0] = reflection.RenderTexture;
    roomTextures[1] = fogTexture.RenderTexture;

#if 1
	//LCZ
	/*RoomStart*/rme = loadRMesh(std::string("GFX/map/173_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); RoomStart::setBase(rme);
	/*Room2storage*/rme = loadRMesh(std::string("GFX/map/Room2storage_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2storage::setBase(rme);
	/*Room3storage*/rme = loadRMesh(std::string("GFX/map/Room3storage_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room3storage::setBase(rme);
	/*Room012*/rme = loadRMesh(std::string("GFX/map/Room012_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room012::setBase(rme);
	/*Room205*/rme = loadRMesh(std::string("GFX/map/room205_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room205::setBase(rme);
	/*Room1123*/rme = loadRMesh(std::string("GFX/map/1123_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room1123::setBase(rme);
	/*Room2*/rme = loadRMesh(std::string("GFX/map/Room2_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2::setBase(rme);
	/*Room2_2*/rme = loadRMesh(std::string("GFX/map/Room2_2_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2_2::setBase(rme);
	/*Room2c*/rme = loadRMesh(std::string("GFX/map/Room2C_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2c::setBase(rme);
	/*Room2closets*/rme = loadRMesh(std::string("GFX/map/Room2closets_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2closets::setBase(rme);
	/*Room2elevator*/rme = loadRMesh(std::string("GFX/map/Room2elevator_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2elevator::setBase(rme);
	/*Room2doors*/rme = loadRMesh(std::string("GFX/map/Room2doors_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2doors::setBase(rme);
	/*Room2scps*/rme = loadRMesh(std::string("GFX/map/Room2scps_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2scps::setBase(rme);
	/*Room3storage*/rme = loadRMesh(std::string("GFX/map/Room3storage_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room3storage::setBase(rme);
	/*Room2testRoom2*/rme = loadRMesh(std::string("GFX/map/Room2testRoom2_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2testRoom2::setBase(rme);
	/*Room3*/rme = loadRMesh(std::string("GFX/map/Room3_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room3::setBase(rme);
	/*Room3_2*/rme = loadRMesh(std::string("GFX/map/Room3_2_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room3_2::setBase(rme);
	/*Room4*/rme = loadRMesh(std::string("GFX/map/Room4_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room4::setBase(rme);
	/*Roompj*/rme = loadRMesh(std::string("GFX/map/Roompj_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Roompj::setBase(rme);
	/*Room914*/rme = loadRMesh(std::string("GFX/map/machineRoom_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room914::setBase(rme);
#endif
#if 0
	//HCZ
	/*Room008*/rme = loadRMesh(std::string("GFX/map/008_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room008::setBase(rme);
	/*RoomCoffin*/rme = loadRMesh(std::string("GFX/map/coffin_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); RoomCoffin::setBase(rme);
	/*RoomEndroom2*/rme = loadRMesh(std::string("GFX/map/endroom2_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); RoomEndroom2::setBase(rme);
	/*RoomTestroom*/rme = loadRMesh(std::string("GFX/map/testRoom_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); RoomTestroom::setBase(rme);
	/*RoomTunnel*/rme = loadRMesh(std::string("GFX/map/tunnel_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); RoomTunnel::setBase(rme);
	/*RoomTunnel2*/rme = loadRMesh(std::string("GFX/map/tunnel2_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); RoomTunnel2::setBase(rme);
	/*Room035*/rme = loadRMesh(std::string("GFX/map/Room035_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room035::setBase(rme);
	/*Room049*/rme = loadRMesh(std::string("GFX/map/Room049_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room049::setBase(rme);
	/*Room106*/rme = loadRMesh(std::string("GFX/map/Room106_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room106::setBase(rme);
	/*Room2ctunnel*/rme = loadRMesh(std::string("GFX/map/Room2Ctunnel_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2ctunnel::setBase(rme);
	/*Room2nuke*/rme = loadRMesh(std::string("GFX/map/Room2nuke_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2nuke::setBase(rme);
	/*Room2pipes*/rme = loadRMesh(std::string("GFX/map/Room2pipes_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2pipes::setBase(rme);
	/*Room2pit*/rme = loadRMesh(std::string("GFX/map/Room2pit_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2pit::setBase(rme);
	/*Room3pit*/rme = loadRMesh(std::string("GFX/map/Room3pit_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room3pit::setBase(rme);
	/*Room2servers*/rme = loadRMesh(std::string("GFX/map/Room2servers_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2servers::setBase(rme);
	/*Room2tunnel*/rme = loadRMesh(std::string("GFX/map/Room2tunnel_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2tunnel::setBase(rme);
	/*Room3tunnel*/rme = loadRMesh(std::string("GFX/map/Room3tunnel_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room3tunnel::setBase(rme);
	/*Room4tunnels*/rme = loadRMesh(std::string("GFX/map/4tunnels_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room4tunnels::setBase(rme);
	/*Room513*/rme = loadRMesh(std::string("GFX/map/Room513_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room513::setBase(rme);
#endif
#if 0
	//EZ
	/*Room860*/rme = loadRMesh(std::string("GFX/map/Room860_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room860::setBase(rme);
	/*RoomExit1*/rme = loadRMesh(std::string("GFX/map/exit1_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); RoomExit1::setBase(rme);
	/*RoomGateaentrance*/rme = loadRMesh(std::string("GFX/map/gateaentrance_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); RoomGateaentrance::setBase(rme);
	/*RoomLockroom2*/rme = loadRMesh(std::string("GFX/map/lockroom2_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); RoomLockroom2::setBase(rme);

	/*Room2z3*/rme = loadRMesh(std::string("GFX/map/Room2z3_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2z3::setBase(rme);
	/*Room2cafeteria*/rme = loadRMesh(std::string("GFX/map/Room2cafeteria_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2cafeteria::setBase(rme);
	/*Room2cz3*/rme = loadRMesh(std::string("GFX/map/Room2Cz3_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2cz3::setBase(rme);
	/*Room2ccont*/rme = loadRMesh(std::string("GFX/map/Room2ccont_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2ccont::setBase(rme);
	/*Room2offices*/rme = loadRMesh(std::string("GFX/map/Room2offices_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2offices::setBase(rme);
	/*Room2offices2*/rme = loadRMesh(std::string("GFX/map/Room2offices2_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2offices2::setBase(rme);
	/*Room2offices3*/rme = loadRMesh(std::string("GFX/map/Room2offices3_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2offices3::setBase(rme);
	/*Room2poffices*/rme = loadRMesh(std::string("GFX/map/Room2poffices_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2poffices::setBase(rme);
	/*Room2poffices2*/rme = loadRMesh(std::string("GFX/map/Room2poffices2_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2poffices2::setBase(rme);
	/*Room2sroom*/rme = loadRMesh(std::string("GFX/map/Room2sroom_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2sroom::setBase(rme);
	/*Room2toilets*/rme = loadRMesh(std::string("GFX/map/Room2toilets_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2toilets::setBase(rme);
	/*Room2tesla*/rme = loadRMesh(std::string("GFX/map/Room2tesla_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room2tesla::setBase(rme);
	/*Room3servers*/rme = loadRMesh(std::string("GFX/map/Room3servers_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room3servers::setBase(rme);
	/*Room3servers2*/rme = loadRMesh(std::string("GFX/map/Room3servers2_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room3servers2::setBase(rme);
	/*Room3z3*/rme = loadRMesh(std::string("GFX/map/Room3z3_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room3z3::setBase(rme);
	/*Room4z3*/rme = loadRMesh(std::string("GFX/map/Room4z3_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room4z3::setBase(rme);
#endif
#if 1
	//Misc
	/*Room173*/rme = loadRMesh(std::string("GFX/map/173bright_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room173::setBase(rme);
	/*RoomCheckpoint1*/rme = loadRMesh(std::string("GFX/map/checkpoint1_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); RoomCheckpoint1::setBase(rme);
	/*RoomCheckpoint2*/rme = loadRMesh(std::string("GFX/map/checkpoint2_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); RoomCheckpoint2::setBase(rme);
	/*RoomGatea*/rme = loadRMesh(std::string("GFX/map/gatea_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); RoomGatea::setBase(rme);
	/*RoomPocketdimension*/rme = loadRMesh(std::string("GFX/map/pocketdimension1_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); RoomPocketdimension::setBase(rme);
#endif
    /*Room079*/rme = loadRMesh(std::string("GFX/map/room079_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); Room079::setBase(rme);
	/*RoomLockroom*/rme = loadRMesh(std::string("GFX/map/lockroom_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); RoomLockroom::setBase(rme);
	/*RoomEndroom*/rme = loadRMesh(std::string("GFX/map/endroom_opt.rm2"),irrFileSystem,irrSmgr,roomTextures,roomShaders); RoomEndroom::setBase(rme);

    NPC096::baseNode = irrSmgr->addAnimatedMeshSceneNode(irrSmgr->getMesh("GFX/NPCs/scp096.b3d"));
    setupForHWSkinning(NPC096::baseNode->getMesh());

    NPC096::baseNode->getMaterial(0).setTexture(1,irrDriver->getTexture("GFX/NPCs/normal_flat.png"));
    NPC096::baseNode->getMaterial(0).setTexture(2,irrDriver->getTexture("GFX/NPCs/SCP096_specular.png"));

    NPC096::baseNode->getMaterial(1).setTexture(1,irrDriver->getTexture("GFX/NPCs/normal_flat.png"));
    NPC096::baseNode->getMaterial(1).setTexture(2,irrDriver->getTexture("GFX/NPCs/SCP096_specular.png"));

    setupForNormalsLighting(NPC096::baseNode);

    NPC096::baseNode->getMaterial(2).MaterialType = plainLightShader;
    NPC096::baseNode->getMaterial(2).setTexture(1,fogTexture.RenderTexture);
    NPC096::baseNode->getMaterial(2).setTexture(2,renderedLights[0].RenderTexture);
    NPC096::baseNode->getMaterial(2).setTexture(3,renderedLights[1].RenderTexture);
    NPC096::baseNode->getMaterial(2).setTexture(4,nullptr);
    NPC096::baseNode->getMaterial(2).setTexture(5,nullptr);
    NPC096::baseNode->setScale(irr::core::vector3df(4.f*RoomScale,4.f*RoomScale,4.f*RoomScale));
    NPC096::baseNode->setAnimationSpeed(0.f); //animation is handled by the npc, not Irrlicht

    NPC106::baseNode = irrSmgr->addAnimatedMeshSceneNode(irrSmgr->getMesh("GFX/NPCs/106_2.b3d"));
    setupForHWSkinning(NPC106::baseNode->getMesh());

    NPC106::baseNode->getMaterial(0).setTexture(1,irrDriver->getTexture("GFX/NPCs/106_normals.png"));
    NPC106::baseNode->getMaterial(0).setTexture(2,irrDriver->getTexture("GFX/NPCs/106_specular.png"));

    setupForNormalsLighting(NPC106::baseNode);

    NPC106::baseNode->setScale(irr::core::vector3df(2.8f*RoomScale,2.8f*RoomScale,2.8f*RoomScale));
    NPC106::baseNode->setAnimationSpeed(0.f); //animation is handled by the npc, not Irrlicht

    NPC513::baseNode = irrSmgr->addAnimatedMeshSceneNode(irrSmgr->getMesh("GFX/NPCs/bll.b3d"));
    setupForHWSkinning(NPC513::baseNode->getMesh());
    setupForPlainLighting(NPC513::baseNode);

    NPC513::baseNode->setScale(irr::core::vector3df(1.8f*RoomScale,1.8f*RoomScale,1.8f*RoomScale));
    NPC513::baseNode->setAnimationSpeed(0.f); //animation is handled by the npc, not Irrlicht

    NPC178::baseNode = irrSmgr->addAnimatedMeshSceneNode(irrSmgr->getMesh("GFX/NPCs/npc178.b3d"));
    setupForHWSkinning(NPC178::baseNode->getMesh());
    setupForNormalsLighting(NPC178::baseNode);
    NPC178::baseNode->setScale(irr::core::vector3df(0.45f*RoomScale,0.45f*RoomScale,0.45f*RoomScale));
    NPC178::baseNode->setAnimationSpeed(0.f); //animation is handled by the npc, not Irrlicht
    NPC178::baseNode->getMaterial(0).setTexture(1, irrDriver->getTexture("GFX/NPCs/178_normal.png"));
    NPC178::baseNode->getMaterial(0).setTexture(2, irrDriver->getTexture("GFX/NPCs/178_specular.jpg"));

    NPC173::baseNode = irrSmgr->addMeshSceneNode(irrSmgr->getMesh("GFX/NPCs/173_2.b3d"));

    NPC173::baseNode->setScale(irr::core::vector3df(1.3*RoomScale));

    NPC173::baseOcclusionNode = irrSmgr->addCubeSceneNode(10.0f,nullptr,-1,irr::core::vector3df(0,0,0),irr::core::vector3df(0,0,0),irr::core::vector3df(0.7f*RoomScale, 2.75f*RoomScale, 0.7f*RoomScale));
    NPC173::baseOcclusionNode->getMaterial(0).MaterialType = irr::video::EMT_TRANSPARENT_ADD_COLOR;

    setupForNormalsLighting(NPC173::baseNode,true);

    NPC173::baseNode->getMaterial(0).setTexture(1, irrDriver->getTexture("GFX/NPCs/173_norm.jpg"));
    NPC173::baseNode->getMaterial(0).setTexture(2, irrDriver->getTexture("GFX/NPCs/173_Spec.jpg"));

	GameObject::mainPlayer = new Player(this,irrSmgr,dynamics,irrReceiver);

	GameObject::mainPlayer->testNode = NPC173::baseNode;

    Door::baseDoorNode[0] = irrSmgr->addMeshSceneNode(irrSmgr->getMesh("GFX/map/Door01.x"));
    Door::baseDoorNode[0]->setScale(irr::core::vector3df(RoomScale*0.1f,RoomScale*0.1f,RoomScale*0.1f));
    setupForPlainLighting(Door::baseDoorNode[0]);

    Door::baseFrameNode = irrSmgr->addMeshSceneNode(irrSmgr->getMesh("GFX/map/DoorFrame.x"));
    Door::baseFrameNode->setScale(irr::core::vector3df(RoomScale*0.1f,RoomScale*0.1f,RoomScale*0.1f));
    setupForPlainLighting(Door::baseFrameNode);

    Door::baseButtonNode[0] = irrSmgr->addMeshSceneNode(irrSmgr->getMesh("GFX/map/Button.x"));
    Door::baseButtonNode[0]->setScale(irr::core::vector3df(RoomScale*1.f,RoomScale*1.f,RoomScale*1.f));
    setupForPlainLighting(Door::baseButtonNode[0]);

    Door::baseButtonNode[1] = irrSmgr->addMeshSceneNode(irrSmgr->getMesh("GFX/map/ButtonKeycard.x"));
    Door::baseButtonNode[1]->setScale(irr::core::vector3df(RoomScale*1.f,RoomScale*1.f,RoomScale*1.f));
    setupForPlainLighting(Door::baseButtonNode[1]);

    Door::openSound[0][0] = Sound::getSound(std::string("SFX/Doors/DoorOpen1.ogg"),true,1);
    Door::openSound[0][1] = Sound::getSound(std::string("SFX/Doors/DoorOpen2.ogg"),true,1);
    Door::openSound[0][2] = Sound::getSound(std::string("SFX/Doors/DoorOpen3.ogg"),true,1);
    Door::closeSound[0][0] = Sound::getSound(std::string("SFX/Doors/DoorClose1.ogg"),true,1);
    Door::closeSound[0][1] = Sound::getSound(std::string("SFX/Doors/DoorClose2.ogg"),true,1);
    Door::closeSound[0][2] = Sound::getSound(std::string("SFX/Doors/DoorClose3.ogg"),true,1);

	createMap(0);

	for (int i=0;i<50;i++) {
        fogBillboards[i]=irrSmgr->addBillboardSceneNode(nullptr,irr::core::dimension2df(500.f*0.1f*RoomScale,500.f*0.1f*RoomScale));
        fogBillboards[i]->setMaterialTexture(0,fogBillTex);
        fogBillboards[i]->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        fogBillboards[i]->setMaterialType(fogBillboardShader);
        fogBillboards[i]->getMaterial(0).ZBuffer = 0;
        fogBillboards[i]->setPosition(mainPlayer->getPosition()+irr::core::vector3df((rand()%1024-512)*0.1f*RoomScale,-(rand()%100)*0.1f*RoomScale,(rand()%1024-512)*0.1f*RoomScale));
        fogBillTargetPos[i]=fogBillboards[i]->getPosition();
        fogBillboards[i]->setVisible(false);
    }

	irrDevice->getCursorControl()->setVisible(false);
	irrDevice->getCursorControl()->setPosition((irr::s32)mainWidth/2,(irr::s32)mainHeight/2);

    irr::scene::IMesh* mesh1 = irrSmgr->getMesh("GFX/NPCs/scp-066.b3d");

    testNode = irrSmgr->addMeshSceneNode(mesh1);

    testNode->setScale(irr::core::vector3df(2.1*RoomScale));

    testNode->getMaterial(0).setTexture(1, irrDriver->getTexture("GFX/NPCs/scp-066_normal.png"));
    testNode->getMaterial(0).setTexture(2, irrDriver->getTexture("GFX/NPCs/scp-066_specular.png"));
    testNode->getMaterial(1).setTexture(1, irrDriver->getTexture("GFX/NPCs/scp-066_normal.png"));
    testNode->getMaterial(1).setTexture(2, irrDriver->getTexture("GFX/NPCs/scp-066_specular.png"));
    setupForNormalsLighting(testNode,true);
    testNode->setVisible(false);

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
	for (unsigned int i=0;i<itemList.size();i++) {
        itemList[i]->pick();
        itemList[i]->unpick(mainPlayer->getPosition());
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

	/*auto a = irr::LeakHunter::getReferenceCountedObjects();
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

	gContactAddedCallback = CustomMaterialCombinerCallback;*/
}

bool World::run() {
    time = irrTimer->getRealTime();
    irrReceiver->setTime(time);

	/*if (prevTime==0) { fpsFactor = 1.0; } else {
		fpsFactor = (time-prevTime)/(1000.0/70.0);
	}*/

	if (menusOpen==menus::NONE) {
        if (prevTime!=0) {
            stepsToMake += (time-prevTime)/(1000.f/60.f);
        }
        while (stepsToMake>0.f) {
            mainPlayer->selectPrevItem();
            irrSmgr->captureAll();
            drawHandIcon.clear();
            float prec = 1.f;
            for (unsigned int i=0;i<inventory_size;i++) {
                if (mainPlayer->takeFromInventory(i,false)!=nullptr) {
                    switch (mainPlayer->takeFromInventory(i,false)->getTempID()) {
                        case ItemTempIDs::ITEM_KEY1:
                        case ItemTempIDs::ITEM_KEY2:
                        case ItemTempIDs::ITEM_KEY3:
                        case ItemTempIDs::ITEM_KEY4:
                        case ItemTempIDs::ITEM_KEY5:
                        case ItemTempIDs::ITEM_KEY6:
                            mainPlayer->takeFromInventory(i,false)->setDrawCoords(irr::core::vector2di(mainWidth/2,mainHeight/2));
                        break;
                        default: break;
                    }
                }
            }

            //std::cout<<mainPlayer->getPosition().X<<"\n";
            mainPlayer->update();
            dynamics->simStep(1.f/60.f,1.f/60.f);
            mainPlayer->resetSpeeds();
            for (int i=0;i<npcList.size();i++) {
                npcList[i]->update();
                npcList[i]->updateModel();
            }
            if (irrReceiver->IsKeyDown(irr::KEY_KEY_1) && !irrReceiver->IsPrevKeyDown(irr::KEY_KEY_1)) {
                irr::core::vector3df pickPos = mainPlayer->camera->getTarget();
                NPC* newNPC = NPC513::createNPC513();
                newNPC->teleport(pickPos);
                npcList.push_back(newNPC);
            }

            if (irrReceiver->IsKeyDown(irr::KEY_KEY_2) && !irrReceiver->IsPrevKeyDown(irr::KEY_KEY_2)) {
                irr::core::vector3df pickPos = mainPlayer->camera->getTarget();
                NPC* newNPC = NPC178::createNPC178();
                newNPC->teleport(pickPos);
                npcList.push_back(newNPC);
            }

            if (irrReceiver->IsKeyDown(irr::KEY_KEY_3) && !irrReceiver->IsPrevKeyDown(irr::KEY_KEY_3)) {
                irr::scene::IMeshSceneNode* node = static_cast<irr::scene::IMeshSceneNode*>(testNode->clone());
                irr::core::vector3df pickPos = mainPlayer->camera->getTarget();
                node->setPosition(pickPos);
                node->setVisible(true);
                btRigidBody* rbody = dynamics->addTriMesh_moving(node,0.1f,5,1,1);
				rbody->setFriction(0.1f);
				rbody->setRollingFriction(0.1f);
				rbody->setDamping(0,0);
				rbody->setSleepingThresholds(0.1f,0.1f);
				btTransform oTrans = rbody->getCenterOfMassTransform();
                oTrans.setOrigin(btVector3(pickPos.X,pickPos.Y,pickPos.Z));
                rbody->setCenterOfMassTransform(oTrans);
                rbody->setLinearVelocity(irrToBtVec(mainPlayer->camera->getTarget()-mainPlayer->camera->getAbsolutePosition()));
            }

            irr::core::matrix4 tMat = irrDriver->getTransform(irr::video::ETS_PROJECTION)*irrDriver->getTransform(irr::video::ETS_VIEW);
            for (unsigned int i=0;i<itemList.size();i++) {
                if ((mainPlayer->getPosition()-itemList[i]->getPosition()).getLengthSQ()<500.f*RoomScale*RoomScale) {
                    irr::scene::SViewFrustum frust = *mainPlayer->getViewFrustum();
                    //transform the frustum to the node's current absolute transformation
                    irr::core::matrix4 invTrans(itemList[i]->getTransform(), irr::core::matrix4::EM4CONST_INVERSE);
                    //invTrans.makeInverse();
                    frust.transform(invTrans);
                    if (!itemList[i]->getPicked()) {
                        if ((mainPlayer->getPosition()-itemList[i]->getPosition()).getLengthSQ()<250.f*RoomScale*RoomScale) {
                            //irrDriver->draw2DImage(irrDriver->getTexture("GFX/handsymbol.jpg"),irr::core::position2di(mainWidth/2,mainHeight/2));
                            irr::core::vector3df pos = itemList[i]->getPosition();
                            tMat.transformVect(pos);
                            if (pos.Z>0.f) {
                                drawHandIcon.push_back(irr::core::vector2di(pos.X*mainWidth*0.066f*(150.f-pos.Z)/150.f+(mainWidth/2),-pos.Y*mainHeight*0.066f*(150.f-pos.Z)/150.f+(mainHeight/2)));
                            }
                            //std::cout<<"handicon_add\n";
                            if (mainPlayer->seesBoundingBox(itemList[i]->getBBox(),frust)) {
                                if (irrReceiver->IsMouseDown(0) != mainPlayer->getLastMouseDown(0) && irrReceiver->IsMouseDown(0)==false) {
                                    mainPlayer->addToInventory(itemList[i]);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            for (unsigned int i=0;i<doorList.size();i++) {
                if ((mainPlayer->getPosition()-doorList[i]->getPosition()).getLength()<4096.f*RoomScale*RoomScale) {
                    float rot0 = doorList[i]->getButtonRotation(0);
                    float rot1 = doorList[i]->getButtonRotation(1);
                    irr::core::matrix4 rotMat;
                    rotMat.makeIdentity();
                    rotMat.setRotationDegrees(irr::core::vector3df(0.f,rot0,0.f));
                    irr::core::vector3df buttonPointing0(0.f,0.f,-1.f);
                    rotMat.transformVect(buttonPointing0);
                    rotMat.makeIdentity();
                    rotMat.setRotationDegrees(irr::core::vector3df(0.f,rot1,0.f));
                    irr::core::vector3df buttonPointing1(0.f,0.f,-1.f);
                    rotMat.transformVect(buttonPointing1);

                    irr::core::vector3df pos0 = doorList[i]->getButtonPosition(0);
                    irr::core::vector3df pos1 = doorList[i]->getButtonPosition(1);
                    irr::core::vector3df camPos = irrSmgr->getActiveCamera()->getAbsolutePosition();
                    irr::core::vector3df dir0 = (camPos-pos0);
                    dir0.normalize();
                    irr::core::vector3df dir0NoY = dir0; dir0NoY.Y = 0.f; dir0NoY.normalize();
                    dir0 = dir0.getSphericalCoordinateAngles();
                    irr::core::vector3df dir1 = (camPos-pos1);
                    dir1.normalize();
                    irr::core::vector3df dir1NoY = dir1; dir1NoY.Y = 0.f; dir1NoY.normalize();
                    dir1 = dir1.getSphericalCoordinateAngles();
                    irr::core::vector3df camTarget = -(irrSmgr->getActiveCamera()->getTarget()-irrSmgr->getActiveCamera()->getAbsolutePosition()).normalize();
                    camTarget = camTarget.getSphericalCoordinateAngles();

                    //float angleDiff0 = dir0.Y-rot0;
                    //while (angleDiff0>=180.f) { angleDiff0-=360.f; } while (angleDiff0<-180.f) { angleDiff0+=360.f; }
                    //float angleDiff1 = dir1.Y-rot1;
                    //while (angleDiff1>=180.f) { angleDiff1-=360.f; } while (angleDiff1<-180.f) { angleDiff1+=360.f; }

                    float yawvalue,pitchvalue;
                    bool closetobutton = false;
                    if (dir0NoY.getDistanceFromSQ(buttonPointing0)<1.f) {
                        if ((mainPlayer->getPosition()-doorList[i]->getButtonPosition(0)).getLengthSQ()<150.f*RoomScale*RoomScale) {
                            if (doorList[i]->getButtonVisibility(0)==true) {
                                yawvalue = camTarget.Y-dir0.Y;
                                pitchvalue = camTarget.X-dir0.X;
                                closetobutton = true;
                                std::cout<<"closeTo0 "<<buttonPointing0.X<<" "<<buttonPointing0.Z<<" "<<buttonPointing1.X<<" "<<buttonPointing1.Z<<" "<<rot0<<" "<<rot1<<" "<<"\n";
                            }
                        }
                    }
                    if (dir1NoY.getDistanceFromSQ(buttonPointing1)<1.f) {
                        if ((mainPlayer->getPosition()-doorList[i]->getButtonPosition(1)).getLengthSQ()<150.f*RoomScale*RoomScale) {
                            if (doorList[i]->getButtonVisibility(1)==true) {
                                yawvalue = camTarget.Y-dir1.Y;
                                pitchvalue = camTarget.X-dir1.X;
                                closetobutton = true;
                                std::cout<<"closeTo1 "<<buttonPointing0.X<<" "<<buttonPointing0.Z<<" "<<buttonPointing1.X<<" "<<buttonPointing1.Z<<" "<<rot0<<" "<<rot1<<" "<<"\n";
                            }
                        }
                    }
                    if (closetobutton) {
                        while (yawvalue>=360) { yawvalue-=360.f; } while (yawvalue<0) { yawvalue+=360.f; }
                        if (yawvalue > 90 && yawvalue <= 180) { yawvalue = 90; }
                        if (yawvalue > 180 && yawvalue < 270) { yawvalue = 270; }
                        while (pitchvalue>=360) { pitchvalue-=360.f; } while (pitchvalue<0) { pitchvalue+=360.f; }
                        if (pitchvalue > 90 && pitchvalue <= 180) { pitchvalue = 90; }
                        if (pitchvalue > 180 && pitchvalue < 270) { pitchvalue = 270; }
                        irr::core::vector2di coords = irr::core::position2di(mainWidth/2 + std::sin(yawvalue * irr::core::DEGTORAD) * (mainWidth / 3),mainHeight/2 + std::sin(pitchvalue * irr::core::DEGTORAD) * (mainHeight / 3));


                        unsigned char keycard = 0;
                        unsigned char highestCard = 0;
                        unsigned char itemToSelect = inventory_size;

                        if (doorList[i]->getButtonIndex()) {
                            if (mainPlayer->getSelectedItem()>=inventory_size) {
                                for (unsigned int j=0;j<inventory_size;j++) {
                                    if (mainPlayer->takeFromInventory(j,false)!=nullptr) {
                                        switch (mainPlayer->takeFromInventory(j,false)->getTempID()) {
                                            case ItemTempIDs::ITEM_KEY1:
                                                if (highestCard<1) {
                                                    highestCard = 1;
                                                    itemToSelect = j;
                                                }
                                            break;
                                            case ItemTempIDs::ITEM_KEY2:
                                                if (highestCard<2) {
                                                    highestCard = 2;
                                                    itemToSelect = j;
                                                }
                                            break;
                                            case ItemTempIDs::ITEM_KEY3:
                                                if (highestCard<3) {
                                                    highestCard = 3;
                                                    itemToSelect = j;
                                                }
                                            break;
                                            case ItemTempIDs::ITEM_KEY4:
                                                if (highestCard<4) {
                                                    highestCard = 4;
                                                    itemToSelect = j;
                                                }
                                            break;
                                            case ItemTempIDs::ITEM_KEY5:
                                                if (highestCard<5) {
                                                    highestCard = 5;
                                                    itemToSelect = j;
                                                }
                                            break;
                                            case ItemTempIDs::ITEM_KEY6:
                                                if (highestCard<6) {
                                                    highestCard = 6;
                                                    itemToSelect = j;
                                                }
                                            break;
                                            default: break;
                                        }
                                    }
                                }
                                if (itemToSelect<inventory_size) {
                                    mainPlayer->selectItem(itemToSelect,true);
                                }
                            }

                            if (mainPlayer->getSelectedItem()<inventory_size) {
                                switch (mainPlayer->takeFromInventory(mainPlayer->getSelectedItem(),false)->getTempID()) {
                                    case ItemTempIDs::ITEM_KEY1:
                                        keycard = 1;
                                        mainPlayer->takeFromInventory(mainPlayer->getSelectedItem(),false)->setDrawCoords(coords);
                                    break;
                                    case ItemTempIDs::ITEM_KEY2:
                                        keycard = 2;
                                        mainPlayer->takeFromInventory(mainPlayer->getSelectedItem(),false)->setDrawCoords(coords);
                                    break;
                                    case ItemTempIDs::ITEM_KEY3:
                                        keycard = 3;
                                        mainPlayer->takeFromInventory(mainPlayer->getSelectedItem(),false)->setDrawCoords(coords);
                                    break;
                                    case ItemTempIDs::ITEM_KEY4:
                                        keycard = 4;
                                        mainPlayer->takeFromInventory(mainPlayer->getSelectedItem(),false)->setDrawCoords(coords);
                                    break;
                                    case ItemTempIDs::ITEM_KEY5:
                                        keycard = 5;
                                        mainPlayer->takeFromInventory(mainPlayer->getSelectedItem(),false)->setDrawCoords(coords);
                                    break;
                                    case ItemTempIDs::ITEM_KEY6:
                                        keycard = 6;
                                        mainPlayer->takeFromInventory(mainPlayer->getSelectedItem(),false)->setDrawCoords(coords);
                                    break;
                                    default:
                                        drawHandIcon.push_back(coords);
                                    break;
                                }
                            } else {
                                drawHandIcon.push_back(coords);
                            }
                        } else {
                            drawHandIcon.push_back(coords);
                        }

                        if (irrReceiver->IsPrevMouseDown(0)==true && irrReceiver->IsMouseDown(0)==false) {
                            doorList[i]->toggle(keycard,0,0);
                            mainPlayer->selectItem(inventory_size);
                        }
                    }
                    doorList[i]->update();
                } else {
                    doorList[i]->disable();
                }
            }
            /*if (irrReceiver->IsMouseDown(1) != mainPlayer->getLastMouseDown(1) && irrReceiver->IsMouseDown(1)==false) {
                for (unsigned int i=0;i<inventory_size;i++) {
                    if (mainPlayer->takeFromInventory(i)) break;
                }
            }*/

            mainPlayer->updateHead();

            int px,py;
            px = coordToRoomGrid(mainPlayer->getPosition().X);
            py = coordToRoomGrid(mainPlayer->getPosition().Z);
            if (roomArray[px][py]!=nullptr) {
                roomArray[px][py]->updateEvent();
            } else if (roomArray[ppx][ppy]!=nullptr) {
                roomArray[ppx][ppy]->updateEvent();
            }
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
                        int prioritize = nLights.size();

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
                        plainLightCallback->setLights(nLights,prioritize);
                        normalsCallback->setLights(nLights,prioritize);
                        ppx = px;
                        ppy = py;
                    }
                }
            }

            irr::core::position2di mousePos = irrDevice->getCursorControl()->getPosition();
            mouseDistFromCenter = mousePos-irr::core::position2di(mainWidth/2,mainHeight/2);
            if (mousePos != irr::core::position2di(mainWidth/2,mainHeight/2)) {
                float oldYaw = mainPlayer->yaw;
                mainPlayer->yaw += ((int)mousePos.X-(int)(mainWidth/2))*0.1f;
                mainPlayer->pitch += ((int)mousePos.Y-(int)(mainHeight/2))*0.1f;
                irrDevice->getCursorControl()->setPosition((irr::s32)mainWidth/2,(irr::s32)mainHeight/2);
            }

            if (irrReceiver->IsKeyDown(irr::KEY_TAB)==false && irrReceiver->IsPrevKeyDown(irr::KEY_TAB)==true) {
                menusOpen = menus::INVOPEN;
                break;
            }
            if (irrReceiver->IsPrevKeyDown(irr::KEY_ESCAPE)==true && irrReceiver->IsKeyDown(irr::KEY_ESCAPE)==false) {
                menusOpen = menus::PAUSEOPEN;
                break;
            }
            stepsToMake-=1.f;

            irrReceiver->perLoopUpdate();
            prevMenusOpen = menusOpen;
        }

		/*for (unsigned int i=0;i<doorList.size();++i) {
            doorList[i]->update();
		}*/
	} else {
		//dynamics->resetTimer(time);
	}

    irrDriver->beginScene(true, true, irr::video::SColor(255, 255, 0, 255));

	draw3D();

	//mainPlayer->booststamina(10.f,-1.f);

	drawHUD();

	irrDriver->setRenderTarget(nullptr,false,false);
	screenQuad->setMaterialTexture(0,finalImage.RenderTexture);
	screenQuad->setMaterialType(gammaShader);
	screenQuad->render();
	screenQuad->setMaterialTexture(0,blurImage.RenderTexture);
	screenQuad->setMaterialType(postProcShader);
    //irrDriver->draw2DImage(finalImage.RenderTexture,irr::core::vector2di(0,0));

	if (menusOpen!=menus::NONE) {
        irrReceiver->perLoopUpdate();
        prevMenusOpen = menusOpen;
	}

    irrDriver->endScene();

    //time = irrTimer->getRealTime();
	/*while (time-prevTime<1000/60) {
        irrDevice->sleep((1000/60)-(time-prevTime));
        time = irrTimer->getRealTime();
    }*/
    prevTime = time;

    //hudMsg = std::to_string(irrDriver->getFPS());
    //hudMsgTimer = 1000.f;

	Sound::processDrops();

	//drawHandIcon.clear();

    return irrDevice->run();
}

void World::draw3D() {
	//postProcCallback->fpsFactor = fpsFactor;

    SharedShaderCallBack::cameraPos = irrSmgr->getActiveCamera()->interpolatedAbsoluteTransform(1.f+stepsToMake).getTranslation();

    drawFog();

    roomCallback->reflectFactor=0.f;
    irrDriver->setRenderTarget(reflection.RenderTexture,false,true);
    /*mainPlayer->reflectNY();
    irrSmgr->fastDrawAll_init(irrSmgr->getActiveCamera()->getAbsolutePosition());
    irrSmgr->fastDrawAll(irrSmgr->getActiveCamera()->getProjectionMatrix(),irrSmgr->getActiveCamera()->getViewMatrix());
    irrSmgr->fastDrawAll_end();*/
    mainPlayer->resetCam();
    roomCallback->reflectFactor=1.f;
	/*irrDriver->setRenderTarget(blurImage2); //copy the old render
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
    irrDriver->setRenderTarget(deferredLightData,false,true,irr::video::SColor(255,0,0,0));
    //irrDriver->draw2DImage(fogTexture.RenderTexture,irr::core::vector2di(0,0));
    plainLightCallback->renderSpecularFactor = 1.f;
    irrSmgr->drawAll(1.f+stepsToMake,1);
    /*irr::core::list<irr::scene::ISceneNode*> smgrChildren = irrSmgr->getRootSceneNode()->getChildren();
    for (auto it = smgrChildren.begin(); it != smgrChildren.end(); it++) {
        irr::scene::ISceneNode* node = *it;
        if (node->getType()==irr::scene::ESNT_MESH) {
            irr::scene::IMeshSceneNode* mnode = static_cast<irr::scene::IMeshSceneNode*>(node);
            for (unsigned int i=0;i<mnode->getMaterialCount();i++) {
                if (mnode->getMaterial(i).MaterialType==plainLightShader) {
                    //std::cout<<"PLAIN OL' LIGHT\n";
                }
            }
        }
    }*/
    irrDriver->setRenderTarget(renderedLights,true,true,irr::video::SColor(255,0,0,0));
    //deferredLightSphere->setPosition(irr::core::vector3df(ppx*204.8f*RoomScale,0,ppy*204.8f*RoomScale));
    deferredLightSphere->setVisible(true);
    //LightsShaderCallBack::sortLights(irr::core::vector3df(ppx*204.8f*RoomScale,0,ppy*204.8f*RoomScale));
    for (unsigned int i=0;i<LightsShaderCallBack::lightList.size();i++) {
        renderDeferredLightCallback->lightToRender = i;
        float lightScale = std::sqrt(LightsShaderCallBack::lightList[i].pos[3]);
        deferredLightSphere->setScale(irr::core::vector3df(lightScale,lightScale,lightScale));
        irr::core::vector3df lightPos;
        lightPos.X = LightsShaderCallBack::lightList[i].pos[0];
        lightPos.Y = LightsShaderCallBack::lightList[i].pos[1];
        lightPos.Z = LightsShaderCallBack::lightList[i].pos[2];
        deferredLightSphere->setPosition(lightPos);
        deferredLightSphere->updateAbsolutePosition();
        deferredLightSphere->render();
    }
    deferredLightSphere->setVisible(false);

    irrDriver->setRenderTarget(blurImage.RenderTexture,false,true);
    irrDriver->draw2DImage(fogTexture.RenderTexture,irr::core::vector2di(0,0));
    plainLightCallback->renderSpecularFactor = 0.f;
    irrSmgr->drawAll(1.f+stepsToMake,0);

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
		if (itemSelected==true) {
            postProcCallback->minBlur=2.f;
        } else {
            postProcCallback->minBlur=0.f;
        }
		if (menusOpen!=menus::NONE) postProcCallback->minBlur = 2.f;
    }

    irrDriver->setRenderTarget(zBuffer.RenderTexture,true,true,irr::video::SColor(255,255,0,0));

    irrDriver->getOverrideMaterial().EnableFlags = irr::video::EMF_MATERIAL_TYPE;
    irrDriver->getOverrideMaterial().EnablePasses = irr::scene::ESNRP_SOLID;
    irrDriver->getOverrideMaterial().Material.MaterialType = (irr::video::E_MATERIAL_TYPE)zBufferShader;
    irrSmgr->drawAll(1.f+stepsToMake,0);
    irrDriver->getOverrideMaterial().EnablePasses = 0;

    /*irrDriver->runAllOcclusionQueries(true); //OCCLUSION QUERIES ARE REALLY SLOW
    irrDriver->updateAllOcclusionQueries();*/

    irrDriver->setRenderTarget(finalImage.RenderTexture,false,true);
    screenQuad->render();
    irrDriver->draw2DImage(blurImage2.RenderTexture,irr::core::vector2di(0,0),irr::core::recti(0,0,mainWidth,mainHeight),nullptr,irr::video::SColor(10,255,255,255));
    irrDriver->setRenderTarget(blurImage2.RenderTexture,false,false);
    irrDriver->draw2DImage(finalImage.RenderTexture,irr::core::vector2di(0,0));
    irrDriver->setRenderTarget(finalImage.RenderTexture,false,false);
}

void World::drawHUD() {

    itemSelected = mainPlayer->drawSelectedItem();

    if (menusOpen==menus::INVOPEN) {
        mainPlayer->selectItem(inventory_size);
		if (prevMenusOpen!=menusOpen) {
			Sound::freezeCategory(1);
			irrReceiver->perLoopUpdate();
			for (unsigned char i=0;i<inventory_size;i++) {
                if (mainPlayer->takeFromInventory(i,false)!=nullptr) {
                    std::string imgpath = mainPlayer->takeFromInventory(i,false)->getInvImgPath();
                    if (imgpath!="") {
                        invImgs[i]=irrDriver->getTexture(imgpath.c_str());
                    }
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
					font1->draw(mainPlayer->takeFromInventory(i,false)->getInvName().c_str(),irr::core::recti(irr::core::position2di(x+2,y+(h+8)*scale2Db),irr::core::position2di(x+(w+2)*scale2Db,y+(h+24)*scale2Db)),irr::video::SColor(100,0,0,0),true,true);
					font1->draw(mainPlayer->takeFromInventory(i,false)->getInvName().c_str(),irr::core::recti(irr::core::position2di(x,y+(h+6)*scale2Db),irr::core::position2di(x+w*scale2Db,y+(h+22)*scale2Db)),irr::video::SColor(255,255,255,255),true,true);
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
		if (irrReceiver->IsKeyDown(irr::KEY_TAB)==false && irrReceiver->IsPrevKeyDown(irr::KEY_TAB)==true) { menusOpen = menus::NONE; }
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
			irrReceiver->perLoopUpdate();
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

			if (button(std::string("Resume"),mainWidth/2-180*scale2D,mainHeight/2-20*scale2D,455*scale2D,70*scale2D)) { menusOpen = menus::NONE; }
			if (button(std::string("Options"),mainWidth/2-180*scale2D,mainHeight/2+60*scale2D,455*scale2D,70*scale2D)) { menusOpen = menus::OPTIONSOPEN; }
			if (button(std::string("Quit"),mainWidth/2-180*scale2D,mainHeight/2+140*scale2D,455*scale2D,70*scale2D)) { irrDevice->closeDevice(); }//TODO: add main menu

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
					ss << 1.0/gammaCallback->invGamma;
					s = ss.str();

					font1->draw(s.c_str(),irr::core::recti(mainWidth/2-150*scale2D,mainHeight/2-125*scale2D,mainWidth/2+100*scale2D,mainHeight/2-110*scale2D),irr::video::SColor(255,255,255,255),false,false);

					outline = irr::core::recti(mainWidth/2-150*scale2D,mainHeight/2-100*scale2D,mainWidth/2+260*scale2D,mainHeight/2-86*scale2D);
					irrDriver->draw2DRectangleOutline(irr::core::recti(outline.UpperLeftCorner.X-3*scale2D,outline.UpperLeftCorner.Y-3*scale2D,outline.LowerRightCorner.X+3*scale2D,outline.LowerRightCorner.Y+3*scale2D));

					if (irrReceiver->IsMouseDown(0)) {
						if (irrReceiver->getMousePos().X>outline.UpperLeftCorner.X && irrReceiver->getMousePos().Y>outline.UpperLeftCorner.Y &&
							irrReceiver->getMousePos().X<outline.LowerRightCorner.X && irrReceiver->getMousePos().Y<outline.LowerRightCorner.Y) {
							gammaCallback->invGamma = ((irrReceiver->getMousePos().X-outline.UpperLeftCorner.X)/(float)(outline.LowerRightCorner.X-outline.UpperLeftCorner.X))*1.5f+0.5f;
							gammaCallback->invGamma = ((int)(gammaCallback->invGamma*100.f))*0.01f;
							gammaCallback->invGamma = 1.f/gammaCallback->invGamma;
						}
					}

					irrDriver->draw2DImage(blinkMeterIMG,irr::core::recti(
						mainWidth/2-150*scale2D+((1.f/gammaCallback->invGamma)-0.5f)/1.5f*406*scale2D,
						mainHeight/2-100*scale2D,
						mainWidth/2-142*scale2D+((1.f/gammaCallback->invGamma)-0.5f)/1.5f*406*scale2D,mainHeight/2-86*scale2D),
						irr::core::recti(0,0,8,14));

					irrDriver->draw2DRectangle(irr::video::SColor(255,0,0,0),irr::core::recti(mainWidth/2-150*scale2D,mainHeight/2-65*scale2D,mainWidth/2+250*scale2D,mainHeight/2+100*scale2D));

					color = 7;
					irrDriver->draw2DRectangle(irr::video::SColor(255,color,color,color),irr::core::recti(mainWidth/2-100*scale2D,mainHeight/2-20*scale2D,mainWidth/2-32*scale2D,mainHeight/2+48*scale2D));
					color = 4;
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
			irrReceiver->perLoopUpdate();
		}
	} else {
        if (drawHandIcon.size()>0) {
            for (unsigned int i=0;i<drawHandIcon.size();i++) {
                irrDriver->draw2DImage(irrDriver->getTexture("GFX/handsymbol.png"),drawHandIcon[i]-irr::core::vector2di(16,16));
                //std::cout<<"handicon_draw\n";
            }
        }
	}

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
}

void World::drawFog() {
    irrDriver->setRenderTarget(fogTexture.RenderTexture,true,false);

    //draw a triangle with a different material to make alpha blending for the billboards work properly
    irr::video::SMaterial mat;
    mat.MaterialType = vertLightShader;
    mat.FrontfaceCulling = true;
    irrDriver->setMaterial(mat);
    irr::core::vector3df vert1(-1000.f,0.f,1.f);
    irr::core::vector3df vert2(-1000.f,-1.f,0.f);
    irr::core::vector3df vert3(-1000.f,-1.f,1.f);
    irrDriver->draw3DTriangle(irr::core::triangle3df(vert1,vert2,vert3),irr::video::SColor(255,0,0,0));

    for (int i=0;i<50;i++) {
        irr::core::matrix4 rotMatrix;
        irr::core::vector3df dist2d = fogBillTargetPos[i]-mainPlayer->getPosition();
        dist2d.Y = 0.f;
        float dist = dist2d.getLength();
        if (dist<200.f*0.1f*RoomScale) {
            fogBillTargetPos[i]=(fogBillTargetPos[i]+(dist2d.normalize()*0.01f));
        } else if (dist>1000.f*0.1f*RoomScale) {
            irr::core::vector3df diff2d = (mainPlayer->getPosition()-fogBillTargetPos[i]);
            diff2d.Y = 0.f;
            fogBillTargetPos[i]+=diff2d.normalize()*(dist+(900.f*0.1f*RoomScale));
            fogBillTargetPos[i].Y = mainPlayer->getPosition().Y-(rand()%20)+10.f;
            fogBillboards[i]->setPosition(fogBillTargetPos[i]);
            std::cout<<"fogbillboard teleport\n";
        } else {
            rotMatrix.setRotationDegrees(irr::core::vector3df((rand()%100-50)/25.f,(rand()%100-50)/12.f,(rand()%100-50)/150.f)*0.2f);
            fogBillTargetPos[i]+=irr::core::vector3df(rand()%3-1,rand()%3-1,rand()%3-1)*0.1f;
            irr::core::vector3df diff = fogBillTargetPos[i]-mainPlayer->getPosition();
            rotMatrix.transformVect(diff);
            fogBillTargetPos[i]=mainPlayer->getPosition()+diff;
        }

        if ((fogBillTargetPos[i].Y-mainPlayer->getPosition().Y)>25.f) {
            fogBillTargetPos[i].Y-=40.f;
        }
        if ((mainPlayer->getPosition().Y-fogBillTargetPos[i].Y)>25.f) {
            fogBillTargetPos[i].Y+=40.f;
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

        fogBillboards[i]->setVisible(true);
        fogBillboards[i]->render();
        fogBillboards[i]->setVisible(false);
    }
}

void World::shadersSetup() {

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
    //normalsCallback->ambient = irr::video::SColor(255,0,0,0);

	plainLightShader = irr::video::EMT_SOLID; // Fallback material type
    plainLightCallback= new PlainLightShaderCallBack;
    plainLightShader = (irr::video::E_MATERIAL_TYPE)irrGpu->addHighLevelShaderMaterialFromFiles("GFX/shaders/LightingVert.vert", "main", irr::video::EVST_VS_1_1,"GFX/shaders/LightingFrag.frag", "main", irr::video::EPST_PS_1_1,plainLightCallback, irr::video::EMT_SOLID);
    //plainLightCallback->ambient = irr::video::SColor(255,0,0,0);

    renderDeferredLightShader = irr::video::EMT_TRANSPARENT_ADD_COLOR; // Fallback material type
    renderDeferredLightCallback= new RenderDeferredLightShaderCallBack;
    renderDeferredLightShader = (irr::video::E_MATERIAL_TYPE)irrGpu->addHighLevelShaderMaterialFromFiles("GFX/shaders/DeferredLightQuadVert.vert", "main", irr::video::EVST_VS_1_1,"GFX/shaders/DeferredLightQuadFrag.frag", "main", irr::video::EPST_PS_1_1,renderDeferredLightCallback, irr::video::EMT_TRANSPARENT_ADD_COLOR);

	postProcShader = irr::video::EMT_SOLID; // Fallback material type
    postProcCallback= new PostProcShaderCallBack;
    postProcShader = (irr::video::E_MATERIAL_TYPE)irrGpu->addHighLevelShaderMaterialFromFiles("GFX/shaders/PostProcessVert.vert", "main", irr::video::EVST_VS_1_1,"GFX/shaders/PostProcessFrag.frag", "main", irr::video::EPST_PS_1_1,postProcCallback, irr::video::EMT_SOLID);

    gammaShader = irr::video::EMT_SOLID; // Fallback material type
    gammaCallback= new GammaShaderCallBack;
    gammaShader = (irr::video::E_MATERIAL_TYPE)irrGpu->addHighLevelShaderMaterialFromFiles("GFX/shaders/PostProcessVert.vert", "main", irr::video::EVST_VS_1_1,"GFX/shaders/GammaFragShader.frag", "main", irr::video::EPST_PS_1_1,gammaCallback, irr::video::EMT_SOLID);
    gammaCallback->invGamma = 1.f;

	zBufferShader = irr::video::EMT_SOLID; // Fallback material type
    zBufferCallback= new ZBufferShaderCallBack;
    zBufferShader = (irr::video::E_MATERIAL_TYPE)irrGpu->addHighLevelShaderMaterialFromFiles("GFX/shaders/ZBufferVert.vert", "main", irr::video::EVST_VS_1_1,"GFX/shaders/ZBufferFrag.frag", "main", irr::video::EPST_PS_1_1,zBufferCallback, irr::video::EMT_SOLID);

    // create new vertex format.

    irr::video::IVertexDescriptor* vertexDescriptor = irrDriver->addVertexDescriptor("Skinning");

    SharedShaderCallBack::fogNear = -1.f;
    SharedShaderCallBack::fogFar = 30.f;

    vertexDescriptor->addAttribute("inPosition", 3, irr::video::EVAS_POSITION, irr::video::EVAT_FLOAT, 0);
    vertexDescriptor->addAttribute("inNormal", 3, irr::video::EVAS_NORMAL, irr::video::EVAT_FLOAT, 0);
    vertexDescriptor->addAttribute("inTangent", 3, irr::video::EVAS_TANGENT, irr::video::EVAT_FLOAT, 0);
    vertexDescriptor->addAttribute("inBinormal", 3, irr::video::EVAS_BINORMAL, irr::video::EVAT_FLOAT, 0);
    vertexDescriptor->addAttribute("inTexCoord0", 2, irr::video::EVAS_TEXCOORD0, irr::video::EVAT_FLOAT, 0);
    vertexDescriptor->addAttribute("inBlendWeight", 4, irr::video::EVAS_BLEND_WEIGHTS, irr::video::EVAT_FLOAT, 0);
    vertexDescriptor->addAttribute("inBlendIndex", 4, irr::video::EVAS_BLEND_INDICES, irr::video::EVAT_FLOAT, 0);
}

void World::setupForHWSkinning(irr::scene::IAnimatedMesh* mesh) {
    irr::scene::ISkinnedMesh* skinMesh = static_cast<irr::scene::ISkinnedMesh*>(mesh);
    // convert vertices.

    for (irr::u32 i = 0; i < skinMesh->getMeshBufferCount(); ++i) {
        irrSmgr->getMeshManipulator()->convertVertices<SSkinningVertex>(skinMesh->getMeshBuffer(i), irrDriver->getVertexDescriptor("Skinning"), false);
    }
    irrSmgr->getMeshManipulator()->recalculateTangents(mesh,true,true,true);

    skinMesh->setHardwareSkinning(true);

    // fill blending data.

    for (irr::u32 i = 0; i < skinMesh->getJointCount(); ++i) {
        for (irr::u32 j = 0; j < skinMesh->getAllJoints()[i]->Weights.size(); ++j) {
            SSkinningVertex* Vertices = static_cast<SSkinningVertex*>(mesh->getMeshBuffer(skinMesh->getAllJoints()[i]->Weights[j].buffer_id)->getVertexBuffer(0)->getVertices());

            irr::s32 id = -1;

            for (irr::s32 k = 0; k < 4; ++k) {
                if (Vertices[skinMesh->getAllJoints()[i]->Weights[j].vertex_id].BlendWeight[k] == 0.f) {
                    id = k;
                    break;
                }
            }

            if (id >= 0) {
                Vertices[skinMesh->getAllJoints()[i]->Weights[j].vertex_id].BlendWeight[id] = skinMesh->getAllJoints()[i]->Weights[j].strength;
                Vertices[skinMesh->getAllJoints()[i]->Weights[j].vertex_id].BlendIndex[id] = float(i);
            }
        }
    }
}

void World::setupForPlainLighting(irr::scene::ISceneNode* node) {
    for (unsigned int i=0;i<node->getMaterialCount();i++) {
        irr::video::SMaterial& material = node->getMaterial(i);
        material.Filter = 1;
        material.MaterialType = plainLightShader;
        material.setTexture(1,fogTexture.RenderTexture);
        material.setTexture(2,renderedLights[0].RenderTexture);
        material.setTexture(3,renderedLights[1].RenderTexture);
    }
}

void World::setupForNormalsLighting(irr::scene::ISceneNode* node,bool convertVerts) {
    if (convertVerts) {
        irr::scene::IMesh* mesh = static_cast<irr::scene::IMeshSceneNode*>(node)->getMesh();
        for (irr::u32 i = 0; i <mesh->getMeshBufferCount(); ++i) {
            irrSmgr->getMeshManipulator()->convertVertices<SSkinningVertex>(mesh->getMeshBuffer(i), irrDriver->getVertexDescriptor("Skinning"), false);
            SSkinningVertex* Vertices = static_cast<SSkinningVertex*>(mesh->getMeshBuffer(i)->getVertexBuffer(0)->getVertices());
            for (irr::u32 j=0;j<mesh->getMeshBuffer(i)->getVertexBuffer(0)->getVertexCount();++j) {
                Vertices[j].BlendIndex[0] = 0.f;
                Vertices[j].BlendWeight[0] = 1.f;
                Vertices[j].BlendIndex[1] = 0.f;
                Vertices[j].BlendWeight[1] = 0.f;
                Vertices[j].BlendIndex[2] = 0.f;
                Vertices[j].BlendWeight[2] = 0.f;
                Vertices[j].BlendIndex[3] = 0.f;
                Vertices[j].BlendWeight[3] = 0.f;
            }
        }
        irrSmgr->getMeshManipulator()->recalculateTangents(mesh,true,true,true);
    }
    for (unsigned int i=0;i<node->getMaterialCount();i++) {
        irr::video::SMaterial& material = node->getMaterial(i);
        material.Filter = 1;
        material.MaterialType = normalsShader;
        material.setTexture(3,fogTexture.RenderTexture);
        material.setTexture(4,renderedLights[0].RenderTexture);
        material.setTexture(5,renderedLights[1].RenderTexture);
    }
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

bool World::mouseRotate(irr::scene::IMeshSceneNode* node,irr::core::vector3df rot1,irr::core::vector3df rot2,bool verticalDrag) {
    irr::scene::ICameraSceneNode* cam = mainPlayer->camera;
    float distFactor = (0.4f/cam->getPosition().getDistanceFromSQ(node->getPosition()))*0.1f;
    if (distFactor>0.02f) {
        if ((node->getPosition()-cam->getPosition()).normalize().getDistanceFromSQ(cam->getTarget()-cam->getPosition())<distFactor) {
            drawHandIcon.push_back(irr::core::vector2di(mainWidth/2,mainHeight/2));
            if (irrReceiver->IsMouseDown(0)) {
                //if (mainPlayer->seesMeshNode(node)) {
                float currentRot = 0.f;
                irr::core::vector3df selfRot = node->getRotation();
                /*while (selfRot.X>180.f) { selfRot.X-=360.f; } while (selfRot.X<=-180.f) { selfRot.X+=360.f; }
                while (selfRot.Y>180.f) { selfRot.Y-=360.f; } while (selfRot.Y<=-180.f) { selfRot.Y+=360.f; }
                while (selfRot.Z>180.f) { selfRot.Z-=360.f; } while (selfRot.Z<=-180.f) { selfRot.Z+=360.f; }
                while (rot1.X>180.f) { rot1.X-=360.f; } while (rot1.X<=-180.f) { rot1.X+=360.f; }
                while (rot1.Y>180.f) { rot1.Y-=360.f; } while (rot1.Y<=-180.f) { rot1.Y+=360.f; }
                while (rot1.Z>180.f) { rot1.Z-=360.f; } while (rot1.Z<=-180.f) { rot1.Z+=360.f; }
                while (rot2.X>180.f) { rot2.X-=360.f; } while (rot2.X<=-180.f) { rot2.X+=360.f; }
                while (rot2.Y>180.f) { rot2.Y-=360.f; } while (rot2.Y<=-180.f) { rot2.Y+=360.f; }
                while (rot2.Z>180.f) { rot2.Z-=360.f; } while (rot2.Z<=-180.f) { rot2.Z+=360.f; }*/
                float d1 = selfRot.getDistanceFrom(rot1);
                float d2 = selfRot.getDistanceFrom(rot2);
                currentRot = d2/(d1+d2);
                float dragFactor = 0.f;
                if (verticalDrag) {
                    dragFactor = (float)(mouseDistFromCenter.Y)*0.005f;
                } else {
                    dragFactor = (float)(mouseDistFromCenter.X)*0.005f;
                }
                currentRot+= dragFactor;
                if (currentRot<0.f) { currentRot=0.f; }
                if (currentRot>1.f) { currentRot=1.f; }
                node->setRotation(rot1*currentRot+rot2*(1.f-currentRot));
                return true;
                //}
            }
        }
    }
    return false;
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

	irr::scene::IMeshBuffer* buf = mesh->getMeshBuffer(tri.meshBufferIndex);
	irr::video::ITexture* tex = buf->getMaterial().getTexture(0);
	if (!tex)
		return false;

	texname = std::string(tex->getName().getPath().c_str());
	return true;
}

int coordToRoomGrid(float coord) {
	return (int)((coord+102.4f*RoomScale)/(204.8f*RoomScale));
}
