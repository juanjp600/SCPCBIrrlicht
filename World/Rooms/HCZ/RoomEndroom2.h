#ifndef ENDROOM2_ROOM_INCLUDED
#define ENDROOM2_ROOM_INCLUDED

class RoomEndroom2 : public Room {
	private:
		static RMesh* baseRMesh;
		RoomEndroom2() {};
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM1; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static RoomEndroom2* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
