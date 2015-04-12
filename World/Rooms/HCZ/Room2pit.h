#ifndef ROOM2PIT_ROOM_INCLUDED
#define ROOM2PIT_ROOM_INCLUDED

class Room2pit : public Room {
	private:
		static RMesh* baseRMesh;
		Room2pit() {};
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static Room2pit* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
