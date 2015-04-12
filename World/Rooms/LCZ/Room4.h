#ifndef ROOM4_ROOM_INCLUDED
#define ROOM4_ROOM_INCLUDED

class Room4 : public Room {
	private:
		static RMesh* baseRMesh;
		Room4() {};
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM4; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static Room4* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
