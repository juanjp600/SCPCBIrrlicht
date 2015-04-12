#ifndef ROOM2POFFICES2_ROOM_INCLUDED
#define ROOM2POFFICES2_ROOM_INCLUDED

class Room2poffices2 : public Room {
	private:
		static RMesh* baseRMesh;
		Room2poffices2() {};
	public:
		virtual RoomTypes getType() const { return RoomTypes::ROOM2; }
		virtual void updateEvent() { return; }
		static void setBase(RMesh* inRme);
		static Room2poffices2* createNew(irr::core::vector3df inPosition,char inAngle);
		virtual void findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList);
};

#endif
