#ifndef ROOM2STORAGE_ROOM_INCLUDED
#define ROOM2STORAGE_ROOM_INCLUDED

class Room2storage : public Room {
	private:
		static RMesh* baseRMesh;
		Room2storage() {};
		irr::core::aabbox3df tpBBox1;
		irr::core::vector3df tpPos1S; irr::core::vector3df tpPos1D;
		irr::core::aabbox3df tpBBox2;
		irr::core::vector3df tpPos2S; irr::core::vector3df tpPos2D;
		class Door* doors[4];
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM2; }
		virtual void updateEvent();
		static void setBase(RMesh* inRme);
		static Room2storage* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
		virtual void setDoor(unsigned char index,class Door* door);
};

#endif
