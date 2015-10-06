#ifndef ROOM2DOORS_ROOM_INCLUDED
#define ROOM2DOORS_ROOM_INCLUDED

class Room2doors : public Room {
	private:
		static RMesh* baseRMesh;
		class Door* doors[2];
		unsigned short state = 0;
		Room2doors() {};
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM2; }
		virtual void updateEvent();
		virtual void setDoor(unsigned char index,class Door* door);
		static void setBase(RMesh* inRme);
		static Room2doors* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
