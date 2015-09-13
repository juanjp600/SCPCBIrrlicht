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
		virtual void setNPC(unsigned char index,class NPC* npc) { return; };
		virtual void setIrrNode(unsigned char index,irr::scene::ISceneNode* node) { return; };
		virtual void setDoor(unsigned char index,class Door* door) { return; };
		virtual void setEvent(unsigned char event) { return; };
};

#endif // ROOM_H_INCLUDED
