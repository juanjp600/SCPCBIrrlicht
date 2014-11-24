#include "3dworld.h"
#include "player.h"

#include "rooms/room.h"
#include "rooms/rmesh.h"

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <sstream>

world::world(unsigned int width,unsigned int height,bool fullscreen) {
    mainWidth = width; mainHeight = height;

	scale2D = height/1024.f;
	scale2Db = height/720.f;

    irrDriverType = irr::video::EDT_OPENGL;
    irrReceiver = new MainEventReceiver;
    irrDevice = irr::createDevice(irrDriverType,irr::core::dimension2d<irr::u32>(width,height),32,fullscreen,false,true,irrReceiver);
    irrDriver = irrDevice->getVideoDriver();
	irrSmgr = irrDevice->getSceneManager();
	irrColl = irrSmgr->getSceneCollisionManager();

	room::setSmgr(irrSmgr);

	sound::initSounds();

	menusOpen = menus::NONE;

    irrDriver->setTextureCreationFlag(irr::video::ETCF_ALWAYS_32_BIT, true);

    irrEnv = irrDevice->getGUIEnvironment();

    irrFileSystem = irrDevice->getFileSystem();

    irrTimer = irrDevice->getTimer();
    irrReceiver->setTimer(irrTimer);

	font1 = irr::gui::CGUITTFont::createTTFont(irrEnv, "GFX/cour.ttf", 16*scale2D, true, true);
	font2 = irr::gui::CGUITTFont::createTTFont(irrEnv, "GFX/cour.ttf", 64*scale2D, true, true);

	int seed = irrTimer->getRealTime();
	std::cout<<"Seed: "<<seed<<"\n";

    srand(seed);

    irr::video::IGPUProgrammingServices* irrGpu = irrDriver->getGPUProgrammingServices();
    RoomShader = irr::video::EMT_LIGHTMAP; // Fallback material type
    RoomCallback = new RoomShaderCallBack;
    RoomShader = irrGpu->addHighLevelShaderMaterialFromFiles("GFX/shaders/RoomVertShader.vert", "main", irr::video::EVST_VS_1_1,"GFX/shaders/RoomFragShader.frag", "main", irr::video::EPST_PS_1_1,RoomCallback, irr::video::EMT_LIGHTMAP);

    NormalsShader = irr::video::EMT_SOLID; // Fallback material type
    NormalsCallback= new NormalsShaderCallBack;
    NormalsShader = irrGpu->addHighLevelShaderMaterialFromFiles("GFX/shaders/NewNormalVert.vert", "main", irr::video::EVST_VS_1_1,"GFX/shaders/NewNormalFrag.frag", "main", irr::video::EPST_PS_1_1,NormalsCallback, irr::video::EMT_SOLID);
    NormalsCallback->fvAmbient = irr::video::SColor(255,20,20,20);

	LightsShader = irr::video::EMT_SOLID; // Fallback material type
    LightsCallback= new LightsShaderCallBack;
    LightsShader = irrGpu->addHighLevelShaderMaterialFromFiles("GFX/shaders/LightingVert.vert", "main", irr::video::EVST_VS_1_1,"GFX/shaders/LightingFrag.frag", "main", irr::video::EPST_PS_1_1,LightsCallback, irr::video::EMT_SOLID);
    LightsCallback->fvAmbient = irr::video::SColor(255,20,20,20);

	PostProcShader = irr::video::EMT_SOLID; // Fallback material type
    PostProcCallback= new PostProcShaderCallBack;
    PostProcShader = irrGpu->addHighLevelShaderMaterialFromFiles("GFX/shaders/PostProcessVert.vert", "main", irr::video::EVST_VS_1_1,"GFX/shaders/PostProcessFrag.frag", "main", irr::video::EPST_PS_1_1,PostProcCallback, irr::video::EMT_SOLID);

	ZBufferShader = irr::video::EMT_SOLID; // Fallback material type
    ZBufferCallback= new ZBufferShaderCallBack;
    ZBufferShader = irrGpu->addHighLevelShaderMaterialFromFiles("GFX/shaders/ZBufferVert.vert", "main", irr::video::EVST_VS_1_1,"GFX/shaders/ZBufferFrag.frag", "main", irr::video::EPST_PS_1_1,ZBufferCallback, irr::video::EMT_SOLID);

    blurImage = irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(width,height),"",irr::video::ECF_R8G8B8);
    blurImage2 = irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(width,height),"",irr::video::ECF_R8G8B8);
    ZBuffer = irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(width,height),"",irr::video::ECF_R8G8B8);
    finalImage = irrDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(width,height),"",irr::video::ECF_R8G8B8);

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
	buf->getMaterial().setTexture(1,ZBuffer);

	buf->getMaterial().MaterialType = (irr::video::E_MATERIAL_TYPE)PostProcShader;

	buf->recalculateBoundingBox();

	buf->drop();
	screenQuad = irrSmgr->addMeshSceneNode(quadMesh);
	screenQuad->setVisible(false);

    BlinkMeterIMG = irrDriver->getTexture("GFX/BlinkMeter.jpg");
    StaminaMeterIMG = irrDriver->getTexture("GFX/StaminaMeter.jpg");

	//Add test model

    dynamics = new irrDynamics();
    //gContactAddedCallback = CustomMaterialCombinerCallback;
    dynamics->setGravity(-100*RoomScale);

	irr::scene::IMeshSceneNode* node = nullptr;
    dynRegister* itemDyn = new dynRegister(dynamics);
	item::setDynamics(itemDyn);

    item420::setMeshNode(genItemNode(std::string("GFX/items/420.x"),std::string(""),0.015f*RoomScale));
	itemKey1::setMeshNode(genItemNode(std::string("GFX/items/keycard.x"),std::string("GFX/items/keycard1.jpg"),0.012f*RoomScale));
	itemKey2::setMeshNode(genItemNode(std::string("GFX/items/keycard.x"),std::string("GFX/items/keycard2.jpg"),0.012f*RoomScale));
	itemKey3::setMeshNode(genItemNode(std::string("GFX/items/keycard.x"),std::string("GFX/items/keycard3.jpg"),0.012f*RoomScale));
	itemKey4::setMeshNode(genItemNode(std::string("GFX/items/keycard.x"),std::string("GFX/items/keycard4.jpg"),0.012f*RoomScale));
	itemKey5::setMeshNode(genItemNode(std::string("GFX/items/keycard.x"),std::string("GFX/items/keycard5.jpg"),0.012f*RoomScale));
	itemMisc::setMeshNode(genItemNode(std::string("GFX/items/keycard.x"),std::string("GFX/items/card.jpg"),0.012f*RoomScale));
	itemKey6::setMeshNode(genItemNode(std::string("GFX/items/keycard.x"),std::string("GFX/items/keycardomni.jpg"),0.012f*RoomScale));
	itemScp860::setMeshNode(genItemNode(std::string("GFX/items/key.b3d"),std::string(""),0.03f*RoomScale));
	itemPaper::setMeshNode(genItemNode(std::string("GFX/items/paper.x"),std::string(""),0.09f*RoomScale));
	itemVest::setMeshNode(genItemNode(std::string("GFX/items/vest.x"),std::string("GFX/npcs/MTFbody.jpg"),0.6f*RoomScale));
	itemFinevest::setMeshNode(genItemNode(std::string("GFX/items/vest.x"),std::string("GFX/npcs/MTFbody.jpg"),0.66f*RoomScale));
	itemVeryfinevest::setMeshNode(genItemNode(std::string("GFX/items/vest.x"),std::string("GFX/npcs/MTFbody.jpg"),0.75f*RoomScale));
	itemHazmatsuit::setMeshNode(genItemNode(std::string("GFX/items/hazmat.b3d"),std::string(""),0.39f*RoomScale));
	itemHazmatsuit2::setMeshNode(genItemNode(std::string("GFX/items/hazmat.b3d"),std::string(""),0.39f*RoomScale));
	itemHazmatsuit3::setMeshNode(genItemNode(std::string("GFX/items/hazmat.b3d"),std::string(""),0.39f*RoomScale));
	itemCup::setMeshNode(genItemNode(std::string("GFX/items/cup.x"),std::string(""),1.2f*RoomScale));
	itemEmptycup::setMeshNode(genItemNode(std::string("GFX/items/cup.x"),std::string(""),1.2f*RoomScale));
	itemScp500::setMeshNode(genItemNode(std::string("GFX/items/pill.b3d"),std::string(""),0.03f*RoomScale));
	itemFirstaid::setMeshNode(genItemNode(std::string("GFX/items/firstaid.x"),std::string(""),1.5f*RoomScale));
	itemFinefirstaid::setMeshNode(genItemNode(std::string("GFX/items/firstaid.x"),std::string(""),0.9f*RoomScale));
	itemFirstaid2::setMeshNode(genItemNode(std::string("GFX/items/firstaid.x"),std::string("GFX/items/firstaidkit2.jpg"),0.9f*RoomScale));
	itemVeryfinefirstaid::setMeshNode(genItemNode(std::string("GFX/items/eyedrops.b3d"),std::string("GFX/items/bottle.jpg"),0.06f*RoomScale));
	itemGasmask::setMeshNode(genItemNode(std::string("GFX/items/gasmask.b3d"),std::string(""),0.6f*RoomScale));
	itemSupergasmask::setMeshNode(genItemNode(std::string("GFX/items/gasmask.b3d"),std::string(""),0.63f*RoomScale));
	itemGasmask3::setMeshNode(genItemNode(std::string("GFX/items/gasmask.b3d"),std::string(""),0.63f*RoomScale));
	itemBat::setMeshNode(genItemNode(std::string("GFX/items/battery.x"),std::string(""),0.006f*RoomScale));
	itemScp148::setMeshNode(genItemNode(std::string("GFX/items/metalpanel.x"),std::string(""),0.117187f*RoomScale));
	itemScp148ingot::setMeshNode(genItemNode(std::string("GFX/items/scp148.x"),std::string(""),0.117187f*RoomScale));
	itemNav::setMeshNode(genItemNode(std::string("GFX/items/navigator.x"),std::string(""),0.024f*RoomScale));
	itemRadio::setMeshNode(genItemNode(std::string("GFX/items/radio.x"),std::string(""),30.0f*RoomScale));
	itemFineradio::setMeshNode(genItemNode(std::string("GFX/items/radio.x"),std::string(""),0.03f*RoomScale));
	itemVeryfineradio::setMeshNode(genItemNode(std::string("GFX/items/radio.x"),std::string(""),0.03f*RoomScale));
	item18vradio::setMeshNode(genItemNode(std::string("GFX/items/radio.x"),std::string(""),0.036f*RoomScale));
	itemCigarette::setMeshNode(genItemNode(std::string("GFX/items/420.x"),std::string(""),0.012f*RoomScale));
	item420s::setMeshNode(genItemNode(std::string("GFX/items/420.x"),std::string(""),0.012f*RoomScale));
	itemHand::setMeshNode(genItemNode(std::string("GFX/items/severedhand.b3d"),std::string(""),1.2f*RoomScale));
	itemHand2::setMeshNode(genItemNode(std::string("GFX/items/severedhand.b3d"),std::string("GFX/items/shand2.png"),1.2f*RoomScale));
	item18vbat::setMeshNode(genItemNode(std::string("GFX/items/battery.x"),std::string(""),0.009f*RoomScale));
	itemKillbat::setMeshNode(genItemNode(std::string("GFX/items/battery.x"),std::string(""),0.009f*RoomScale));
	itemFineeyedrops::setMeshNode(genItemNode(std::string("GFX/items/eyedrops.b3d"),std::string(""),0.036f*RoomScale));
	itemSupereyedrops::setMeshNode(genItemNode(std::string("GFX/items/eyedrops.b3d"),std::string(""),0.036f*RoomScale));
	itemEyedrops::setMeshNode(genItemNode(std::string("GFX/items/eyedrops.b3d"),std::string(""),0.036f*RoomScale));
	itemScp714::setMeshNode(genItemNode(std::string("GFX/items/scp714.b3d"),std::string(""),9.0f*RoomScale));
	itemScp1025::setMeshNode(genItemNode(std::string("GFX/items/scp1025.b3d"),std::string(""),3.0f*RoomScale));
	itemScp513::setMeshNode(genItemNode(std::string("GFX/items/513.x"),std::string(""),3.0f*RoomScale));

	for (irr::u32 ui=0;ui<2;ui++) {
        item* it = itemEyedrops::createItemEyedrops();
        itemList.push_back(it);

		it = itemGasmask::createItemGasmask();
        itemList.push_back(it);

        it = itemKey2::createItemKey2();
        itemList.push_back(it);

        it = itemNav::createItemNav();
        itemList.push_back(it);
        it = itemMisc::createItemMisc();
        itemList.push_back(it);
        it = itemScp148ingot::createItemScp148ingot();
        itemList.push_back(it);
        it = itemPaper::createItemPaper();
        itemList.push_back(it);

        it = itemKey3::createItemKey3();
        itemList.push_back(it);

        it = itemKey4::createItemKey4();
        itemList.push_back(it);

        it = itemKey5::createItemKey5();
        itemList.push_back(it);

        it = itemKey6::createItemKey6();
        itemList.push_back(it);
    }

	btRigidBody* rbody;

	room::setDynamics(itemDyn);

	ambient[0] = sound::getSound(std::string("SFX/Music/The Dread.ogg"),false);
	if (ambient[0]!=nullptr) {
		ambient[0]->playSound(true);
	}

	RMesh* rme;
	//LCZ
	/*lockroom*/rme = loadRMesh(std::string("GFX/map/lockroom_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); lockroom::setBase(rme);
	/*start*/rme = loadRMesh(std::string("GFX/map/173_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); start::setBase(rme);
	/*room2storage*/rme = loadRMesh(std::string("GFX/map/room2storage_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2storage::setBase(rme);
	/*room3storage*/rme = loadRMesh(std::string("GFX/map/room3storage_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room3storage::setBase(rme);
	/*endroom*/rme = loadRMesh(std::string("GFX/map/endroom_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); endroom::setBase(rme);
	/*room012*/rme = loadRMesh(std::string("GFX/map/room012_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room012::setBase(rme);
	/*room2*/rme = loadRMesh(std::string("GFX/map/room2_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2::setBase(rme);
	/*room2_2*/rme = loadRMesh(std::string("GFX/map/room2_2_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2_2::setBase(rme);
	/*room2c*/rme = loadRMesh(std::string("GFX/map/room2C_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2c::setBase(rme);
	/*room2closets*/rme = loadRMesh(std::string("GFX/map/room2closets_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2closets::setBase(rme);
	/*room2elevator*/rme = loadRMesh(std::string("GFX/map/room2elevator_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2elevator::setBase(rme);
	/*room2doors*/rme = loadRMesh(std::string("GFX/map/room2doors_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2doors::setBase(rme);
	/*room2scps*/rme = loadRMesh(std::string("GFX/map/room2scps_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2scps::setBase(rme);
	/*room3storage*/rme = loadRMesh(std::string("GFX/map/room3storage_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room3storage::setBase(rme);
	/*room2testroom2*/rme = loadRMesh(std::string("GFX/map/room2testroom2_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2testroom2::setBase(rme);
	/*room3*/rme = loadRMesh(std::string("GFX/map/room3_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room3::setBase(rme);
	/*room3_2*/rme = loadRMesh(std::string("GFX/map/room3_2_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room3_2::setBase(rme);
	/*room4*/rme = loadRMesh(std::string("GFX/map/room4_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room4::setBase(rme);
	/*roompj*/rme = loadRMesh(std::string("GFX/map/roompj_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); roompj::setBase(rme);
	/*r_914*/rme = loadRMesh(std::string("GFX/map/machineroom_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); r_914::setBase(rme);
	#if 0
	//HCZ
	/*r_008*/rme = loadRMesh(std::string("GFX/map/008_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); r_008::setBase(rme);
	/*coffin*/rme = loadRMesh(std::string("GFX/map/coffin_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); coffin::setBase(rme);
	/*endroom2*/rme = loadRMesh(std::string("GFX/map/endroom2_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); endroom2::setBase(rme);
	/*testroom*/rme = loadRMesh(std::string("GFX/map/testroom_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); testroom::setBase(rme);
	/*tunnel*/rme = loadRMesh(std::string("GFX/map/tunnel_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); tunnel::setBase(rme);
	/*tunnel2*/rme = loadRMesh(std::string("GFX/map/tunnel2_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); tunnel2::setBase(rme);
	/*room035*/rme = loadRMesh(std::string("GFX/map/room035_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room035::setBase(rme);
	/*room049*/rme = loadRMesh(std::string("GFX/map/room049_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room049::setBase(rme);
	/*room106*/rme = loadRMesh(std::string("GFX/map/room106_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room106::setBase(rme);
	/*room2ctunnel*/rme = loadRMesh(std::string("GFX/map/room2Ctunnel_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2ctunnel::setBase(rme);
	/*room2nuke*/rme = loadRMesh(std::string("GFX/map/room2nuke_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2nuke::setBase(rme);
	/*room2pipes*/rme = loadRMesh(std::string("GFX/map/room2pipes_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2pipes::setBase(rme);
	/*room2pit*/rme = loadRMesh(std::string("GFX/map/room2pit_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2pit::setBase(rme);
	/*room3pit*/rme = loadRMesh(std::string("GFX/map/room3pit_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room3pit::setBase(rme);
	/*room2servers*/rme = loadRMesh(std::string("GFX/map/room2servers_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2servers::setBase(rme);
	/*room2tunnel*/rme = loadRMesh(std::string("GFX/map/room2tunnel_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2tunnel::setBase(rme);
	/*room3tunnel*/rme = loadRMesh(std::string("GFX/map/room3tunnel_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room3tunnel::setBase(rme);
	/*room4tunnels*/rme = loadRMesh(std::string("GFX/map/4tunnels_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room4tunnels::setBase(rme);
	/*room513*/rme = loadRMesh(std::string("GFX/map/room513_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room513::setBase(rme);
	//EZ
	/*room860*/rme = loadRMesh(std::string("GFX/map/room860_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room860::setBase(rme);
	/*exit1*/rme = loadRMesh(std::string("GFX/map/exit1_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); exit1::setBase(rme);
	/*gateaentrance*/rme = loadRMesh(std::string("GFX/map/gateaentrance_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); gateaentrance::setBase(rme);
	/*lockroom2*/rme = loadRMesh(std::string("GFX/map/lockroom2_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); lockroom2::setBase(rme);
	/*room079*/rme = loadRMesh(std::string("GFX/map/room079_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room079::setBase(rme);
	/*room2z3*/rme = loadRMesh(std::string("GFX/map/room2z3_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2z3::setBase(rme);
	/*room2cafeteria*/rme = loadRMesh(std::string("GFX/map/room2cafeteria_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2cafeteria::setBase(rme);
	/*room2cz3*/rme = loadRMesh(std::string("GFX/map/room2Cz3_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2cz3::setBase(rme);
	/*room2ccont*/rme = loadRMesh(std::string("GFX/map/room2ccont_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2ccont::setBase(rme);
	/*room2offices*/rme = loadRMesh(std::string("GFX/map/room2offices_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2offices::setBase(rme);
	/*room2offices2*/rme = loadRMesh(std::string("GFX/map/room2offices2_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2offices2::setBase(rme);
	/*room2offices3*/rme = loadRMesh(std::string("GFX/map/room2offices3_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2offices3::setBase(rme);
	/*room2poffices*/rme = loadRMesh(std::string("GFX/map/room2poffices_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2poffices::setBase(rme);
	/*room2poffices2*/rme = loadRMesh(std::string("GFX/map/room2poffices2_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2poffices2::setBase(rme);
	/*room2sroom*/rme = loadRMesh(std::string("GFX/map/room2sroom_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2sroom::setBase(rme);
	/*room2toilets*/rme = loadRMesh(std::string("GFX/map/room2toilets_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2toilets::setBase(rme);
	/*room2tesla*/rme = loadRMesh(std::string("GFX/map/room2tesla_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room2tesla::setBase(rme);
	/*room3servers*/rme = loadRMesh(std::string("GFX/map/room3servers_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room3servers::setBase(rme);
	/*room3servers2*/rme = loadRMesh(std::string("GFX/map/room3servers2_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room3servers2::setBase(rme);
	/*room3z3*/rme = loadRMesh(std::string("GFX/map/room3z3_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room3z3::setBase(rme);
	/*room4z3*/rme = loadRMesh(std::string("GFX/map/room4z3_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); room4z3::setBase(rme);
	//Misc
	/*r_173*/rme = loadRMesh(std::string("GFX/map/173bright_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); r_173::setBase(rme);
	/*checkpoint1*/rme = loadRMesh(std::string("GFX/map/checkpoint1_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); checkpoint1::setBase(rme);
	/*checkpoint2*/rme = loadRMesh(std::string("GFX/map/checkpoint2_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); checkpoint2::setBase(rme);
	/*gatea*/rme = loadRMesh(std::string("GFX/map/gatea_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); gatea::setBase(rme);
	/*pocketdimension*/rme = loadRMesh(std::string("GFX/map/pocketdimension1_opt.rm2"),irrFileSystem,irrDriver,(irr::video::E_MATERIAL_TYPE)RoomShader); pocketdimension::setBase(rme);
	#endif

	mainPlayer = new player(this,irrSmgr,dynamics,irrReceiver);

	mainPlayer->update();

	createMap(0);

	irrDevice->getCursorControl()->setVisible(false);
	irrDevice->getCursorControl()->setPosition((irr::s32)mainWidth/2,(irr::s32)mainHeight/2);

	for (int y=19;y>=0;y--) {
		for (int x=19;x>=0;x--) {
			if (roomArray[x][y]!=nullptr) {
				//test node
				irr::scene::IMesh* mesh1 = irrSmgr->getMesh("GFX/npcs/173_2.b3d");

				node = irrSmgr->addMeshSceneNode(mesh1);

				node->setScale(irr::core::vector3df(1.3*RoomScale));
				node->setPosition(irr::core::vector3df(x*204.8f*RoomScale,10*RoomScale,y*204.8f*RoomScale));
				node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
				node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);

				node->getMaterial(0).Lighting = true;
				node->getMaterial(0).MaterialType = (irr::video::E_MATERIAL_TYPE)NormalsShader;

				rbody = dynamics->addTriMesh_moving(node,16000.f,20,1,1);
				rbody->setAngularFactor(btVector3(0,0,0));

				node->getMaterial(0).setTexture(1, irrDriver->getTexture("GFX/npcs/173_norm.jpg"));
				node->getMaterial(0).setTexture(2, irrDriver->getTexture("GFX/npcs/173_Spec.jpg"));

				node->getMaterial(0).EmissiveColor = irr::video::SColor(100,100,100,100);

				mainPlayer->testNode = node;
				//------------

				mesh1 = irrSmgr->getMesh("GFX/npcs/scp-066.b3d");

				node = irrSmgr->addMeshSceneNode(mesh1);


				node->setScale(irr::core::vector3df(2.1*RoomScale));
				node->setPosition(irr::core::vector3df(x*204.8f*RoomScale,10*RoomScale,y*204.8f*RoomScale));

				rbody = dynamics->addTriMesh_moving(node,5.0f,5,1,1);
				rbody->setFriction(10.0f);
				rbody->setDamping(0,0);
				node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);

				node->getMaterial(0).setTexture(1, irrDriver->getTexture("GFX/npcs/scp-066_normal.png"));
				node->getMaterial(0).setTexture(2, irrDriver->getTexture("GFX/npcs/scp-066_specular.png"));
				node->getMaterial(1).MaterialType = (irr::video::E_MATERIAL_TYPE)NormalsShader;
				node->getMaterial(1).setTexture(1, irrDriver->getTexture("GFX/npcs/scp-066_normal.png"));
				node->getMaterial(1).setTexture(2, irrDriver->getTexture("GFX/npcs/scp-066_specular.png"));
				node->getMaterial(0).MaterialType = (irr::video::E_MATERIAL_TYPE)NormalsShader;

				node->getMaterial(0).SpecularColor.set(0,0,0,0);
				node->getMaterial(0).Shininess = 0.f;
				node->getMaterial(1).SpecularColor.set(0,0,0,0);
				node->getMaterial(1).Shininess = 0.0f;

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

world::~world() {


    if (mainPlayer!=nullptr) { delete mainPlayer; mainPlayer=nullptr; }
    for (unsigned char i=0;i<inventory_size;i++) {
		if (invImgs[i]!=nullptr) { irrDriver->removeTexture(invImgs[i]); invImgs[i]=nullptr; }
    }
    for (unsigned char i=0;i<3;i++) {
		if (pauseImgs[i]!=nullptr) { irrDriver->removeTexture(pauseImgs[i]); pauseImgs[i]=nullptr; }
    }
    sound::killSounds();
    irrDevice->drop();

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
}

bool world::run() {

	if (prevTime==0) { FPSfactor = 1.0; } else {
		FPSfactor = (irrTimer->getRealTime()-prevTime)/(1000.0/70.0);
	}
	prevTime = irrTimer->getRealTime();

	if (menusOpen==menus::NONE) {

		float prec = 0.65f;

		mainPlayer->update();
		dynamics->simStep(irrTimer->getRealTime(),60.f * prec);
		mainPlayer->resetSpeeds();

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
					std::vector<irr::video::SLight> nLights = roomArray[px][py]->getPointLights();
					irr::core::vector3df offset((px*204.8f*RoomScale),0.f,(py*204.8f*RoomScale));
					for (unsigned int i=0;i<nLights.size();i++) {
						irr::core::matrix4 rotMatrix;
						rotMatrix.setRotationDegrees(irr::core::vector3df(0.f,roomArray[px][py]->getAngle()*90.f,0.f));
						rotMatrix.transformVect(nLights[i].Position);

						nLights[i].Position+=offset;

					}
					LightsCallback->setLights(nLights);
					NormalsCallback->setLights(nLights);
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
		dynamics->resetTimer(irrTimer->getRealTime());
	}

    irrDriver->beginScene(true, true, irr::video::SColor(255, 255, 0, 255));

	draw3D();
	drawHUD();

    irrDriver->endScene();

	if (irrTimer->getRealTime()-prevTime<17) irrDevice->sleep(17-(irrTimer->getRealTime()-prevTime));

	sound::processDrops();
	irrReceiver->perLoopUpdate();
	prevMenusOpen = menusOpen;
    return irrDevice->run();
}

void world::draw3D() {

	PostProcCallback->fpsFactor = FPSfactor;

	irrDriver->setRenderTarget(blurImage2); //copy the old render
    irrDriver->draw2DImage(finalImage,irr::core::position2d<irr::s32>(0,0),irr::core::rect<irr::s32>(0,0,mainWidth,mainHeight), 0,irr::video::SColor(255,255,255,255), false);
    irrDriver->setRenderTarget(blurImage); //create a new render, using the old one to add a blur effect
    irrSmgr->drawAll();
    float BlinkTimer = mainPlayer->BlinkTimer;
    if (BlinkTimer<0) {
        float darkA = 0.f;
        if (mainPlayer->BlinkTimer>=-0.5f) {
			darkA = std::max(0.f,std::min(-mainPlayer->BlinkTimer*2.f,1.f));
        } else if (mainPlayer->BlinkTimer>=-1.0f) {
            darkA = std::max(0.f,std::min(1.f+(mainPlayer->BlinkTimer+0.5f)*2.f,1.f));
        }
        darkA = 1.f-darkA;
        darkA *= darkA;
        darkA = 1.f-darkA;
        irrDriver->draw2DRectangle(irr::video::SColor(std::min(255.f,darkA*255.f),0,0,0),irr::core::rect<irr::s32>(0,0,mainWidth,mainHeight));
        PostProcCallback->minBlur = darkA*6.f;
    } else {
		PostProcCallback->minBlur = 0.f;
		if (menusOpen!=menus::NONE) PostProcCallback->minBlur = 2.f;
    }

    irrDriver->setRenderTarget(ZBuffer,true,true,irr::video::SColor(255,255,255,255)); //white = far

    irrDriver->getOverrideMaterial().EnableFlags = irr::video::EMF_MATERIAL_TYPE;
	irrDriver->getOverrideMaterial().EnablePasses = irr::scene::ESNRP_SOLID;
	irrDriver->getOverrideMaterial().Material.MaterialType = (irr::video::E_MATERIAL_TYPE)ZBufferShader;
	irrSmgr->drawAll();
	irrDriver->getOverrideMaterial().EnablePasses = 0;

    irrDriver->setRenderTarget(finalImage); //draw to screen
    //irrDriver->clearZBuffer();

	screenQuad->render();
	irrDriver->draw2DImage(blurImage2,irr::core::position2d<irr::s32>(0,0),irr::core::rect<irr::s32>(0,0,mainWidth,mainHeight), 0,irr::video::SColor(std::min(blurAlpha/FPSfactor,200.0f),255,255,255), false);

	irrDriver->setRenderTarget(0);
	irrDriver->draw2DImage(finalImage,irr::core::position2d<irr::s32>(0,0),irr::core::rect<irr::s32>(0,0,mainWidth,mainHeight), 0,irr::video::SColor(255,255,255,255), false);
}

void world::drawHUD() {
	if (hudMsgTimer>0.f) {
		hudMsgTimer-=getFPSfactor();

		float alpha = std::min(std::max(hudMsgTimer*0.01f,0.f),1.f);
		font1->draw(hudMsg.c_str(),irr::core::rect<irr::s32>(mainWidth/2+2,mainHeight+2,mainWidth/2+2,mainHeight/2+102),irr::video::SColor(150*alpha,0,0,0),true,true);
		font1->draw(hudMsg.c_str(),irr::core::rect<irr::s32>(mainWidth/2,mainHeight,mainWidth/2,mainHeight/2+100),irr::video::SColor(255*alpha,255,255,255),true,true);
	}

    //Blink meter
    irrDriver->draw2DLine(irr::core::position2d<irr::s32>(80*scale2Db,mainHeight-95*scale2Db),irr::core::position2d<irr::s32>(284*scale2Db,mainHeight-95*scale2Db),irr::video::SColor(255,255,255,255));
    irrDriver->draw2DLine(irr::core::position2d<irr::s32>(80*scale2Db,mainHeight-75*scale2Db),irr::core::position2d<irr::s32>(284*scale2Db,mainHeight-75*scale2Db),irr::video::SColor(255,255,255,255));
    irrDriver->draw2DLine(irr::core::position2d<irr::s32>(80*scale2Db,mainHeight-95*scale2Db),irr::core::position2d<irr::s32>(80*scale2Db,mainHeight-75*scale2Db),irr::video::SColor(255,255,255,255));
    irrDriver->draw2DLine(irr::core::position2d<irr::s32>(284*scale2Db,mainHeight-95*scale2Db),irr::core::position2d<irr::s32>(284*scale2Db,mainHeight-75*scale2Db),irr::video::SColor(255,255,255,255));

    for (int i=0;i<mainPlayer->BlinkTimer/100*20;i++) {
        irrDriver->draw2DImage(BlinkMeterIMG,irr::core::recti((83+i*10)*scale2Db,mainHeight-92*scale2Db,(91+i*10)*scale2Db,mainHeight-78*scale2Db),irr::core::recti(0,0,8,14));
    }

    //Stamina meter
    irrDriver->draw2DLine(irr::core::position2d<irr::s32>(80*scale2Db,mainHeight-55*scale2Db),irr::core::position2d<irr::s32>(284*scale2Db,mainHeight-55*scale2Db),irr::video::SColor(255,255,255,255));
    irrDriver->draw2DLine(irr::core::position2d<irr::s32>(80*scale2Db,mainHeight-35*scale2Db),irr::core::position2d<irr::s32>(284*scale2Db,mainHeight-35*scale2Db),irr::video::SColor(255,255,255,255));
    irrDriver->draw2DLine(irr::core::position2d<irr::s32>(80*scale2Db,mainHeight-55*scale2Db),irr::core::position2d<irr::s32>(80*scale2Db,mainHeight-35*scale2Db),irr::video::SColor(255,255,255,255));
    irrDriver->draw2DLine(irr::core::position2d<irr::s32>(284*scale2Db,mainHeight-55*scale2Db),irr::core::position2d<irr::s32>(284*scale2Db,mainHeight-35*scale2Db),irr::video::SColor(255,255,255,255));

    for (int i=0;i<mainPlayer->Stamina/100*20;i++) {
        irrDriver->draw2DImage(StaminaMeterIMG,irr::core::recti((83+i*10)*scale2Db,mainHeight-52*scale2Db,(91+i*10)*scale2Db,mainHeight-38*scale2Db),irr::core::recti(0,0,8,14));
    }

    blurAlpha = 100;

    if (menusOpen==menus::INVOPEN) {
		if (prevMenusOpen!=menusOpen) {
			sound::freezeCategory(1);
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
			sound::unfreezeCategory(1);
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
		if (pauseImgs[0]==nullptr) {
			pauseImgs[0] = irrDriver->getTexture("GFX/menu/pausemenu.jpg");
			pauseImgs[1] = irrDriver->getTexture("GFX/menu/menublack.jpg");
			pauseImgs[2] = irrDriver->getTexture("GFX/menu/menuwhite.jpg");
			irrReceiver->perLoopUpdate();
			irrDevice->getCursorControl()->setVisible(true);
			sound::freezeCategory(1);
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

			if (button(std::string("Resume"),mainWidth/2-180*scale2D,mainHeight/2,455*scale2D,70*scale2D)) menusOpen = menus::NONE;
			if (button(std::string("Options"),mainWidth/2-180*scale2D,mainHeight/2+80*scale2D,455*scale2D,70*scale2D)) menusOpen = menus::OPTIONSOPEN;
			if (button(std::string("Quit"),mainWidth/2-180*scale2D,mainHeight/2+160*scale2D,455*scale2D,70*scale2D)) irrDevice->closeDevice(); //TODO: add main menu

			if (irrReceiver->IsPrevKeyDown(irr::KEY_ESCAPE)==true && irrReceiver->IsKeyDown(irr::KEY_ESCAPE)==false) menusOpen = menus::NONE;
		} else {
			font2->draw("OPTIONS",irr::core::recti(mainWidth/2-180*scale2D,mainHeight/2-300*scale2D,mainWidth/2+300*scale2D,mainHeight/2-205*scale2D),irr::video::SColor(255,255,255,255),true,true);

			if (button(std::string("Graphics"),mainWidth/2-175*scale2D,mainHeight/2-190*scale2D,140*scale2D,24*scale2D)) subMenusOpen=0;
			if (button(std::string("Audio"),mainWidth/2-20*scale2D,mainHeight/2-190*scale2D,140*scale2D,24*scale2D)) subMenusOpen=1;
			if (button(std::string("Controls"),mainWidth/2+135*scale2D,mainHeight/2-190*scale2D,140*scale2D,24*scale2D)) subMenusOpen=2;

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
					ss << PostProcCallback->gammaFactor;
					s = ss.str();

					font1->draw(s.c_str(),irr::core::recti(mainWidth/2-150*scale2D,mainHeight/2-125*scale2D,mainWidth/2+100*scale2D,mainHeight/2-110*scale2D),irr::video::SColor(255,255,255,255),false,false);

					outline = irr::core::recti(mainWidth/2-150*scale2D,mainHeight/2-100*scale2D,mainWidth/2+260*scale2D,mainHeight/2-86*scale2D);
					irrDriver->draw2DRectangleOutline(irr::core::recti(outline.UpperLeftCorner.X-3*scale2D,outline.UpperLeftCorner.Y-3*scale2D,outline.LowerRightCorner.X+3*scale2D,outline.LowerRightCorner.Y+3*scale2D));

					if (irrReceiver->IsMouseDown(0)) {
						if (irrReceiver->getMousePos().X>outline.UpperLeftCorner.X && irrReceiver->getMousePos().Y>outline.UpperLeftCorner.Y &&
							irrReceiver->getMousePos().X<outline.LowerRightCorner.X && irrReceiver->getMousePos().Y<outline.LowerRightCorner.Y) {
							PostProcCallback->gammaFactor = ((irrReceiver->getMousePos().X-outline.UpperLeftCorner.X)/(float)(outline.LowerRightCorner.X-outline.UpperLeftCorner.X))*1.5f+0.5f;
							PostProcCallback->gammaFactor = ((int)(PostProcCallback->gammaFactor*100.f))*0.01f;
							PostProcCallback->invGammaFactor = 1.f/PostProcCallback->gammaFactor;
						}
					}

					irrDriver->draw2DImage(BlinkMeterIMG,irr::core::recti(
						mainWidth/2-150*scale2D+(PostProcCallback->gammaFactor-0.5f)/1.5f*406*scale2D,
						mainHeight/2-100*scale2D,
						mainWidth/2-142*scale2D+(PostProcCallback->gammaFactor-0.5f)/1.5f*406*scale2D,mainHeight/2-86*scale2D),
						irr::core::recti(0,0,8,14));

					irrDriver->draw2DRectangle(irr::video::SColor(255,0,0,0),irr::core::recti(mainWidth/2-150*scale2D,mainHeight/2-65*scale2D,mainWidth/2+250*scale2D,mainHeight/2+100*scale2D));

					color = std::pow(2.5f/255.f,PostProcCallback->invGammaFactor)*255;
					irrDriver->draw2DRectangle(irr::video::SColor(255,color,color,color),irr::core::recti(mainWidth/2-100*scale2D,mainHeight/2-20*scale2D,mainWidth/2-32*scale2D,mainHeight/2+48*scale2D));
					color = std::pow(1.25f/255.f,PostProcCallback->invGammaFactor)*255;
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
			sound::unfreezeCategory(1);
		}
	}
}

bool world::button(const std::string &text,int x,int y,int w,int h) {
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

float world::getFPSfactor() {
    return FPSfactor;
}

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

inline int coordToRoomGrid(float coord) {
	return (int)((coord+102.4f*RoomScale)/(204.8f*RoomScale));
}
