#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED

#include "rmesh.h"

enum class roomTypes {
	ROOM1,ROOM2,ROOM2C,ROOM3,ROOM4
};

class room {
	protected:
		char angle;
		char selectedEvent;
		float eventState[3];
		irr::scene::ISceneNode* node;
		btRigidBody* rbody;

		static dynRegister* dynamics;
	public:
		virtual roomTypes getType() =0;
		virtual char getAngle() { return angle; };
		virtual void updateEvent() =0;
		void setDynamics(dynRegister* inDyn);
};

#include "lcz_generic.h"

#endif // ROOM_H_INCLUDED
