#ifndef ROOM2OFFICES3_ROOM_INCLUDED
#define ROOM2OFFICES3_ROOM_INCLUDED

class Room2offices3 : public Room {
	private:
		static RMesh* baseRMesh;
		Room2offices3() {};
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static Room2offices3* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
