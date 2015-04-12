#ifndef TESTROOM_ROOM_INCLUDED
#define TESTROOM_ROOM_INCLUDED

class RoomTestroom : public Room {
	private:
		static RMesh* baseRMesh;
		RoomTestroom() {};
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static RoomTestroom* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
