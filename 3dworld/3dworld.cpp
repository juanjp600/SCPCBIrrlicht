#include "3dworld.h"
#include "player.h"
//#include "rooms/rmesh.h"
#include "rooms/room.h"

#include <iostream>
#include <cmath>
#include <cstdlib>

const unsigned char world::PAUSEOPEN = 1;
const unsigned char world::INVOPEN = 2;

world::world(unsigned int width,unsigned int height,bool fullscreen) {
    mainWidth = width; mainHeight = height;

    irrDriverType = irr::video::EDT_OPENGL;
    irrReceiver = new MainEventReceiver;
    irrDevice = irr::createDevice(irrDriverType,irr::core::dimension2d<irr::u32>(width,height),32,fullscreen,false,true,irrReceiver);
    irrDriver = irrDevice->getVideoDriver();
	irrSmgr = irrDevice->getSceneManager();
	irrColl = irrSmgr->getSceneCollisionManager();

	room::setSmgr(irrSmgr);

	sound::initSounds();

	menusOpen = INVOPEN;

	//irrSmgr->setAmbientLight(irr::video::SColor(255,20,20,20));

    irrDriver->setTextureCreationFlag(irr::video::ETCF_ALWAYS_32_BIT, true);

    irrEnv = irrDevice->getGUIEnvironment();

    irrFileSystem = irrDevice->getFileSystem();

    irrTimer = irrDevice->getTimer();
    irrReceiver->setTimer(irrTimer);

	font1 = irr::gui::CGUITTFont::createTTFont(irrEnv, "test/arial.ttf", 16, true, true);

	int seed = irrTimer->getRealTime();
	std::cout<<"Seed: "<<seed<<"\n";

    srand(seed);

    irr::video::IGPUProgrammingServices* irrGpu = irrDriver->getGPUProgrammingServices();
    RoomShader = irr::video::EMT_LIGHTMAP; // Fallback material type
    RoomCallback = new RoomShaderCallBack;
    RoomShader = irrGpu->addHighLevelShaderMaterialFromFiles("RoomVertShader.txt", "vertMain", irr::video::EVST_VS_1_1,"RoomFragShader.txt", "fragMain", irr::video::EPST_PS_1_1,RoomCallback, irr::video::EMT_LIGHTMAP);

    NormalsShader = irr::video::EMT_SOLID; // Fallback material type
    NormalsCallback= new NormalsShaderCallBack;
    NormalsShader = irrGpu->addHighLevelShaderMaterialFromFiles("NewNormalVert.txt", "main", irr::video::EVST_VS_1_1,"NewNormalFrag.txt", "main", irr::video::EPST_PS_1_1,NormalsCallback, irr::video::EMT_SOLID);
    NormalsCallback->fvAmbient = irr::video::SColor(255,20,20,20);

	LightsShader = irr::video::EMT_SOLID; // Fallback material type
    LightsCallback= new LightsShaderCallBack;
    LightsShader = irrGpu->addHighLevelShaderMaterialFromFiles("LightingVert.txt", "main", irr::video::EVST_VS_1_1,"LightingFrag.txt", "main", irr::video::EPST_PS_1_1,LightsCallback, irr::video::EMT_SOLID);
    LightsCallback->fvAmbient = irr::video::SColor(255,20,20,20);

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
	item::setDynamics(itemDyn);

    /*node = irrSmgr->addMeshSceneNode(irrSmgr->getMesh("test/eyedrops.b3d"));
    node->setMaterialType((irr::video::E_MATERIAL_TYPE)LightsShader);

    node->setScale(irr::core::vector3df(0.06*RoomScale,0.06*RoomScale,0.06*RoomScale));
    node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
    itemEyedrops::setMeshNode(node);

    node = irrSmgr->addMeshSceneNode(irrSmgr->getMesh("test/gasmask.b3d"));
    //node->setDebugDataVisible(irr::scene::EDS_NORMALS);
    node->setMaterialType((irr::video::E_MATERIAL_TYPE)LightsShader);

    node->setScale(irr::core::vector3df(0.6*RoomScale,0.6*RoomScale,0.6*RoomScale));
    node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
    itemGasMask::setMeshNode(node);

    for (irr::u32 ui=0;ui<10;ui++) {
        item* it = itemEyedrops::createItemEyedrops();
        itemList.push_back(it);

        it = itemGasMask::createItemGasMask();
        itemList.push_back(it);
    }*/

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

        /*it = item18vbat::createItem18vbat();
        itemList.push_back(it);*/
    }

	btRigidBody* rbody;

	/*btTransform Transform;
	Transform.setIdentity();
	btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);
	btVector3 localInertia(0,0,0);
	btRigidBody* rbody = new btRigidBody(0.f, MotionState, rme->shape,localInertia);
	//rbody->setActivationState(DISABLE_DEACTIVATION);
	dynamics->registerNewRBody(node,rbody,0);
	//all rigid bodies must have a friction value for friction to work
	rbody->setFriction(1.f);
	rbody->setRollingFriction(1.f);*/

	//node->setScale(irr::core::vector3df(0.1f*RoomScale));

	room::setDynamics(itemDyn);

	ambient[0] = sound::getSound(std::string("SFX/Music/The Dread.ogg"),false);
	if (ambient[0]!=nullptr) {
		ambient[0]->playSound(true);
	}

	RMesh* rme;
	//LCZ
	/*lockroom*/rme = loadRMesh(std::string("GFX/map/lockroom_opt.rmesh"),irrFileSystem,irrDriver); lockroom::setBase(rme);
	/*start*/rme = loadRMesh(std::string("GFX/map/173_opt.rmesh"),irrFileSystem,irrDriver); start::setBase(rme);
	/*room2storage*/rme = loadRMesh(std::string("GFX/map/room2storage_opt.rmesh"),irrFileSystem,irrDriver); room2storage::setBase(rme);
	/*room3storage*/rme = loadRMesh(std::string("GFX/map/room3storage_opt.rmesh"),irrFileSystem,irrDriver); room3storage::setBase(rme);
	/*endroom*/rme = loadRMesh(std::string("GFX/map/endroom_opt.rmesh"),irrFileSystem,irrDriver); endroom::setBase(rme);
	/*room012*/rme = loadRMesh(std::string("GFX/map/room012_opt.rmesh"),irrFileSystem,irrDriver); room012::setBase(rme);
	/*room2*/rme = loadRMesh(std::string("GFX/map/room2_opt.rmesh"),irrFileSystem,irrDriver); room2::setBase(rme);
	/*room2_2*/rme = loadRMesh(std::string("GFX/map/room2_2_opt.rmesh"),irrFileSystem,irrDriver); room2_2::setBase(rme);
	/*room2c*/rme = loadRMesh(std::string("GFX/map/room2C_opt.rmesh"),irrFileSystem,irrDriver); room2c::setBase(rme);
	/*room2closets*/rme = loadRMesh(std::string("GFX/map/room2closets_opt.rmesh"),irrFileSystem,irrDriver); room2closets::setBase(rme);
	/*room2elevator*/rme = loadRMesh(std::string("GFX/map/room2elevator_opt.rmesh"),irrFileSystem,irrDriver); room2elevator::setBase(rme);
	/*room2doors*/rme = loadRMesh(std::string("GFX/map/room2doors_opt.rmesh"),irrFileSystem,irrDriver); room2doors::setBase(rme);
	/*room2scps*/rme = loadRMesh(std::string("GFX/map/room2scps_opt.rmesh"),irrFileSystem,irrDriver); room2scps::setBase(rme);
	/*room3storage*/rme = loadRMesh(std::string("GFX/map/room3storage_opt.rmesh"),irrFileSystem,irrDriver); room3storage::setBase(rme);
	/*room2testroom2*/rme = loadRMesh(std::string("GFX/map/room2testroom2_opt.rmesh"),irrFileSystem,irrDriver); room2testroom2::setBase(rme);
	/*room3*/rme = loadRMesh(std::string("GFX/map/room3_opt.rmesh"),irrFileSystem,irrDriver); room3::setBase(rme);
	/*room3_2*/rme = loadRMesh(std::string("GFX/map/room3_2_opt.rmesh"),irrFileSystem,irrDriver); room3_2::setBase(rme);
	/*room4*/rme = loadRMesh(std::string("GFX/map/room4_opt.rmesh"),irrFileSystem,irrDriver); room4::setBase(rme);
	/*roompj*/rme = loadRMesh(std::string("GFX/map/roompj_opt.rmesh"),irrFileSystem,irrDriver); roompj::setBase(rme);
	/*r_914*/rme = loadRMesh(std::string("GFX/map/machineroom_opt.rmesh"),irrFileSystem,irrDriver); r_914::setBase(rme);
	//HCZ
	/*r_008*/rme = loadRMesh(std::string("GFX/map/008_opt.rmesh"),irrFileSystem,irrDriver); r_008::setBase(rme);
	/*coffin*/rme = loadRMesh(std::string("GFX/map/coffin_opt.rmesh"),irrFileSystem,irrDriver); coffin::setBase(rme);
	/*endroom2*/rme = loadRMesh(std::string("GFX/map/endroom2_opt.rmesh"),irrFileSystem,irrDriver); endroom2::setBase(rme);
	/*testroom*/rme = loadRMesh(std::string("GFX/map/testroom_opt.rmesh"),irrFileSystem,irrDriver); testroom::setBase(rme);
	/*tunnel*/rme = loadRMesh(std::string("GFX/map/tunnel_opt.rmesh"),irrFileSystem,irrDriver); tunnel::setBase(rme);
	/*tunnel2*/rme = loadRMesh(std::string("GFX/map/tunnel2_opt.rmesh"),irrFileSystem,irrDriver); tunnel2::setBase(rme);
	/*room035*/rme = loadRMesh(std::string("GFX/map/room035_opt.rmesh"),irrFileSystem,irrDriver); room035::setBase(rme);
	/*room049*/rme = loadRMesh(std::string("GFX/map/room049_opt.rmesh"),irrFileSystem,irrDriver); room049::setBase(rme);
	/*room106*/rme = loadRMesh(std::string("GFX/map/room106_opt.rmesh"),irrFileSystem,irrDriver); room106::setBase(rme);
	/*room2ctunnel*/rme = loadRMesh(std::string("GFX/map/room2Ctunnel_opt.rmesh"),irrFileSystem,irrDriver); room2ctunnel::setBase(rme);
	/*room2nuke*/rme = loadRMesh(std::string("GFX/map/room2nuke_opt.rmesh"),irrFileSystem,irrDriver); room2nuke::setBase(rme);
	/*room2pipes*/rme = loadRMesh(std::string("GFX/map/room2pipes_opt.rmesh"),irrFileSystem,irrDriver); room2pipes::setBase(rme);
	/*room2pit*/rme = loadRMesh(std::string("GFX/map/room2pit_opt.rmesh"),irrFileSystem,irrDriver); room2pit::setBase(rme);
	/*room3pit*/rme = loadRMesh(std::string("GFX/map/room3pit_opt.rmesh"),irrFileSystem,irrDriver); room3pit::setBase(rme);
	/*room2servers*/rme = loadRMesh(std::string("GFX/map/room2servers_opt.rmesh"),irrFileSystem,irrDriver); room2servers::setBase(rme);
	/*room2tunnel*/rme = loadRMesh(std::string("GFX/map/room2tunnel_opt.rmesh"),irrFileSystem,irrDriver); room2tunnel::setBase(rme);
	/*room3tunnel*/rme = loadRMesh(std::string("GFX/map/room3tunnel_opt.rmesh"),irrFileSystem,irrDriver); room3tunnel::setBase(rme);
	/*room4tunnels*/rme = loadRMesh(std::string("GFX/map/4tunnels_opt.rmesh"),irrFileSystem,irrDriver); room4tunnels::setBase(rme);
	/*room513*/rme = loadRMesh(std::string("GFX/map/room513_opt.rmesh"),irrFileSystem,irrDriver); room513::setBase(rme);
	//EZ
	/*room860*/rme = loadRMesh(std::string("GFX/map/room860_opt.rmesh"),irrFileSystem,irrDriver); room860::setBase(rme);
	/*exit1*/rme = loadRMesh(std::string("GFX/map/exit1_opt.rmesh"),irrFileSystem,irrDriver); exit1::setBase(rme);
	/*gateaentrance*/rme = loadRMesh(std::string("GFX/map/gateaentrance_opt.rmesh"),irrFileSystem,irrDriver); gateaentrance::setBase(rme);
	/*lockroom2*/rme = loadRMesh(std::string("GFX/map/lockroom2_opt.rmesh"),irrFileSystem,irrDriver); lockroom2::setBase(rme);
	/*room079*/rme = loadRMesh(std::string("GFX/map/room079_opt.rmesh"),irrFileSystem,irrDriver); room079::setBase(rme);
	/*room2z3*/rme = loadRMesh(std::string("GFX/map/room2z3_opt.rmesh"),irrFileSystem,irrDriver); room2z3::setBase(rme);
	/*room2cafeteria*/rme = loadRMesh(std::string("GFX/map/room2cafeteria_opt.rmesh"),irrFileSystem,irrDriver); room2cafeteria::setBase(rme);
	/*room2cz3*/rme = loadRMesh(std::string("GFX/map/room2Cz3_opt.rmesh"),irrFileSystem,irrDriver); room2cz3::setBase(rme);
	/*room2ccont*/rme = loadRMesh(std::string("GFX/map/room2ccont_opt.rmesh"),irrFileSystem,irrDriver); room2ccont::setBase(rme);
	/*room2offices*/rme = loadRMesh(std::string("GFX/map/room2offices_opt.rmesh"),irrFileSystem,irrDriver); room2offices::setBase(rme);
	/*room2offices2*/rme = loadRMesh(std::string("GFX/map/room2offices2_opt.rmesh"),irrFileSystem,irrDriver); room2offices2::setBase(rme);
	/*room2offices3*/rme = loadRMesh(std::string("GFX/map/room2offices3_opt.rmesh"),irrFileSystem,irrDriver); room2offices3::setBase(rme);
	/*room2poffices*/rme = loadRMesh(std::string("GFX/map/room2poffices_opt.rmesh"),irrFileSystem,irrDriver); room2poffices::setBase(rme);
	/*room2poffices2*/rme = loadRMesh(std::string("GFX/map/room2poffices2_opt.rmesh"),irrFileSystem,irrDriver); room2poffices2::setBase(rme);
	/*room2sroom*/rme = loadRMesh(std::string("GFX/map/room2sroom_opt.rmesh"),irrFileSystem,irrDriver); room2sroom::setBase(rme);
	/*room2toilets*/rme = loadRMesh(std::string("GFX/map/room2toilets_opt.rmesh"),irrFileSystem,irrDriver); room2toilets::setBase(rme);
	/*room2tesla*/rme = loadRMesh(std::string("GFX/map/room2tesla_opt.rmesh"),irrFileSystem,irrDriver); room2tesla::setBase(rme);
	/*room3servers*/rme = loadRMesh(std::string("GFX/map/room3servers_opt.rmesh"),irrFileSystem,irrDriver); room3servers::setBase(rme);
	/*room3servers2*/rme = loadRMesh(std::string("GFX/map/room3servers2_opt.rmesh"),irrFileSystem,irrDriver); room3servers2::setBase(rme);
	/*room3z3*/rme = loadRMesh(std::string("GFX/map/room3z3_opt.rmesh"),irrFileSystem,irrDriver); room3z3::setBase(rme);
	/*room4z3*/rme = loadRMesh(std::string("GFX/map/room4z3_opt.rmesh"),irrFileSystem,irrDriver); room4z3::setBase(rme);
	//Misc
	/*r_173*/rme = loadRMesh(std::string("GFX/map/173bright_opt.rmesh"),irrFileSystem,irrDriver); r_173::setBase(rme);
	/*checkpoint1*/rme = loadRMesh(std::string("GFX/map/checkpoint1_opt.rmesh"),irrFileSystem,irrDriver); checkpoint1::setBase(rme);
	/*checkpoint2*/rme = loadRMesh(std::string("GFX/map/checkpoint2_opt.rmesh"),irrFileSystem,irrDriver); checkpoint2::setBase(rme);
	/*gatea*/rme = loadRMesh(std::string("GFX/map/gatea_opt.rmesh"),irrFileSystem,irrDriver); gatea::setBase(rme);
	/*pocketdimension*/rme = loadRMesh(std::string("GFX/map/pocketdimension1_opt.rmesh"),irrFileSystem,irrDriver); pocketdimension::setBase(rme);

	mainPlayer = new player(this,irrSmgr,dynamics,irrReceiver);

	mainPlayer->update();

	createMap();
	/*room2::createNew(irr::core::vector3df(0,0,0),0);
	room2c::createNew(irr::core::vector3df(0,0,204.8f*RoomScale),1);
	room3::createNew(irr::core::vector3df(0,0,-204.8f*RoomScale),2);
	room4::createNew(irr::core::vector3df(-204.8f*RoomScale,0,-204.8f*RoomScale),2);
	room1::createNew(irr::core::vector3df(204.8f*RoomScale,0,-204.8f*RoomScale),1);*/

    //node->setPosition(irr::core::vector3df(0,0*RoomScale,0));

    //dynamics->addTriMesh_static(node);

	irrDevice->getCursorControl()->setVisible(false);
	irrDevice->getCursorControl()->setPosition((irr::s32)mainWidth/2,(irr::s32)mainHeight/2);

	for (int y=19;y>=0;y--) {
		for (int x=19;x>=0;x--) {
			if (roomArray[x][y]!=nullptr) {
				//mainPlayer->teleport(irr::core::vector3df(x*204.8f*RoomScale,10.f,y*204.8f*RoomScale));
				//std::cout<<"Placed player at coords ["<<x<<"]["<<y<<"]\n";

				//test node
				irr::scene::IMesh* mesh1 = irrSmgr->getMesh("test/173_2.b3d");

				node = irrSmgr->addMeshSceneNode(mesh1);

				node->setScale(irr::core::vector3df(1.3*RoomScale));
				node->setPosition(irr::core::vector3df(x*204.8f*RoomScale,10*RoomScale,y*204.8f*RoomScale));
				node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
				node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);

				node->getMaterial(0).Lighting = true;
				node->getMaterial(0).MaterialType = (irr::video::E_MATERIAL_TYPE)NormalsShader;

				rbody = dynamics->addTriMesh_moving(node,16000.f,20,1,1);
				rbody->setAngularFactor(btVector3(0,0,0));
				//rbody->setLinearFactor(btVector3(0.1,0.1,0.1));

				node->getMaterial(0).setTexture(1, irrDriver->getTexture("test/173_norm.jpg"));
				node->getMaterial(0).setTexture(2, irrDriver->getTexture("test/173_Spec.jpg"));

				node->getMaterial(0).EmissiveColor = irr::video::SColor(100,100,100,100);

				mainPlayer->testNode = node;
				//------------

				mesh1 = irrSmgr->getMesh("test/scp-066.b3d");

				node = irrSmgr->addMeshSceneNode(mesh1);


				node->setScale(irr::core::vector3df(2.1*RoomScale));
				node->setPosition(irr::core::vector3df(x*204.8f*RoomScale,10*RoomScale,y*204.8f*RoomScale));

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

				y=-1;

				break;
			}
		}
	}
}

world::~world() {
    //fileSystem->drop();
    //env->drop();
    //smgr->drop();
    //driver->drop();
    irrDevice->drop();
    sound::killSounds();
}

bool world::run() {

	std::cout<<irrReceiver->IsMouseDown(0)<<"\n";

	if (prevTime==0) { FPSfactor = 1.0; } else {
		FPSfactor = (irrTimer->getRealTime()-prevTime)/(1000.0/70.0);
	}
	prevTime = irrTimer->getRealTime();

	if (menusOpen==0) {

		float prec = 0.75f;

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
		//std::cout<<mainPlayer->seesMeshNode(mainPlayer->testNode)<<"\n";

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

		if (irrReceiver->IsKeyDown(irr::KEY_TAB) && !irrReceiver->IsPrevKeyDown(irr::KEY_TAB)) {
			menusOpen = INVOPEN;
			for (unsigned char i=0;i<inventory_size;i++) {
				invImgs[i]=nullptr;
				std::string imgpath = mainPlayer->getItemInvImg(i);
				if (imgpath!="") {
					invImgs[i]=irrDriver->getTexture(imgpath.c_str());
				}
			}
			irrDevice->getCursorControl()->setVisible(true);
		}
	} else {
		FPSfactor = 0.f;

		dynamics->resetTimer(irrTimer->getRealTime());
		if (menusOpen==INVOPEN) {
			if (irrReceiver->IsKeyDown(irr::KEY_TAB) && !irrReceiver->IsPrevKeyDown(irr::KEY_TAB)) {
				menusOpen = 0;
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
		}
	}

    irrDriver->beginScene(true, true, irr::video::SColor(255, 255, 0, 255));

	draw3D();
	drawHUD();

    irrDriver->endScene();

	if (irrTimer->getRealTime()-prevTime<17) irrDevice->sleep(17-(irrTimer->getRealTime()-prevTime));

	sound::processDrops();
	irrReceiver->perLoopUpdate();
    return irrDevice->run();
}

void world::draw3D() {
	irrDriver->setRenderTarget(blurImage2); //copy the old render
    irrDriver->draw2DImage(blurImage,irr::core::position2d<irr::s32>(0,0),irr::core::rect<irr::s32>(0,0,mainWidth,mainHeight), 0,irr::video::SColor(255,255,255,255), false);
    irrDriver->setRenderTarget(blurImage); //create a new render, using the old one to add a blur effect
    irrSmgr->drawAll();
}

void world::drawHUD() {
	float BlinkTimer = mainPlayer->BlinkTimer;
    if (BlinkTimer<0) {
        float darkA;
        if (mainPlayer->BlinkTimer>=-0.5f) {
			darkA = std::max(0.f,std::min(-mainPlayer->BlinkTimer*2.f,1.f));
        } else if (mainPlayer->BlinkTimer>=-1.0f) {
            darkA = std::max(0.f,std::min(1.f+(mainPlayer->BlinkTimer+0.5f)*2.f,1.f));
        }
        darkA = 1.f-darkA;
        darkA *= darkA;
        darkA = 1.f-darkA;
        irrDriver->draw2DRectangle(irr::video::SColor(std::min(255.f,darkA*255.f),0,0,0),irr::core::rect<irr::s32>(0,0,mainWidth,mainHeight));
    }
    irrDriver->draw2DImage(blurImage2,irr::core::position2d<irr::s32>(0,0),irr::core::rect<irr::s32>(0,0,mainWidth,mainHeight), 0,irr::video::SColor(std::min(blurAlpha/FPSfactor,200.0f),255,255,255), false);
    irrDriver->setRenderTarget(0); //draw to screen
    irrDriver->clearZBuffer();
    irrDriver->draw2DImage(blurImage,irr::core::position2d<irr::s32>(0,0),irr::core::rect<irr::s32>(0,0,mainWidth,mainHeight), 0,irr::video::SColor(255,255,255,255), false);
	std::string fpsStr;
	fpsStr = "FPS: ";
	fpsStr += std::to_string(irrDriver->getFPS());
	font1->draw(fpsStr.c_str(),irr::core::rect<irr::s32>(mainWidth/2+2,mainHeight+2,mainWidth/2+2,mainHeight/2+102),irr::video::SColor(150,0,0,0),true,true);
	font1->draw(fpsStr.c_str(),irr::core::rect<irr::s32>(mainWidth/2,mainHeight,mainWidth/2,mainHeight/2+100),irr::video::SColor(255,255,255,255),true,true);

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

    blurAlpha = 100;

    if (menusOpen==INVOPEN) {
		for (unsigned char i=0;i<inventory_size;i++) {
			int x,y,w,h;
			x = mainWidth/2-(90*inventory_size/4)+(i%5*90);
			y = mainHeight/2-(90*inventory_size/10)+(i/5*90);
			w = 64;
			h = 64;
			irrDriver->draw2DRectangle(irr::video::SColor(255,255,255,255),irr::core::recti(irr::core::position2di(x-2,y-2),irr::core::position2di(x+w+2,y+h+2)));
			if (invImgs[i]!=nullptr && i!=dragItem) {
				irrDriver->draw2DImage(invImgs[i],irr::core::position2di(x,y),irr::core::rect<irr::s32>(0,0,w,h));
				if (irrReceiver->getMousePos().X>x && irrReceiver->getMousePos().X<x+w && irrReceiver->getMousePos().Y>y && irrReceiver->getMousePos().Y<y+h) {
					font1->draw(mainPlayer->getItemName(i).c_str(),irr::core::recti(irr::core::position2di(x+2,y+h+8),irr::core::position2di(x+w+2,y+h+24)),irr::video::SColor(100,0,0,0),true,true);
					font1->draw(mainPlayer->getItemName(i).c_str(),irr::core::recti(irr::core::position2di(x,y+h+6),irr::core::position2di(x+w,y+h+22)),irr::video::SColor(255,255,255,255),true,true);
					if (irrReceiver->IsDoubleClick(0)) {
						menusOpen=0;
					} else if (irrReceiver->IsMouseDown(0) && dragItem>=inventory_size) {
						dragItem = i;
					}
				}
			} else {
				irrDriver->draw2DRectangle(irr::video::SColor(255,0,0,0),irr::core::recti(irr::core::position2di(x,y),irr::core::position2di(x+w,y+h)));
			}
		}
		if (dragItem<inventory_size) {
			if (invImgs[dragItem]==nullptr) {
				dragItem = inventory_size;
			} else {
				int x,y,w,h;
				unsigned int dist = 100;
				unsigned char targetSlot = inventory_size;
				for (unsigned char i=0;i<inventory_size;i++) {
					x = mainWidth/2-(90*inventory_size/4)+(i%5*90);
					y = mainHeight/2-(90*inventory_size/10)+(i/5*90);
					w = 64;
					h = 64;

					int xd,yd;
					xd = x+w/2-irrReceiver->getMousePos().X;
					yd = y+h/2-irrReceiver->getMousePos().Y;

					if ((irr::u32)irr::core::squareroot(xd*xd+yd*yd)<dist) {
						std::cout<<irr::core::squareroot(xd*xd+yd*yd)<<"\n";
						dist = irr::core::squareroot(xd*xd+yd*yd);
						targetSlot = i;
					}
				}
				if (targetSlot<inventory_size) {
					x = mainWidth/2-(90*inventory_size/4)+(targetSlot%5*90);
					y = mainHeight/2-(90*inventory_size/10)+(targetSlot/5*90);
					w = 64;
					h = 64;
					irrDriver->draw2DImage(invImgs[dragItem],irr::core::position2di(x,y),irr::core::rect<irr::s32>(0,0,w,h),nullptr,irr::video::SColor(100,255,255,255));
				}
				x = irrReceiver->getMousePos().X-32;
				y = irrReceiver->getMousePos().Y-32;
				irrDriver->draw2DImage(invImgs[dragItem],irr::core::position2di(x,y),irr::core::rect<irr::s32>(0,0,w,h));
				if (!irrReceiver->IsMouseDown(0)) {
					if (targetSlot<inventory_size) {
						if (mainPlayer->moveToSlot(dragItem,targetSlot)==1) {
							invImgs[targetSlot] = invImgs[dragItem];
							invImgs[dragItem] = nullptr;
						}
					} else {
						mainPlayer->takeFromInventory(dragItem);
						menusOpen=0;
					}
					dragItem = inventory_size;
				}
			}
		}
		if (menusOpen==0) {
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
	}
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
