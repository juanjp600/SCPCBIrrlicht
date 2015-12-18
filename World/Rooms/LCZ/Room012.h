#ifndef ROOM012_ROOM_INCLUDED
#define ROOM012_ROOM_INCLUDED

class Room012 : public Room {
	private:
		static RMesh* baseRMesh;
		Room012() {};
		class Door* doors[3];
		int state = 0;
		irr::core::vector3df standPos;
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM2; }
		virtual void updateEvent();
		virtual void setDoor(unsigned char index,class Door* door);
		static void setBase(RMesh* inRme);
		static Room012* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
