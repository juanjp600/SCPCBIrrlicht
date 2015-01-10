#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED

#include "rmesh.h"

enum class roomTypes {
	ROOM1,ROOM2,ROOM2C,ROOM3,ROOM4
};

class room {
	private:
		static irr::scene::ISceneManager* smgr;
	protected:
		char angle;
		irr::core::matrix4 rotMatrix;
		char selectedEvent;
		float eventState[3];
		irr::scene::ISceneNode* node;
		btRigidBody* rbody;

		static irrDynamics* dynamics;
		irr::scene::ITriangleSelector* getSelector(irr::scene::IMesh* mesh);
		static irr::scene::IMeshSceneNode* getNewNode(irr::scene::IMesh* mesh);

		bool isActivated = true;

		void loadAssets(RMesh* rme,irr::core::vector3df inPosition,float inAngle);

		signed char linkedTurnDists[4];

		virtual void findWPPath(RMesh* rme,irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
		virtual short wpPathStep(RMesh* rme,unsigned char destWP,class tempWPPathList &currWP);
	public:
		room();
		virtual roomTypes getType() const =0;
		virtual char getAngle() const { return angle; };
		virtual void updateEvent() =0;
		virtual void setActivation(bool s);
		virtual const std::vector<irr::video::SLight>& getPointLights()=0;
		virtual bool disableDecals() const { return false; }
		virtual void setLinkedTurnDist(unsigned char index,signed char value);
		virtual char getLinkedTurnDist(unsigned char index) const;
		static void setDynamics(irrDynamics* inDyn);
		static void setSmgr(irr::scene::ISceneManager* inSmgr);
		virtual void destroy();
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) =0;
};

//extern bool CustomMaterialCombinerCallback(btManifoldPoint& cp,const btCollisionObjectWrapper* colObj0,int partId0,int index0,const btCollisionObjectWrapper* colObj1,int partId1,int index1);

//LCZ
#include "LCZ/lockroom.h"
#include "LCZ/start.h"
#include "LCZ/room2storage.h"
#include "LCZ/room3storage.h"
#include "LCZ/endroom.h"
#include "LCZ/room012.h"
#include "LCZ/room2.h"
#include "LCZ/room2_2.h"
#include "LCZ/room2c.h"
#include "LCZ/room2closets.h"
#include "LCZ/room2elevator.h"
#include "LCZ/room2doors.h"
#include "LCZ/room2scps.h"
#include "LCZ/room3storage.h"
#include "LCZ/room2testroom2.h"
#include "LCZ/room3.h"
#include "LCZ/room3_2.h"
#include "LCZ/room4.h"
#include "LCZ/roompj.h"
#include "LCZ/r_914.h"
//HCZ
#include "HCZ/r_008.h"
#include "HCZ/coffin.h"
#include "HCZ/endroom2.h"
#include "HCZ/testroom.h"
#include "HCZ/tunnel.h"
#include "HCZ/tunnel2.h"
#include "HCZ/room035.h"
#include "HCZ/room049.h"
#include "HCZ/room106.h"
#include "HCZ/room2ctunnel.h"
#include "HCZ/room2nuke.h"
#include "HCZ/room2pipes.h"
#include "HCZ/room2pit.h"
#include "HCZ/room3pit.h"
#include "HCZ/room2servers.h"
#include "HCZ/room2tunnel.h"
#include "HCZ/room3tunnel.h"
#include "HCZ/room4tunnels.h"
#include "HCZ/room513.h"
//EZ
#include "EZ/room860.h"
#include "EZ/exit1.h"
#include "EZ/gateaentrance.h"
#include "EZ/lockroom2.h"
#include "EZ/room079.h"
#include "EZ/room2z3.h"
#include "EZ/room2cafeteria.h"
#include "EZ/room2cz3.h"
#include "EZ/room2ccont.h"
#include "EZ/room2offices.h"
#include "EZ/room2offices2.h"
#include "EZ/room2offices3.h"
#include "EZ/room2poffices.h"
#include "EZ/room2poffices2.h"
#include "EZ/room2sroom.h"
#include "EZ/room2toilets.h"
#include "EZ/room2tesla.h"
#include "EZ/room3servers.h"
#include "EZ/room3servers2.h"
#include "EZ/room3z3.h"
#include "EZ/room4z3.h"
//Misc
#include "Misc/r_173.h"
#include "Misc/checkpoint1.h"
#include "Misc/checkpoint2.h"
#include "Misc/gatea.h"
#include "Misc/pocketdimension.h"

#endif // ROOM_H_INCLUDED
