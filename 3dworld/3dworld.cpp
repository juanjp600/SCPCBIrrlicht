#include "3dworld.h"
//#include "rooms/rmesh.h"
#include "rooms/room.h"

#include <iostream>
#include <cmath>
#include <cstdlib>

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

	//irrSmgr->setAmbientLight(irr::video::SColor(255,20,20,20));

    irrDriver->setTextureCreationFlag(irr::video::ETCF_ALWAYS_32_BIT, true);

    irrEnv = irrDevice->getGUIEnvironment();

    irrFileSystem = irrDevice->getFileSystem();

    irrTimer = irrDevice->getTimer();

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

    node = irrSmgr->addMeshSceneNode(irrSmgr->getMesh("test/eyedrops.b3d"));
    node->setMaterialType((irr::video::E_MATERIAL_TYPE)LightsShader);

	item::setDynamics(itemDyn);

    node->setScale(irr::core::vector3df(0.06*RoomScale,0.06*RoomScale,0.06*RoomScale));
    node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
    itemEyedrops::setMeshNode(node);

    node = irrSmgr->addMeshSceneNode(irrSmgr->getMesh("test/gasmask.b3d"));
    //node->setDebugDataVisible(irr::scene::EDS_NORMALS);
    node->setMaterialType((irr::video::E_MATERIAL_TYPE)LightsShader);

    node->setScale(irr::core::vector3df(0.6*RoomScale,0.6*RoomScale,0.6*RoomScale));
    node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
    itemGasMask::setMeshNode(node);

    for (irr::u32 ui=0;ui<10000;ui++) {
        item* it = itemEyedrops::createItemEyedrops();
        itemList.push_back(it);

        it = itemGasMask::createItemGasMask();
        itemList.push_back(it);
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

	ambient[0] = sound::getSound(std::string("test/The Dread.ogg"),false);
	if (ambient[0]!=nullptr) {
		ambient[0]->playSound(true);
	}

	RMesh* rme;
	//LCZ
	/*lockroom*/rme = loadRMesh(std::string("test/lockroom_opt.rmesh"),irrFileSystem,irrDriver); lockroom::setBase(rme);
	/*start*/rme = loadRMesh(std::string("test/173_opt.rmesh"),irrFileSystem,irrDriver); start::setBase(rme);
	/*room2storage*/rme = loadRMesh(std::string("test/room2storage_opt.rmesh"),irrFileSystem,irrDriver); room2storage::setBase(rme);
	/*room3storage*/rme = loadRMesh(std::string("test/room3storage_opt.rmesh"),irrFileSystem,irrDriver); room3storage::setBase(rme);
	/*endroom*/rme = loadRMesh(std::string("test/endroom_opt.rmesh"),irrFileSystem,irrDriver); endroom::setBase(rme);
	/*room012*/rme = loadRMesh(std::string("test/room012_opt.rmesh"),irrFileSystem,irrDriver); room012::setBase(rme);
	/*room2*/rme = loadRMesh(std::string("test/room2_opt.rmesh"),irrFileSystem,irrDriver); room2::setBase(rme);
	/*room2_2*/rme = loadRMesh(std::string("test/room2_2_opt.rmesh"),irrFileSystem,irrDriver); room2_2::setBase(rme);
	/*room2c*/rme = loadRMesh(std::string("test/room2C_opt.rmesh"),irrFileSystem,irrDriver); room2c::setBase(rme);
	/*room2closets*/rme = loadRMesh(std::string("test/room2closets_opt.rmesh"),irrFileSystem,irrDriver); room2closets::setBase(rme);
	/*room2elevator*/rme = loadRMesh(std::string("test/room2elevator_opt.rmesh"),irrFileSystem,irrDriver); room2elevator::setBase(rme);
	/*room2doors*/rme = loadRMesh(std::string("test/room2doors_opt.rmesh"),irrFileSystem,irrDriver); room2doors::setBase(rme);
	/*room2scps*/rme = loadRMesh(std::string("test/room2scps_opt.rmesh"),irrFileSystem,irrDriver); room2scps::setBase(rme);
	/*room3storage*/rme = loadRMesh(std::string("test/room3storage_opt.rmesh"),irrFileSystem,irrDriver); room3storage::setBase(rme);
	/*room2testroom2*/rme = loadRMesh(std::string("test/room2testroom2_opt.rmesh"),irrFileSystem,irrDriver); room2testroom2::setBase(rme);
	/*room3*/rme = loadRMesh(std::string("test/room3_opt.rmesh"),irrFileSystem,irrDriver); room3::setBase(rme);
	/*room3_2*/rme = loadRMesh(std::string("test/room3_2_opt.rmesh"),irrFileSystem,irrDriver); room3_2::setBase(rme);
	/*room4*/rme = loadRMesh(std::string("test/room4_opt.rmesh"),irrFileSystem,irrDriver); room4::setBase(rme);
	/*roompj*/rme = loadRMesh(std::string("test/roompj_opt.rmesh"),irrFileSystem,irrDriver); roompj::setBase(rme);
	/*r_914*/rme = loadRMesh(std::string("test/machineroom_opt.rmesh"),irrFileSystem,irrDriver); r_914::setBase(rme);
	//HCZ
	/*r_008*/rme = loadRMesh(std::string("test/008_opt.rmesh"),irrFileSystem,irrDriver); r_008::setBase(rme);
	/*coffin*/rme = loadRMesh(std::string("test/coffin_opt.rmesh"),irrFileSystem,irrDriver); coffin::setBase(rme);
	/*endroom2*/rme = loadRMesh(std::string("test/endroom2_opt.rmesh"),irrFileSystem,irrDriver); endroom2::setBase(rme);
	/*testroom*/rme = loadRMesh(std::string("test/testroom_opt.rmesh"),irrFileSystem,irrDriver); testroom::setBase(rme);
	/*tunnel*/rme = loadRMesh(std::string("test/tunnel_opt.rmesh"),irrFileSystem,irrDriver); tunnel::setBase(rme);
	/*tunnel2*/rme = loadRMesh(std::string("test/tunnel2_opt.rmesh"),irrFileSystem,irrDriver); tunnel2::setBase(rme);
	/*room035*/rme = loadRMesh(std::string("test/room035_opt.rmesh"),irrFileSystem,irrDriver); room035::setBase(rme);
	/*room049*/rme = loadRMesh(std::string("test/room049_opt.rmesh"),irrFileSystem,irrDriver); room049::setBase(rme);
	/*room106*/rme = loadRMesh(std::string("test/room106_opt.rmesh"),irrFileSystem,irrDriver); room106::setBase(rme);
	/*room2ctunnel*/rme = loadRMesh(std::string("test/room2Ctunnel_opt.rmesh"),irrFileSystem,irrDriver); room2ctunnel::setBase(rme);
	/*room2nuke*/rme = loadRMesh(std::string("test/room2nuke_opt.rmesh"),irrFileSystem,irrDriver); room2nuke::setBase(rme);
	/*room2pipes*/rme = loadRMesh(std::string("test/room2pipes_opt.rmesh"),irrFileSystem,irrDriver); room2pipes::setBase(rme);
	/*room2pit*/rme = loadRMesh(std::string("test/room2pit_opt.rmesh"),irrFileSystem,irrDriver); room2pit::setBase(rme);
	/*room3pit*/rme = loadRMesh(std::string("test/room3pit_opt.rmesh"),irrFileSystem,irrDriver); room3pit::setBase(rme);
	/*room2servers*/rme = loadRMesh(std::string("test/room2servers_opt.rmesh"),irrFileSystem,irrDriver); room2servers::setBase(rme);
	/*room2tunnel*/rme = loadRMesh(std::string("test/room2tunnel_opt.rmesh"),irrFileSystem,irrDriver); room2tunnel::setBase(rme);
	/*room3tunnel*/rme = loadRMesh(std::string("test/room3tunnel_opt.rmesh"),irrFileSystem,irrDriver); room3tunnel::setBase(rme);
	/*room4tunnels*/rme = loadRMesh(std::string("test/4tunnels_opt.rmesh"),irrFileSystem,irrDriver); room4tunnels::setBase(rme);
	/*room513*/rme = loadRMesh(std::string("test/room513_opt.rmesh"),irrFileSystem,irrDriver); room513::setBase(rme);
	//EZ
	/*room860*/rme = loadRMesh(std::string("test/room860_opt.rmesh"),irrFileSystem,irrDriver); room860::setBase(rme);
	/*exit1*/rme = loadRMesh(std::string("test/exit1_opt.rmesh"),irrFileSystem,irrDriver); exit1::setBase(rme);
	/*gateaentrance*/rme = loadRMesh(std::string("test/gateaentrance_opt.rmesh"),irrFileSystem,irrDriver); gateaentrance::setBase(rme);
	/*lockroom2*/rme = loadRMesh(std::string("test/lockroom2_opt.rmesh"),irrFileSystem,irrDriver); lockroom2::setBase(rme);
	/*room079*/rme = loadRMesh(std::string("test/room079_opt.rmesh"),irrFileSystem,irrDriver); room079::setBase(rme);
	/*room2z3*/rme = loadRMesh(std::string("test/room2z3_opt.rmesh"),irrFileSystem,irrDriver); room2z3::setBase(rme);
	/*room2cafeteria*/rme = loadRMesh(std::string("test/room2cafeteria_opt.rmesh"),irrFileSystem,irrDriver); room2cafeteria::setBase(rme);
	/*room2cz3*/rme = loadRMesh(std::string("test/room2Cz3_opt.rmesh"),irrFileSystem,irrDriver); room2cz3::setBase(rme);
	/*room2ccont*/rme = loadRMesh(std::string("test/room2ccont_opt.rmesh"),irrFileSystem,irrDriver); room2ccont::setBase(rme);
	/*room2offices*/rme = loadRMesh(std::string("test/room2offices_opt.rmesh"),irrFileSystem,irrDriver); room2offices::setBase(rme);
	/*room2offices2*/rme = loadRMesh(std::string("test/room2offices2_opt.rmesh"),irrFileSystem,irrDriver); room2offices2::setBase(rme);
	/*room2offices3*/rme = loadRMesh(std::string("test/room2offices3_opt.rmesh"),irrFileSystem,irrDriver); room2offices3::setBase(rme);
	/*room2poffices*/rme = loadRMesh(std::string("test/room2poffices_opt.rmesh"),irrFileSystem,irrDriver); room2poffices::setBase(rme);
	/*room2poffices2*/rme = loadRMesh(std::string("test/room2poffices2_opt.rmesh"),irrFileSystem,irrDriver); room2poffices2::setBase(rme);
	/*room2sroom*/rme = loadRMesh(std::string("test/room2sroom_opt.rmesh"),irrFileSystem,irrDriver); room2sroom::setBase(rme);
	/*room2toilets*/rme = loadRMesh(std::string("test/room2toilets_opt.rmesh"),irrFileSystem,irrDriver); room2toilets::setBase(rme);
	/*room2tesla*/rme = loadRMesh(std::string("test/room2tesla_opt.rmesh"),irrFileSystem,irrDriver); room2tesla::setBase(rme);
	/*room3servers*/rme = loadRMesh(std::string("test/room3servers_opt.rmesh"),irrFileSystem,irrDriver); room3servers::setBase(rme);
	/*room3servers2*/rme = loadRMesh(std::string("test/room3servers2_opt.rmesh"),irrFileSystem,irrDriver); room3servers2::setBase(rme);
	/*room3z3*/rme = loadRMesh(std::string("test/room3z3_opt.rmesh"),irrFileSystem,irrDriver); room3z3::setBase(rme);
	/*room4z3*/rme = loadRMesh(std::string("test/room4z3_opt.rmesh"),irrFileSystem,irrDriver); room4z3::setBase(rme);
	//Misc
	/*r_173*/rme = loadRMesh(std::string("test/173bright_opt.rmesh"),irrFileSystem,irrDriver); r_173::setBase(rme);
	/*checkpoint1*/rme = loadRMesh(std::string("test/checkpoint1_opt.rmesh"),irrFileSystem,irrDriver); checkpoint1::setBase(rme);
	/*checkpoint2*/rme = loadRMesh(std::string("test/checkpoint2_opt.rmesh"),irrFileSystem,irrDriver); checkpoint2::setBase(rme);
	/*gatea*/rme = loadRMesh(std::string("test/gatea_opt.rmesh"),irrFileSystem,irrDriver); gatea::setBase(rme);
	/*pocketdimension*/rme = loadRMesh(std::string("test/pocketdimension1_opt.rmesh"),irrFileSystem,irrDriver); pocketdimension::setBase(rme);

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
}

room* world::addRandomRoom(unsigned short x,unsigned short y,roomTypes type,char angle,int zone) {
	int choice = 0;
	room* retRoom = nullptr;
	switch (type) {
		case roomTypes::ROOM1:
			switch (zone) {
				case 0: //LCZ
					choice = rand()%100;
					if (choice>=0 && choice<100) {
						retRoom = endroom::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
				case 1: //HCZ
					choice = rand()%100;
					if (choice>=0 && choice<100) {
						retRoom = endroom2::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
				case 2: //EZ
					choice = rand()%100;
					if (choice>=0 && choice<100) {
						retRoom = endroom::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
			}
		break;
		case roomTypes::ROOM2:
			switch (zone) {
				case 0: //LCZ
					choice = rand()%150;
					if (choice>=0 && choice<50) {
						retRoom = room2::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=50 && choice<100) {
						retRoom = room2_2::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=100 && choice<120) {
						retRoom = room2elevator::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=120 && choice<150) {
						retRoom = room2doors::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
				case 1: //HCZ
					choice = rand()%295;
					if (choice>=0 && choice<100) {
						retRoom = tunnel::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=100 && choice<170) {
						retRoom = tunnel2::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=170 && choice<220) {
						retRoom = room2pipes::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=220 && choice<295) {
						retRoom = room2pit::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
				case 2: //EZ
					choice = rand()%300;
					if (choice>=0 && choice<100) {
						retRoom = room2z3::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=100 && choice<130) {
						retRoom = room2offices::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=130 && choice<150) {
						retRoom = room2offices2::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=150 && choice<170) {
						retRoom = room2offices3::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=170 && choice<200) {
						retRoom = room2toilets::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=200 && choice<300) {
						retRoom = room2tesla::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
			}
		break;
		case roomTypes::ROOM2C:
			switch (zone) {
				case 0: //LCZ
					choice = rand()%70;
					if (choice>=0 && choice<30) {
						retRoom = lockroom::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=30 && choice<70) {
						retRoom = room2c::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
				case 1: //HCZ
					choice = rand()%40;
					if (choice>=0 && choice<40) {
						retRoom = room2ctunnel::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
				case 2: //EZ
					choice = rand()%100;
					if (choice>=0 && choice<100) {
						retRoom = room2cz3::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
			}
		break;
		case roomTypes::ROOM3:
			switch (zone) {
				case 0: //LCZ
					choice = rand()%200;
					if (choice>=0 && choice<100) {
						retRoom = room3::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=100 && choice<200) {
						retRoom = room3_2::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
				case 1: //HCZ
					choice = rand()%200;
					if (choice>=0 && choice<100) {
						retRoom = room3pit::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
					if (choice>=100 && choice<200) {
						retRoom = room3tunnel::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
				case 2: //EZ
					choice = rand()%100;
					if (choice>=0 && choice<100) {
						retRoom = room3z3::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
			}
		break;
		case roomTypes::ROOM4:
			switch (zone) {
				case 0: //LCZ
					choice = rand()%100;
					if (choice>=0 && choice<100) {
						retRoom = room4::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
				case 1: //HCZ
					choice = rand()%100;
					if (choice>=0 && choice<100) {
						retRoom = room4tunnels::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
				case 2: //EZ
					choice = rand()%100;
					if (choice>=0 && choice<100) {
						retRoom = room4z3::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),angle);
					}
				break;
			}
		break;
	}
	roomArray[x][y] = retRoom;
	return retRoom;
}

void world::createMap() {
	auto getZone = [] (short y) -> int { return std::max(std::min(std::floor((float)(20-y)/20.f*3.f),2.f),0.f); };

	short x,y,temp;
	short x2,y2;
	short width,height;

	struct tempRoom {
		roomTypes type;
		char angle;
	};

	tempRoom roomTemp[20][20];
	for (x=0;x<20;x++) {
		for (y=0;y<20;y++) {
			roomArray[x][y] = nullptr;
			roomTemp[x][y].type = roomTypes::ROOM1;
			roomTemp[x][y].angle = -1;
		}
	}

	x = 10;
	y = 18;

	for (int i = y;i<20;i++) {
		roomTemp[x][i].angle = 0;
	}

	while (y>=2) {
		width = (rand() % 6) + 10;

		if (x>12) {
			width = -width;
		} else if (x>8) {
			x = x-10;
		}

		//make sure the hallway doesn't go outside the array
		if (x+width > 17) {
			width=17-x;
		} else if (x+width < 2) {
			width=-x+2;
		}


		x = std::min(x,short(x + width));
		width = std::abs(width);
		for (int i = x;i<=x+width;i++) {
			roomTemp[std::min(i,19)][y].angle = 0;
		}

		height = (rand() % 2) + 3;
		if (y - height < 1) height = y;

		int yhallways = (rand() % 2) + 4;

		if (getZone(y-height)!=getZone(y-height+1)) height--;

		for (int i=1;i<=yhallways;i++) {
			x2 = std::max(std::min((rand() % (width-1)) + x,18),2);
			while (roomTemp[x2][y-1].angle>=0 || roomTemp[x2-1][y-1].angle>=0 || roomTemp[x2+1][y-1].angle>=0) {
				x2++;
				if (x2>18) break;
			}

			if (x2<x+width) {
				short tempheight;
				if (i==1) {
					tempheight = height;
					if (rand()%2 == 0) x2 = x; else x2 = x+width;
				} else {
					tempheight = (rand()%height) + 1;
				}

				for (y2 = y - tempheight;y2<=y;y2++) {
					if (getZone(y2)!=getZone(y2+1)) { //a room leading from zone to another
						roomTemp[x2][y2].angle = 127;
					} else {
						roomTemp[x2][y2].angle = 0;
					}
				}

				if (tempheight == height) temp = x2;
			}
		}

		x = temp;
		y -= height;
	}

	unsigned short room1amount[3];
	unsigned short room2amount[3];
	unsigned short room2camount[3];
	unsigned short room3amount[3];
	unsigned short room4amount[3];

	for (int i=0;i<3;i++) {
		room1amount[i]=0;
		room2amount[i]=0;
		room2camount[i]=0;
		room3amount[i]=0;
		room4amount[i]=0;
	}

	for (x=0;x<20;x++) {
		for (y=0;y<20;y++) {
			bool hasNorth,hasSouth,hasEast,hasWest;
			hasNorth = hasSouth = hasEast = hasWest = false;
			if (roomTemp[x][y].angle==127) {
				//get rid of this checkpoint room if it leads to nothing
				if (roomTemp[x][y-1].angle<0) roomTemp[x][y].angle = -1;
			} else if (roomTemp[x][y].angle==0) { //this is not a checkpoint room
				if (x>0) {
					hasWest = (roomTemp[x-1][y].angle>-1);
				}
				if (x<19) {
					hasEast = (roomTemp[x+1][y].angle>-1);
				}
				if (y>0) {
					hasNorth = (roomTemp[x][y-1].angle>-1);
				}
				if (y<19) {
					hasSouth = (roomTemp[x][y+1].angle>-1);
				}
				if (hasNorth && hasSouth) {
					if (hasEast && hasWest) { //room4
						roomTemp[x][y].type = roomTypes::ROOM4;
						roomTemp[x][y].angle = rand()%4;
					} else if (hasEast && !hasWest) { //room3, pointing east
						roomTemp[x][y].type = roomTypes::ROOM3;
						roomTemp[x][y].angle = 3;
					} else if (!hasEast && hasWest) { //room3, pointing west
						roomTemp[x][y].type = roomTypes::ROOM3;
						roomTemp[x][y].angle = 1;
					} else { //vertical room2
						roomTemp[x][y].type = roomTypes::ROOM2;
						roomTemp[x][y].angle = (rand()%2)*2;
					}
				} else if (hasEast && hasWest) {
					if (hasNorth && !hasSouth) { //room3, pointing north
						roomTemp[x][y].type = roomTypes::ROOM3;
						roomTemp[x][y].angle = 0;
					} else if (!hasNorth && hasSouth) { //room3, pointing south
						roomTemp[x][y].type = roomTypes::ROOM3;
						roomTemp[x][y].angle = 2;
					} else { //horizontal room2
						roomTemp[x][y].type = roomTypes::ROOM2;
						roomTemp[x][y].angle = ((rand()%2)*2)+1;
					}
				} else if (hasNorth) {
					if (hasEast) { //room2c, north-east
						roomTemp[x][y].type = roomTypes::ROOM2C;
						roomTemp[x][y].angle = 0;
					} else if (hasWest) { //room2c, north-west
						roomTemp[x][y].type = roomTypes::ROOM2C;
						roomTemp[x][y].angle = 1;
					} else { //room1, north
						roomTemp[x][y].type = roomTypes::ROOM1;
						roomTemp[x][y].angle = 0;
					}
				} else if (hasSouth) {
					if (hasEast) { //room2c, south-east
						roomTemp[x][y].type = roomTypes::ROOM2C;
						roomTemp[x][y].angle = 3;
					} else if (hasWest) { //room2c, south-west
						roomTemp[x][y].type = roomTypes::ROOM2C;
						roomTemp[x][y].angle = 2;
					} else { //room1, south
						roomTemp[x][y].type = roomTypes::ROOM1;
						roomTemp[x][y].angle = 2;
					}
				} else if (hasEast) { //room1, east
					roomTemp[x][y].type = roomTypes::ROOM1;
					roomTemp[x][y].angle = 3;
				} else { //room1, west
					roomTemp[x][y].type = roomTypes::ROOM1;
					roomTemp[x][y].angle = 1;
				}
				switch (roomTemp[x][y].type) {
					case roomTypes::ROOM1:
						room1amount[getZone(y)]++;
					break;
					case roomTypes::ROOM2:
						room2amount[getZone(y)]++;
					break;
					case roomTypes::ROOM2C:
						room2camount[getZone(y)]++;
					break;
					case roomTypes::ROOM3:
						room3amount[getZone(y)]++;
					break;
					case roomTypes::ROOM4:
						room4amount[getZone(y)]++;
					break;
				}
			}
		}
	}

	//force some rooms in there
	for (int i = 0;i<3;i++) {
		if (room1amount[i]<5) {
			std::cout<<"Forcing some ROOM1s in zone "<<i<<"\n";
			for (y=2;y<19 && room1amount[i]<5;y++) {
				if (getZone(y+2) == i && getZone(y-2) == i) {
					for (x=2;x<19 && room1amount[i]<5;x++) {
						if (roomTemp[x][y].angle<0) {
							if (((roomTemp[x+1][y].angle>=0) != (roomTemp[x-1][y].angle>=0)) != ((roomTemp[x][y+1].angle>=0) != (roomTemp[x][y-1].angle>=0))) {
								tempRoom* adjRoom = nullptr;
								if (roomTemp[x+1][y].angle>=0) {
									adjRoom = &roomTemp[x+1][y];
									roomTemp[x][y].angle = 3;
								} else if (roomTemp[x-1][y].angle>=0) {
									adjRoom = &roomTemp[x-1][y];
									roomTemp[x][y].angle = 1;
								} else if (roomTemp[x][y+1].angle>=0) {
									adjRoom = &roomTemp[x][y+1];
									roomTemp[x][y].angle = 2;
								} else {
									adjRoom = &roomTemp[x][y-1];
									roomTemp[x][y].angle = 0;
								}

								switch (adjRoom->type) {
									case roomTypes::ROOM2:
										roomTemp[x][y].type = roomTypes::ROOM1;
										room1amount[i]++;
										room2amount[i]--;
										room3amount[i]++;
										adjRoom->type = roomTypes::ROOM3;
										switch (roomTemp[x][y].angle) {
											case 0:
												adjRoom->angle = 2;
											break;
											case 1:
												adjRoom->angle = 3;
											break;
											case 2:
												adjRoom->angle = 0;
											break;
											case 3:
												adjRoom->angle = 1;
											break;
										}
									break;
									case roomTypes::ROOM3:
										roomTemp[x][y].type = roomTypes::ROOM1;
										adjRoom->type = roomTypes::ROOM4;
										room1amount[i]++;
										room3amount[i]--;
										room4amount[i]++;
									break;
									default:
										roomTemp[x][y].angle = -1;
									break;
								}
							}
						}
					}
				}
			}
		}
		if (room4amount[i]<3) {
			std::cout<<"Forcing some ROOM4s in zone "<<i<<"\n";
			for (y=2;y<19 && room4amount[i]<3;y++) {
				if (getZone(y+2) == i && getZone(y-2) == i) {
					for (x=2;x<19 && room4amount[i]<3;x++) {
						if (roomTemp[x][y].angle<0) {
							if (((roomTemp[x+1][y].angle>=0) != (roomTemp[x-1][y].angle>=0)) != ((roomTemp[x][y+1].angle>=0) != (roomTemp[x][y-1].angle>=0))) {
								tempRoom* adjRoom = nullptr;

								if (roomTemp[x+1][y].angle>=0) {
									adjRoom = &roomTemp[x+1][y];
									roomTemp[x][y].angle = 3;
								} else if (roomTemp[x-1][y].angle>=0) {
									adjRoom = &roomTemp[x-1][y];
									roomTemp[x][y].angle = 1;
								} else if (roomTemp[x][y+1].angle>=0) {
									adjRoom = &roomTemp[x][y+1];
									roomTemp[x][y].angle = 2;
								} else {
									adjRoom = &roomTemp[x][y-1];
									roomTemp[x][y].angle = 0;
								}

								switch (adjRoom->type) {
									case roomTypes::ROOM3:
										roomTemp[x][y].type = roomTypes::ROOM1;
										adjRoom->type = roomTypes::ROOM4;
										room1amount[i]++;
										room3amount[i]--;
										room4amount[i]++;
									break;
									default:
										roomTemp[x][y].angle = -1;
									break;
								}
							}
						}
					}
				}
			}
		}
		if (room2camount[i]<3) {
			std::cout<<"Forcing some ROOM2Cs in zone "<<i<<"\n";
			for (y=2;y<19 && room2camount[i]<3;y++) {
				if (getZone(y+2) == i && getZone(y-2) == i) {
					for (x=2;x<19 && room2camount[i]<3;x++) {
						if (roomTemp[x][y].angle<0) {
							if (((roomTemp[x+1][y].angle>=0) != (roomTemp[x-1][y].angle>=0)) != ((roomTemp[x][y+1].angle>=0) != (roomTemp[x][y-1].angle>=0))) {
								tempRoom* adjRoom = nullptr;

								if (roomTemp[x+1][y].angle>=0) {
									adjRoom = &roomTemp[x+1][y];
									roomTemp[x][y].angle = 3;
								} else if (roomTemp[x-1][y].angle>=0) {
									adjRoom = &roomTemp[x-1][y];
									roomTemp[x][y].angle = 1;
								} else if (roomTemp[x][y+1].angle>=0) {
									adjRoom = &roomTemp[x][y+1];
									roomTemp[x][y].angle = 2;
								} else {
									adjRoom = &roomTemp[x][y-1];
									roomTemp[x][y].angle = 0;
								}

								switch (adjRoom->type) {
									case roomTypes::ROOM1:
										roomTemp[x][y].type = roomTypes::ROOM1;
										adjRoom->type = roomTypes::ROOM2C;
										switch (roomTemp[x][y].angle) {
											case 0:
												if (adjRoom->angle == 1) adjRoom->angle = 2; //south-west
												else if (adjRoom->angle == 3) adjRoom->angle = 3; //south-east
												else { roomTemp[x][y].angle = -1; adjRoom->type = roomTypes::ROOM1; }
											break;
											case 1:
												if (adjRoom->angle == 0) adjRoom->angle = 1; //north-east
												else if (adjRoom->angle == 2) adjRoom->angle = 3; //south-east
												else { roomTemp[x][y].angle = -1; adjRoom->type = roomTypes::ROOM1; }
											break;
											case 2:
												if (adjRoom->angle == 1) adjRoom->angle = 1; //north-east
												else if (adjRoom->angle == 3) adjRoom->angle = 0; //north-west
												else { roomTemp[x][y].angle = -1; adjRoom->type = roomTypes::ROOM1; }
											break;
											case 3:
												if (adjRoom->angle == 0) adjRoom->angle = 0; //north-west
												else if (adjRoom->angle == 2) adjRoom->angle = 2; //south-west
												else { roomTemp[x][y].angle = -1; adjRoom->type = roomTypes::ROOM1; }
											break;
										}
										if (roomTemp[x][y].angle > -1) {
											room2camount[i]++;
										}
									break;
									default:
										roomTemp[x][y].angle = -1;
									break;
								}
							}
						}
					}
				}
			}
		}
	}

	for (x=0;x<20;x++) {
		for (y=0;y<20;y++) {
			std::cout<<(roomTemp[y][x].angle>-1);
		}
		std::cout<<"\n";
	}

	unsigned short currentRoom1,currentRoom2,currentRoom2c,currentRoom3,currentRoom4;
	currentRoom1 = currentRoom2 = currentRoom2c = currentRoom3 = currentRoom4 = 0;
	short prevZone = getZone(19);
	for (y=19;y>=0;y--) {
		if (getZone(y)!=prevZone) {
			currentRoom1 = currentRoom2 = currentRoom2c = currentRoom3 = currentRoom4 = 0;
		}
		for (x=19;x>=0;x--) {
			if (roomTemp[x][y].angle>-1 && roomTemp[x][y].angle<127) {
				switch (roomTemp[x][y].type) {
					case roomTypes::ROOM1:
						switch (getZone(y)) {
							case 0: //LCZ
								if (currentRoom1==0) {
									roomArray[x][y] = start::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
									mainPlayer->teleport(irr::core::vector3df(x*204.8f*RoomScale,10.f,y*204.8f*RoomScale));
								} else if (currentRoom1==(int)(0.4f*(float)room1amount[0])) {
									roomArray[x][y] = roompj::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom1==(int)(0.8f*(float)room1amount[0])) {
									roomArray[x][y] = r_914::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								}
							break;
							case 1: //HCZ
								if (currentRoom1==(int)(0.1f*(float)room1amount[1])) {
									roomArray[x][y] = room079::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom1==(int)(0.3f*(float)room1amount[1])) {
									roomArray[x][y] = room106::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
									//mainPlayer->teleport(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale));
								} else if (currentRoom1==(int)(0.5f*(float)room1amount[1])) {
									roomArray[x][y] = coffin::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom1==(int)(0.7f*(float)room1amount[1])) {
									roomArray[x][y] = room035::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom1==(int)(0.9f*(float)room1amount[1])) {
									roomArray[x][y] = r_008::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								}
							break;
							case 2: //EZ
								if (currentRoom1==room1amount[2]-1) {
									roomArray[x][y] = gateaentrance::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom1==room1amount[2]-2) {
									roomArray[x][y] = exit1::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								}
							break;
						}
						currentRoom1++;
					break;
					case roomTypes::ROOM2:
						switch (getZone(y)) {
							case 0: //LCZ
								if (currentRoom2==0) {
									roomArray[x][y] = room2closets::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.2f*(float)room2amount[0])) {
									roomArray[x][y] = room2testroom2::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.4f*(float)room2amount[0])) {
									roomArray[x][y] = room2scps::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.8f*(float)room2amount[0])) {
									roomArray[x][y] = room2storage::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.9f*(float)room2amount[0])) {
									roomArray[x][y] = room012::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								}
							break;
							case 1: //HCZ
								if (currentRoom2==(int)(0.2f*(float)room2amount[1])) {
									roomArray[x][y] = room2nuke::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.25f*(float)room2amount[1])) {
									roomArray[x][y] = room2tunnel::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.4f*(float)room2amount[1])) {
									roomArray[x][y] = room049::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.7f*(float)room2amount[1])) {
									roomArray[x][y] = room2servers::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.9f*(float)room2amount[1])) {
									roomArray[x][y] = testroom::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								}
							break;
							case 2: //EZ
								if (currentRoom2==(int)(0.1f*(float)room2amount[2])) {
									roomArray[x][y] = room2poffices::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.2f*(float)room2amount[2])) {
									roomArray[x][y] = room2cafeteria::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.3f*(float)room2amount[2])) {
									roomArray[x][y] = room2sroom::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.45f*(float)room2amount[2])) {
									roomArray[x][y] = room2offices::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.6f*(float)room2amount[2])) {
									roomArray[x][y] = room860::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.8f*(float)room2amount[2])) {
									roomArray[x][y] = room2poffices2::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2==(int)(0.9f*(float)room2amount[2])) {
									roomArray[x][y] = room2offices2::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								}
							break;
						}
						currentRoom2++;
					break;
					case roomTypes::ROOM2C:
						switch (getZone(y)) {
							case 0: //LCZ
								if (currentRoom2c==0) {
									roomArray[x][y] = lockroom::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								}
							break;
							case 1: //HCZ

							break;
							case 2: //EZ
								if (currentRoom2c==0) {
									roomArray[x][y] = room2ccont::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom2c==1) {
									roomArray[x][y] = lockroom2::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								}
							break;
						}
						currentRoom2c++;
					break;
					case roomTypes::ROOM3:
						switch (getZone(y)) {
							case 0: //LCZ
								if (currentRoom3==(int)((((rand()%1000)*0.001f)+0.2f)*(float)room3amount[0])) {
									roomArray[x][y] = room3storage::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								}
							break;
							case 1: //HCZ
								if (currentRoom3==(int)(0.5f*(float)room3amount[1])) {
									roomArray[x][y] = room513::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								}
							break;
							case 2: //EZ
								if (currentRoom3==(int)(0.3f*(float)room3amount[2])) {
									roomArray[x][y] = room3servers::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								} else if (currentRoom3==(int)(0.7f*(float)room3amount[2])) {
									roomArray[x][y] = room3servers2::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),roomTemp[x][y].angle);
								}
							break;
						}
						currentRoom3++;
					break;
					case roomTypes::ROOM4:
						switch (getZone(y)) {
							case 0: //LCZ

							break;
							case 1: //HCZ

							break;
							case 2: //EZ

							break;
						}
						currentRoom4++;
					break;
				}
				if (roomArray[x][y]==nullptr) {
					addRandomRoom(x,y,roomTemp[x][y].type,roomTemp[x][y].angle,getZone(y));
				}
			} else if (roomTemp[x][y].angle==127) {
				if (getZone(y)==1) {
					roomArray[x][y] = checkpoint1::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),0);
				} else {
					roomArray[x][y] = checkpoint2::createNew(irr::core::vector3df(x*204.8f*RoomScale,0,y*204.8f*RoomScale),0);
				}
			}
		}
		prevZone = getZone(y);
	}
}

bool world::run() {

    if (prevTime==0) { FPSfactor = 1.0; } else {
        FPSfactor = (irrTimer->getRealTime()-prevTime)/(1000.0/70.0);
    }
    prevTime = irrTimer->getRealTime();

    float prec = 0.75f;

    dynamics->simStep(irrTimer->getRealTime(),60.f * prec);

	mainPlayer->update();

	//std::cout<<hitTriangle.index<<"\n";

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

    if (irrReceiver->IsMouseDown(0)) {
		ambient[0]->pauseSound();
		//sound::freezeCategory(0);
        for (unsigned int i=0;i<itemList.size();i++) {
            if (itemList[i]->getPicked()) {
                itemList[i]->Unpick(mainPlayer->getPosition());
                break;
            }
        }
    }

    if (irrReceiver->IsMouseDown(1)) {
		ambient[0]->resumeSound();
		std::cout<<ambient[0]->isPlaying()<<"\n";
		//ambient[0]->playSound(true);
		//sound::unfreezeCategory(0);
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

	if (irrTimer->getRealTime()-prevTime<17) irrDevice->sleep(prevTime-irrTimer->getRealTime()+17);

	sound::processDrops();
    return irrDevice->run();
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

	for (unsigned char i=0;i<5;i++) {
		breathSound[i][0] = sound::getSound(std::string("test/breath")+std::to_string(i)+std::string(".ogg"),false);
		breathSound[i][1] = sound::getSound(std::string("test/breath")+std::to_string(i)+std::string("gas.ogg"),false);
	}

	for (unsigned char i=0;i<4;i++) {
		stepSound[0][0][i] = sound::getSound(std::string("test/Step")+std::to_string(i+1)+std::string(".ogg"),false);
		stepSound[0][1][i] = sound::getSound(std::string("test/Run")+std::to_string(i+1)+std::string(".ogg"),false);
		stepSound[1][0][i] = sound::getSound(std::string("test/StepMetal")+std::to_string(i+1)+std::string(".ogg"),false);
		stepSound[1][1][i] = sound::getSound(std::string("test/RunMetal")+std::to_string(i+1)+std::string(".ogg"),false);
		if (i<3) {
			stepSound[2][0][i] = sound::getSound(std::string("test/StepPD")+std::to_string(i+1)+std::string(".ogg"),false);
			stepSound[3][1][i] = sound::getSound(std::string("test/StepForest")+std::to_string(i+1)+std::string(".ogg"),false);
		}
	}

    // Add camera
    Camera = irrSmgr->addCameraSceneNode(0,irr::core::vector3df(0,0,0),irr::core::vector3df(0,0,-1));
    Camera->setPosition(irr::core::vector3df(15*204.8f*RoomScale, 10*RoomScale, 15*204.8f*RoomScale));
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

void player::teleport(irr::core::vector3df position) {
	btTransform oTrans = Capsule->getCenterOfMassTransform();
	oTrans.setOrigin(btVector3(position.X,position.Y,position.Z));
	Capsule->setCenterOfMassTransform(oTrans);
}

void player::update() {

	float fpsFactor = owner->getFPSfactor();

    Camera->updateAbsolutePosition();
    Camera->setPosition(Camera->getAbsolutePosition()+irr::core::vector3df(0.f,std::sin(shake*irr::core::DEGTORAD)*(1.f-(crouchState*1.f)),0.f));

    if (irrReceiver->IsKeyDown(irr::KEY_SPACE)) {
        if (BlinkTimer>0) { BlinkTimer = 0.0; }
        if (BlinkTimer<-0.8925) { BlinkTimer = -0.8925; }
    }

    irr::core::vector3df dir(0,1,0);
    irr::core::matrix4 rotMatrix;

    BlinkTimer-=fpsFactor*0.09;

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

    rotMatrix.setRotationDegrees(irr::core::vector3df(0,0,std::max(std::min(std::sin((shake)*irr::core::DEGTORAD/2.f)*2.5f,8.f),-8.f)*1.5f));
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

    sound::setListenerPos(Camera->getAbsolutePosition());
    sound::setListenerOrientation(Camera->getUpVector(),dir);

    Camera->setTarget(Camera->getAbsolutePosition()+dir);

    btVector3 speed = Capsule->getLinearVelocity();

    if (speed[1]-20.f>prevLinearVelocity[1] && dynamics->rayTest(Capsule->getCenterOfMassPosition(),Capsule->getCenterOfMassPosition()-btVector3(0,height/2.f,0))) {
		unsigned char chosen = rand()%4;
		stepSound[owner->pickPlayerTriangle()][1][chosen]->playSound(false);
    }

	float shakeFactor = 0.f;

    if (!dead) {
		sprintTimer=std::min(std::max(0.f,sprintTimer-0.1f*fpsFactor),10.f);
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
					if (Stamina>0) walkSpeed=80.f;
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

			if (stepTimer<=0.f) {
				unsigned char chosen = rand()%4;
				stepSound[owner->pickPlayerTriangle()][walkSpeed>40.f][chosen]->playSound(false);
				stepTimer = 360.f;
			}
			stepTimer-=walkSpeed*fpsFactor*0.15f;

			dir-=90;
			dir *= irr::core::DEGTORAD;
			Capsule->setFriction(0.15f);
			float xs,zs,nxs,nzs,d,dd;
			xs = speed[0];
			zs = speed[2];
			d = irr::core::squareroot(xs*xs+zs*zs);
			if (irr::core::squareroot(xs*xs+zs*zs)<walkSpeed*0.75f*RoomScale) {
				Capsule->applyCentralImpulse(btVector3(std::cos(dir+yaw*irr::core::DEGTORAD)*walkSpeed*2.f*RoomScale*fpsFactor,0.f,-std::sin(dir+yaw*irr::core::DEGTORAD)*walkSpeed*2.f*RoomScale*fpsFactor));
			} else {
				nxs = std::cos(dir+yaw*irr::core::DEGTORAD);
				nzs = -std::sin(dir+yaw*irr::core::DEGTORAD);
				nxs = std::atan2(nxs,nzs);
				nzs = std::atan2(xs,zs);
				while (nxs<-irr::core::PI) nxs+=irr::core::PI*2.f;
				while (nxs>irr::core::PI) nxs-=irr::core::PI*2.f;
				while (nzs<-irr::core::PI) nzs+=irr::core::PI*2.f;
				while (nzs>irr::core::PI) nzs-=irr::core::PI*2.f;
				dd = nxs - nzs;
				while (dd<-irr::core::PI) dd+=irr::core::PI*2.f;
				while (dd>irr::core::PI) dd-=irr::core::PI*2.f;
				nzs = nzs+dd * 0.08f * fpsFactor;
				nzs -= irr::core::PI/2.f;

				Capsule->setLinearVelocity(btVector3(std::cos(nzs)*d,speed[1],-std::sin(nzs)*d));
			}
			shakeFactor = walkSpeed;
		} else {
			if (std::abs(speed[1])<=4.f) Capsule->setFriction(3.0f); else Capsule->setFriction(1.0f);
			Stamina=std::min(Stamina+0.15*fpsFactor,100.0);
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

		crouchState=std::min(0.5f,crouchState+(0.5f-crouchState)*fpsFactor*0.2f);
		if (crouchState>0.4998f) crouchState = 0.5f;

		if (changed) { //must unregister body to perform changes to its shape
			dynamics->unregisterRBody(Capsule);
			Capsule->getCollisionShape()->setLocalScaling(btVector3(1.f,1.f-crouchState,1.f));
			dynamics->registerNewRBody(Camera,Capsule,-1,1,1,irr::core::vector3df(0,(-(height/2.f)*(1.f-crouchState))+(radius/1.5f),0));
			Capsule->setGravity(btVector3(0.f,-300.0f*RoomScale,0.f));
		}
	} else {
		if (crouchState>0.0f) changed = true;

		crouchState=std::max(0.f,crouchState+(-crouchState)*fpsFactor*0.1f);
		if (crouchState<0.002f) crouchState = 0.f;

		if (changed) { //must unregister body to perform changes to its shape
			dynamics->unregisterRBody(Capsule);
			Capsule->getCollisionShape()->setLocalScaling(btVector3(1.f,1.f-crouchState,1.f));
			dynamics->registerNewRBody(Camera,Capsule,-1,1,1,irr::core::vector3df(0,(-(height/2.f)*(1.f-crouchState))+(radius/1.5f),0));
			Capsule->setGravity(btVector3(0.f,-300.0f*RoomScale,0.f));
		}
	}
	//std::cout<<Capsule->getCollisionShape()->getLocalScaling()[1]<<" "<<crouchState<<"\n";

	prevLinearVelocity = Capsule->getLinearVelocity();

	shake+=shakeFactor*fpsFactor*0.15f; //owner->getFPSfactor()
    while (shake>720.f) shake-=720.f;
    while (shake<0.f) shake+=720.f;

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
