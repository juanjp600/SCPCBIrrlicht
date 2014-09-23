#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED

#include "rmesh.h"

enum class roomTypes {
	ROOM1,ROOM2,ROOM2C,ROOM3,ROOM4
};

class room {
	protected:
		char angle;
		float eventState[3];
		irr::scene::IMeshSceneNode* node;
		btRigidBody* rbody;
	public:
		virtual roomTypes getType() =0;
		virtual char getAngle() { return angle; };
		virtual void updateEvent() =0;
};

//these are for event-less rooms, they can share the same subclass
class room1 : public room {
	private:
		struct assets {
			irr::scene::IMeshSceneNode* node;
			btBvhTriangleMeshShape* shape;
		};
		static std::map<std::string,assets> asset;
	public:
		virtual roomTypes getType() { return roomTypes::ROOM1; };
		virtual void updateEvent() { return; };
};

class room2 : public room {
	private:
		struct assets {
			irr::scene::IMeshSceneNode* node;
			btBvhTriangleMeshShape* shape;
		};
		static std::map<std::string,assets> asset;
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2; };
		virtual void updateEvent() { return; };
};

class room2c : public room {
	private:
		struct assets {
			irr::scene::IMeshSceneNode* node;
			btBvhTriangleMeshShape* shape;
		};
		static std::map<std::string,assets> asset;
	public:
		virtual roomTypes getType() { return roomTypes::ROOM2C; };
		virtual void updateEvent() { return; };
};

class room3 : public room {
	private:
		struct assets {
			irr::scene::IMeshSceneNode* node;
			btBvhTriangleMeshShape* shape;
		};
		static std::map<std::string,assets> asset;
	public:
		virtual roomTypes getType() { return roomTypes::ROOM3; };
		virtual void updateEvent() { return; };
};

class room4 : public room {
	private:
		struct assets {
			irr::scene::IMeshSceneNode* node;
			btBvhTriangleMeshShape* shape;
		};
		static std::map<std::string,assets> asset;
	public:
		virtual roomTypes getType() { return roomTypes::ROOM4; };
		virtual void updateEvent() { return; };
};
//-----------------

#endif // ROOM_H_INCLUDED
