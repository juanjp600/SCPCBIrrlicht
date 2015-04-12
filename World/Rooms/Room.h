#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED

#include "RMesh.h"
#include <irrDynamics.h>

enum class RoomTypes {
	ROOM1,ROOM2,ROOM2C,ROOM3,ROOM4
};

class Room {
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
		virtual short wpPathStep(RMesh* rme,unsigned char destWP,class TempWPPathList &currWP);

		std::vector<pointLight> pointLights;
	public:
		Room();
		virtual RoomTypes getType() const =0;
		virtual char getAngle() const { return angle; };
		virtual void updateEvent() =0;
		virtual void setActivation(bool s);
		const std::vector<pointLight>& getPointLights();
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
#include "LCZ/RoomLockroom.h"
#include "LCZ/RoomStart.h"
#include "LCZ/Room2storage.h"
#include "LCZ/Room3storage.h"
#include "LCZ/RoomEndroom.h"
#include "LCZ/Room012.h"
#include "LCZ/Room2.h"
#include "LCZ/Room2_2.h"
#include "LCZ/Room2c.h"
#include "LCZ/Room2closets.h"
#include "LCZ/Room2elevator.h"
#include "LCZ/Room2doors.h"
#include "LCZ/Room2scps.h"
#include "LCZ/Room3storage.h"
#include "LCZ/Room2testRoom2.h"
#include "LCZ/Room3.h"
#include "LCZ/Room3_2.h"
#include "LCZ/Room4.h"
#include "LCZ/Roompj.h"
#include "LCZ/Room914.h"
//HCZ
#include "HCZ/Room008.h"
#include "HCZ/RoomCoffin.h"
#include "HCZ/RoomEndRoom2.h"
#include "HCZ/RoomTestRoom.h"
#include "HCZ/RoomTunnel.h"
#include "HCZ/RoomTunnel2.h"
#include "HCZ/Room035.h"
#include "HCZ/Room049.h"
#include "HCZ/Room106.h"
#include "HCZ/Room2ctunnel.h"
#include "HCZ/Room2nuke.h"
#include "HCZ/Room2pipes.h"
#include "HCZ/Room2pit.h"
#include "HCZ/Room3pit.h"
#include "HCZ/Room2servers.h"
#include "HCZ/Room2tunnel.h"
#include "HCZ/Room3tunnel.h"
#include "HCZ/Room4tunnels.h"
#include "HCZ/Room513.h"
//EZ
#include "EZ/Room860.h"
#include "EZ/RoomExit1.h"
#include "EZ/RoomGateaentrance.h"
#include "EZ/RoomLockroom2.h"
#include "EZ/Room079.h"
#include "EZ/Room2z3.h"
#include "EZ/Room2cafeteria.h"
#include "EZ/Room2cz3.h"
#include "EZ/Room2ccont.h"
#include "EZ/Room2offices.h"
#include "EZ/Room2offices2.h"
#include "EZ/Room2offices3.h"
#include "EZ/Room2poffices.h"
#include "EZ/Room2poffices2.h"
#include "EZ/Room2sroom.h"
#include "EZ/Room2toilets.h"
#include "EZ/Room2tesla.h"
#include "EZ/Room3servers.h"
#include "EZ/Room3servers2.h"
#include "EZ/Room3z3.h"
#include "EZ/Room4z3.h"
//Misc
#include "Misc/Room173.h"
#include "Misc/RoomCheckpoint1.h"
#include "Misc/RoomCheckpoint2.h"
#include "Misc/RoomGatea.h"
#include "Misc/RoomPocketdimension.h"

#endif // ROOM_H_INCLUDED
